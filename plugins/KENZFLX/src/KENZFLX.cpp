#include "mpc_plugin.h"
#include "KenzflxPresets.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <vector>

namespace {

constexpr float kPi = 3.14159265358979323846f;
constexpr float kTwoPi = 2.0f * kPi;

static inline float softClip(float x)
{
    return std::tanh(x * 1.5f);
}

struct StereoDelay
{
    std::vector<float> buffer;
    size_t writeIndex = 0;
    size_t maxSamples = 0;

    void prepare(size_t maxSamples_)
    {
        maxSamples = std::max<size_t>(2, maxSamples_);
        buffer.assign(maxSamples, 0.0f);
        writeIndex = 0;
    }

    void reset()
    {
        std::fill(buffer.begin(), buffer.end(), 0.0f);
        writeIndex = 0;
    }

    float process(float input, float delayMs, float feedback, float wet, float sampleRate)
    {
        if (buffer.empty())
            return input;

        const size_t delaySamples = std::clamp(
            static_cast<size_t>(delayMs * 0.001f * sampleRate),
            size_t{1},
            maxSamples - 1);

        const size_t readIndex = (writeIndex + maxSamples - delaySamples) % maxSamples;
        const float delayed = buffer[readIndex];

        buffer[writeIndex] = input + delayed * feedback;
        writeIndex = (writeIndex + 1) % maxSamples;

        return input * (1.0f - wet) + delayed * wet;
    }
};

struct StereoChorus
{
    StereoDelay left;
    StereoDelay right;
    float phase = 0.0f;
    float sampleRate = 44100.0f;

    void prepare(float rate)
    {
        sampleRate = std::max(1.0f, rate);
        left.prepare(static_cast<size_t>(sampleRate * 0.35f));
        right.prepare(static_cast<size_t>(sampleRate * 0.35f));
        phase = 0.0f;
    }

    void reset()
    {
        left.reset();
        right.reset();
        phase = 0.0f;
    }

    void process(float inL, float inR, float amount, float rateHz, float& outL, float& outR)
    {
        const float lfo = std::sin(phase * kTwoPi);
        const float modL = 4.0f + amount * 16.0f + lfo * (4.0f + amount * 14.0f);
        const float modR = 6.0f + amount * 18.0f - lfo * (4.0f + amount * 12.0f);

        phase += rateHz / sampleRate;
        if (phase >= 1.0f)
            phase -= 1.0f;

        outL = left.process(inL, modL, 0.34f + amount * 0.4f, 0.45f + amount * 0.45f, sampleRate);
        outR = right.process(inR, modR, 0.32f + amount * 0.38f, 0.45f + amount * 0.45f, sampleRate);
    }
};

struct StereoReverb
{
    StereoDelay d1;
    StereoDelay d2;
    StereoDelay d3;
    StereoDelay d4;
    float sampleRate = 44100.0f;

    void prepare(float rate)
    {
        sampleRate = std::max(1.0f, rate);
        d1.prepare(static_cast<size_t>(sampleRate * 0.12f));
        d2.prepare(static_cast<size_t>(sampleRate * 0.18f));
        d3.prepare(static_cast<size_t>(sampleRate * 0.25f));
        d4.prepare(static_cast<size_t>(sampleRate * 0.33f));
    }

    void reset()
    {
        d1.reset();
        d2.reset();
        d3.reset();
        d4.reset();
    }

    void process(float inL, float inR, float amount, float damp, float& outL, float& outR)
    {
        const float w = amount * 0.8f;

        const float aL = d1.process(inL, 32.0f + amount * 80.0f, 0.72f, w, sampleRate);
        const float aR = d2.process(inR, 40.0f + amount * 90.0f, 0.70f, w, sampleRate);

        const float bL = d3.process(aL, 72.0f + amount * 130.0f, 0.68f, w * 0.7f, sampleRate);
        const float bR = d4.process(aR, 84.0f + amount * 140.0f, 0.66f, w * 0.7f, sampleRate);

        const float lp = 0.2f + damp * 0.7f;
        outL = aL * (1.0f - lp) + bL * lp;
        outR = aR * (1.0f - lp) + bR * lp;
    }
};

struct TempoSliceEngine
{
    std::vector<float> historyL;
    std::vector<float> historyR;
    size_t historySize = 0;
    size_t writeIndex = 0;

    void prepare(float sampleRate)
    {
        historySize = std::max<size_t>(4096, static_cast<size_t>(sampleRate * 32.0f));
        historyL.assign(historySize, 0.0f);
        historyR.assign(historySize, 0.0f);
        writeIndex = 0;
    }

    void reset()
    {
        std::fill(historyL.begin(), historyL.end(), 0.0f);
        std::fill(historyR.begin(), historyR.end(), 0.0f);
        writeIndex = 0;
    }

    static inline float crossFade(float a, float b, float x)
    {
        const float t = std::clamp(x, 0.0f, 1.0f);
        return a * (1.0f - t) + b * t;
    }

    float read(const std::vector<float>& buffer, float delaySamples) const
    {
        if (historySize == 0)
            return 0.0f;

        const float delay = std::clamp(delaySamples, 1.0f, static_cast<float>(historySize - 1));
        float position = static_cast<float>(writeIndex) - delay;
        if (position < 0.0f)
            position += static_cast<float>(historySize);

        const size_t index0 = static_cast<size_t>(position);
        const size_t index1 = (index0 + 1) % historySize;
        const float fraction = position - static_cast<float>(index0);
        return buffer[index0] + (buffer[index1] - buffer[index0]) * fraction;
    }

    void process(float inL, float inR, double ppqPos, double bpm, float sampleRate,
                 int numerator, int denominator, int loopLength,
                 float halfTime, float glitch, float repeat, float reverse,
                 float& outL, float& outR)
    {
        if (historySize == 0)
        {
            outL = inL;
            outR = inR;
            return;
        }

        historyL[writeIndex] = inL;
        historyR[writeIndex] = inR;
        writeIndex = (writeIndex + 1) % historySize;

        const double safeBpm = std::max(1.0, bpm);
        const double samplesPerBeat = static_cast<double>(sampleRate) * 60.0 / safeBpm;
        const double beatPhase = ppqPos - std::floor(ppqPos);
        float wetL = inL;
        float wetR = inR;

        if (halfTime > 0.001f)
        {
            static constexpr std::array<double, 4> loopBars = {0.5, 1.0, 2.0, 4.0};
            const double beatsPerBar =
                std::max(1, numerator) * 4.0 / std::max(1, denominator);
            const double sourceSpan = beatsPerBar * loopBars[std::clamp(loopLength, 0, 3)];
            const double cycleBeats = sourceSpan * 2.0;
            const double cyclePhase = ppqPos - std::floor(ppqPos / cycleBeats) * cycleBeats;
            const float delay = static_cast<float>((sourceSpan + cyclePhase * 0.5) * samplesPerBeat);
            wetL = crossFade(wetL, read(historyL, delay), halfTime);
            wetR = crossFade(wetR, read(historyR, delay), halfTime);
        }

        if (glitch > 0.001f)
        {
            static constexpr std::array<std::array<int, 8>, 4> patterns = {{
                {{0, 1, 1, 3, 2, 2, 5, 7}},
                {{0, 3, 1, 1, 4, 6, 2, 7}},
                {{0, 0, 2, 3, 1, 5, 5, 7}},
                {{7, 6, 5, 4, 3, 2, 1, 0}}
            }};

            const int subdivisions = glitch < 0.34f ? 2 : (glitch < 0.67f ? 4 : 8);
            const double subdivisionPhase = beatPhase * subdivisions;
            const int subdivisionIndex = std::min(subdivisions - 1, static_cast<int>(subdivisionPhase));
            const double phaseInSlice = subdivisionPhase - subdivisionIndex;
            const int patternIndex = std::min(3, static_cast<int>(glitch * 4.0f));
            const int patternCell = std::min(7, subdivisionIndex * 8 / subdivisions);
            const int mappedCell = patterns[patternIndex][patternCell];
            const int sourceIndex = mappedCell * subdivisions / 8;
            const double sourcePosition = (sourceIndex + phaseInSlice) / subdivisions;
            const double ageBeats = 1.0 + beatPhase - sourcePosition;
            const float delay = static_cast<float>(ageBeats * samplesPerBeat);

            const float glitchL = read(historyL, delay);
            const float glitchR = read(historyR, delay);
            const float amount = std::clamp(glitch * 1.6f, 0.0f, 1.0f);
            wetL = crossFade(wetL, glitchL, amount);
            wetR = crossFade(wetR, glitchR, amount);
        }

        if (repeat > 0.001f)
        {
            const int subdivisions = repeat < 0.25f ? 2 : (repeat < 0.50f ? 4 : (repeat < 0.75f ? 8 : 16));
            const double subdivisionPhase = beatPhase * subdivisions;
            const double phaseInSlice = subdivisionPhase - std::floor(subdivisionPhase);
            const double sourcePosition = (1.0 - 1.0 / subdivisions) + phaseInSlice / subdivisions;
            const double ageBeats = 1.0 + beatPhase - sourcePosition;
            const float delay = static_cast<float>(ageBeats * samplesPerBeat);
            wetL = crossFade(wetL, read(historyL, delay), repeat);
            wetR = crossFade(wetR, read(historyR, delay), repeat);
        }

        if (reverse > 0.001f)
        {
            const float delay = static_cast<float>((1.0 + beatPhase * 2.0) * samplesPerBeat);
            wetL = crossFade(wetL, read(historyL, delay), reverse);
            wetR = crossFade(wetR, read(historyR, delay), reverse);
        }

        outL = wetL;
        outR = wetR;
    }
};

struct KenzFlxDSP
{
    float output = 1.0f;
    float drive = 0.45f;
    float warmth = 0.50f;
    float tone = 0.50f;
    float chorus = 0.25f;
    float delay = 0.35f;
    float reverb = 0.30f;
    float crush = 0.10f;
    float mix = 0.70f;
    float reverse = 0.0f;
    float halfTime = 0.0f;
    float glitch = 0.0f;
    float repeat = 0.0f;
    int loopLength = 0;
    float inputGainDb = 0.0f;

    float sampleRate = 44100.0f;
    double bpm = 120.0;
    double ppqPos = 0.0;
    double freePpq = 0.0;
    bool hostPpqValid = false;
    bool transportPlaying = false;
    int timeSigNumerator = 4;
    int timeSigDenominator = 4;

    StereoChorus chorusStage;
    StereoDelay delayStageL;
    StereoDelay delayStageR;
    StereoReverb reverbStage;
    TempoSliceEngine timeEngine;

    float lpL = 0.0f;
    float lpR = 0.0f;
    float hpL = 0.0f;
    float hpR = 0.0f;
    float hpInputL = 0.0f;
    float hpInputR = 0.0f;

    void prepare(double sr)
    {
        sampleRate = std::max(1.0f, static_cast<float>(sr));
        chorusStage.prepare(sampleRate);
        delayStageL.prepare(static_cast<size_t>(sampleRate * 0.8f));
        delayStageR.prepare(static_cast<size_t>(sampleRate * 0.8f));
        reverbStage.prepare(sampleRate);
        timeEngine.prepare(sampleRate);
        lpL = lpR = hpL = hpR = hpInputL = hpInputR = 0.0f;
    }

    void reset()
    {
        lpL = lpR = hpL = hpR = hpInputL = hpInputR = 0.0f;
        freePpq = 0.0;
        chorusStage.reset();
        delayStageL.reset();
        delayStageR.reset();
        reverbStage.reset();
        timeEngine.reset();
    }

    void processBlock(float* leftChannel, float* rightChannel, int numSamples)
    {
        if (numSamples <= 0)
            return;

        const float inputGainLinear = std::pow(10.0f, inputGainDb / 20.0f);
        const float steps = std::max(8.0f, 256.0f * (1.0f - crush * 0.9f));
        const double ppqPerSample = static_cast<double>(bpm) / (60.0 * sampleRate);
        const double blockPpq = hostPpqValid && transportPlaying ? ppqPos : freePpq;

        for (int i = 0; i < numSamples; ++i)
        {
            const float inL = leftChannel[i] * inputGainLinear;
            const float inR = rightChannel[i] * inputGainLinear;

            float sL = softClip(inL * (1.0f + drive * 3.5f));
            float sR = softClip(inR * (1.0f + drive * 3.5f));

            const float toneNorm = std::clamp(tone, 0.0f, 1.0f);
            float lowAlpha = 0.0f;
            float highAlpha = 0.0f;

            if (toneNorm < 0.45f)
            {
                const float r = toneNorm / 0.45f;
                const float cutoff = 180.0f + r * r * 16000.0f;
                const float rc = 1.0f / (kTwoPi * cutoff);
                lowAlpha = (1.0f / sampleRate) / (rc + 1.0f / sampleRate);
            }
            else if (toneNorm > 0.55f)
            {
                const float r = (toneNorm - 0.55f) / 0.45f;
                const float cutoff = 45.0f + r * r * 6000.0f;
                const float rc = 1.0f / (kTwoPi * cutoff);
                highAlpha = rc / (rc + 1.0f / sampleRate);
            }

            if (lowAlpha > 0.0f)
            {
                lpL += lowAlpha * (sL - lpL);
                lpR += lowAlpha * (sR - lpR);
                sL = lpL;
                sR = lpR;
            }
            else if (highAlpha > 0.0f)
            {
                hpL = highAlpha * (hpL + sL - hpInputL);
                hpR = highAlpha * (hpR + sR - hpInputR);
                hpInputL = sL;
                hpInputR = sR;
                sL = hpL;
                sR = hpR;
            }
            else
            {
                hpInputL = sL;
                hpInputR = sR;
            }

            float timeL = sL;
            float timeR = sR;
            float tempL = sL;
            float tempR = sR;
            timeEngine.process(sL, sR, blockPpq + i * ppqPerSample, bpm, sampleRate,
                               timeSigNumerator, timeSigDenominator, loopLength,
                               halfTime, glitch, repeat, reverse, tempL, tempR);
            timeL = tempL;
            timeR = tempR;

            float chorusL = 0.0f;
            float chorusR = 0.0f;
            chorusStage.process(timeL, timeR, chorus, 0.25f + chorus * 1.6f, chorusL, chorusR);

            const float delayMs = 30.0f + delay * 220.0f;
            const float delayWet = 0.2f + delay * 0.7f;
            const float delayFeedback = 0.30f + delay * 0.45f;

            const float dL = delayStageL.process(chorusL, delayMs, delayFeedback, delayWet, sampleRate);
            const float dR = delayStageR.process(chorusR, delayMs * 1.1f, delayFeedback, delayWet, sampleRate);

            float rvL = 0.0f;
            float rvR = 0.0f;
            reverbStage.process(chorusL + dL, chorusR + dR, reverb, 0.3f + warmth * 0.5f, rvL, rvR);

            float crushL = timeL + dL * 0.8f;
            float crushR = timeR + dR * 0.8f;

            if (crush > 0.01f)
            {
                crushL = std::round(crushL * steps) / steps;
                crushR = std::round(crushR * steps) / steps;
            }

            const float finalMix = std::clamp(mix, 0.0f, 1.0f);
            float outL = inL * (1.0f - finalMix) +
                         ((crushL * (1.0f - reverb * 0.5f)) + rvL * reverb * 0.6f + dL * delay * 0.5f) * finalMix;
            float outR = inR * (1.0f - finalMix) +
                         ((crushR * (1.0f - reverb * 0.5f)) + rvR * reverb * 0.6f + dR * delay * 0.5f) * finalMix;

            outL = softClip(outL * (1.0f + warmth * 1.3f)) * output;
            outR = softClip(outR * (1.0f + warmth * 1.3f)) * output;

            leftChannel[i] = outL;
            rightChannel[i] = outR;
        }

        freePpq = blockPpq + numSamples * ppqPerSample;
    }
};

} // namespace

class KENZFLX : public mpc::Plugin
{
public:
    enum Params
    {
        kOutput = 0,
        kDrive,
        kWarmth,
        kTone,
        kChorus,
        kDelay,
        kReverb,
        kCrush,
        kMix,
        kReverse,
        kInputGain,
        kHalfTime,
        kGlitch,
        kRepeat,
        kLoopLength,
        kNumParams
    };

    static mpc::PluginInfo makeInfo()
    {
        mpc::PluginInfo info;
        info.name = "KENZFLX";
        info.vendor = "KENZ";
        info.uniqueId = MPC_FOURCC('K', 'F', 'V', 'B');
        info.version = 1100;
        info.isInstrument = false;
        info.numInputs = 2;
        info.numOutputs = 2;
        info.tailSamples = 0;
        return info;
    }

    KENZFLX() : mpc::Plugin(makeInfo())
    {
        addParam(mpc::floatParam("output", "Output", 0.0f, 1.0f, 1.0f, "", 2));
        addParam(mpc::floatParam("drive", "Drive", 0.0f, 1.0f, 0.45f, "", 2));
        addParam(mpc::floatParam("warmth", "Warmth", 0.0f, 1.0f, 0.50f, "", 2));
        addParam(mpc::floatParam("tone", "Tone", 0.0f, 1.0f, 0.50f, "", 2));
        addParam(mpc::floatParam("chorus", "Chorus", 0.0f, 1.0f, 0.25f, "", 2));
        addParam(mpc::floatParam("delay", "Delay", 0.0f, 1.0f, 0.35f, "", 2));
        addParam(mpc::floatParam("reverb", "Reverb", 0.0f, 1.0f, 0.30f, "", 2));
        addParam(mpc::floatParam("crush", "Crush", 0.0f, 1.0f, 0.10f, "", 2));
        addParam(mpc::floatParam("mix", "Mix", 0.0f, 1.0f, 0.70f, "", 2));
        addParam(mpc::floatParam("reverse", "Reverse", 0.0f, 1.0f, 0.0f, "", 2));
        addParam(mpc::floatParam("inputGain", "Input Gain", -24.0f, 0.0f, 0.0f, "dB", 1));
        addParam(mpc::floatParam("halfTime", "Half-Time", 0.0f, 1.0f, 0.0f, "", 2));
        addParam(mpc::floatParam("glitch", "Glitch", 0.0f, 1.0f, 0.0f, "", 2));
        addParam(mpc::floatParam("repeat", "Repeat", 0.0f, 1.0f, 0.0f, "", 2));
        addParam(mpc::choiceParam("loopLength", "Loop Length",
                                  {"1/2 Bar", "1 Bar", "2 Bars", "4 Bars"}, 0));

        for (int i = 0; i < kNumPresets; ++i)
        {
            const KenzflxPreset& p = kKenzflxPresets[i];
            const char* name = p.name;
            const bool isHalfTime = std::strncmp(name, "[HALF]", 6) == 0;
            const bool isGlitch = std::strncmp(name, "[FLEX]", 6) == 0 ||
                                  std::strncmp(name, "[FX]", 4) == 0;
            const bool isReverse = std::strncmp(name, "[REV]", 5) == 0;
            const bool isRepeat = std::strstr(name, "Stutter") != nullptr ||
                                  std::strstr(name, "Bounce Gate") != nullptr;
            addProgram(p.name, {
                1.0f,
                p.drive,
                p.warmth,
                p.tone,
                p.chorus,
                p.delay,
                p.reverb,
                p.crush,
                p.mix,
                isReverse ? 0.5f + p.reverse * 0.5f : 0.0f,
                0.0f,
                isHalfTime ? 0.5f + p.reverse * 0.5f : 0.0f,
                isGlitch ? 0.5f + p.reverse * 0.5f : 0.0f,
                isRepeat ? std::max(0.6f, p.reverse) : 0.0f,
                0.0f
            });
        }

        setProgram(0);
    }

    void prepare(double sampleRate, int /*maxBlockSize*/) override
    {
        dsp_.prepare(sampleRate);
    }

    void reset() override
    {
        dsp_.reset();
    }

    void process(mpc::ProcessContext& ctx) override
    {
        if (ctx.numInputs < 1 || ctx.numOutputs < 1 ||
            ctx.inputs == nullptr || ctx.outputs == nullptr ||
            ctx.inputs[0] == nullptr || ctx.outputs[0] == nullptr)
            return;

        const float* inputL = ctx.inputs[0];
        const float* inputR =
            (ctx.numInputs > 1 && ctx.inputs[1] != nullptr) ? ctx.inputs[1] : ctx.inputs[0];

        float* outputL = ctx.outputs[0];
        float* outputR =
            (ctx.numOutputs > 1 && ctx.outputs[1] != nullptr) ? ctx.outputs[1] : ctx.outputs[0];

        dsp_.output = get(kOutput);
        dsp_.drive = get(kDrive);
        dsp_.warmth = get(kWarmth);
        dsp_.tone = get(kTone);
        dsp_.chorus = get(kChorus);
        dsp_.delay = get(kDelay);
        dsp_.reverb = get(kReverb);
        dsp_.crush = get(kCrush);
        dsp_.mix = get(kMix);
        dsp_.reverse = get(kReverse);
        dsp_.inputGainDb = get(kInputGain);
        dsp_.halfTime = get(kHalfTime);
        dsp_.glitch = get(kGlitch);
        dsp_.repeat = get(kRepeat);
        dsp_.loopLength = getInt(kLoopLength);

        if (ctx.transport.valid)
        {
            dsp_.bpm = ctx.transport.tempoValid ? ctx.transport.bpm : 120.0;
            dsp_.ppqPos = ctx.transport.ppqValid ? ctx.transport.ppqPos : 0.0;
            dsp_.hostPpqValid = ctx.transport.ppqValid;
            dsp_.transportPlaying = ctx.transport.playing;
            dsp_.timeSigNumerator = ctx.transport.timeSigNumerator > 0 ? ctx.transport.timeSigNumerator : 4;
            dsp_.timeSigDenominator = ctx.transport.timeSigDenominator > 0 ? ctx.transport.timeSigDenominator : 4;
        }
        else
        {
            dsp_.hostPpqValid = false;
            dsp_.transportPlaying = false;
        }

        if (inputL != outputL)
            std::copy(inputL, inputL + ctx.numFrames, outputL);

        if (outputR != outputL && inputR != outputR)
            std::copy(inputR, inputR + ctx.numFrames, outputR);

        if (outputR != outputL)
            dsp_.processBlock(outputL, outputR, ctx.numFrames);
        else
            dsp_.processBlock(outputL, outputL, ctx.numFrames);
    }

private:
    KenzFlxDSP dsp_;
};

MPC_PLUGIN_ENTRY(KENZFLX)
