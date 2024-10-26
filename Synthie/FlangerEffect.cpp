#include "stdafx.h"
#include "FlangerEffect.h"
#include <cmath>

const double M_PI = 3.14;

FlangerEffect::FlangerEffect()
    : m_bufferSize(0), m_depth(0.004 * 44100), m_rate(0.25), m_feedback(0.8), m_wetLevel(0.6), m_dryLevel(0.5), m_phase(0.0)
{
    m_lastOutput[0] = m_lastOutput[1] = 0.0;
}

FlangerEffect::~FlangerEffect()
{
}

void FlangerEffect::Start()
{
    m_bufferSize = static_cast<size_t>((m_depth * 2 + 0.05 * m_sampleRate)); // Buffer large enough for modulation
    m_delayBuffer[0].resize(m_bufferSize, 0.0);
    m_delayBuffer[1].resize(m_bufferSize, 0.0);
    m_phase = 0.0;
    m_lastOutput[0] = m_lastOutput[1] = 0.0;
}

void FlangerEffect::SetDepth(double depth)
{
    m_depth = depth * m_sampleRate; // Convert from seconds to samples
}

void FlangerEffect::SetRate(double rate)
{
    m_rate = rate;
}

void FlangerEffect::SetFeedback(double feedback)
{
    m_feedback = feedback;
}

void FlangerEffect::SetWet(double wetLevel)
{
    m_wetLevel = wetLevel;
}

void FlangerEffect::SetDry(double dryLevel)
{
    m_dryLevel = dryLevel;
}

void FlangerEffect::Process(double* input, double* output)
{
    for (int c = 0; c < 2; ++c)
    {
        // Calculate modulated delay time
        double modulatedDelay = m_depth * (1.0 + sin(2 * M_PI * m_phase)) / 2.0;
        size_t delaySamples = static_cast<size_t>(modulatedDelay);

        // Read delayed sample
        size_t bufferSize = m_delayBuffer[c].size();
        size_t readIndex = bufferSize > delaySamples ? bufferSize - 1 - delaySamples : 0;
        double delayedSample = m_delayBuffer[c][readIndex];

        // Write input + feedback to delay buffer
        double bufferInput = input[c] + m_lastOutput[c] * m_feedback;
        m_delayBuffer[c].push_back(bufferInput);

        // Remove oldest sample if buffer is too large
        if (m_delayBuffer[c].size() > m_bufferSize)
        {
            m_delayBuffer[c].erase(m_delayBuffer[c].begin());
        }

        // Combine dry and wet signals
        output[c] = m_dryLevel * input[c] + m_wetLevel * delayedSample;

        // Store last output for feedback
        m_lastOutput[c] = output[c];
    }

    // Advance phase
    m_phase += m_rate / m_sampleRate;
    if (m_phase >= 1.0)
    {
        m_phase -= 1.0;
    }
}