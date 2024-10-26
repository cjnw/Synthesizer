#include "stdafx.h"
#include "ChorusEffect.h"
#include <cmath>

double M_PI = 3.14;
ChorusEffect::ChorusEffect()
    : m_bufferSize(0), m_depth(0.002 * 44100), m_rate(0.5), m_wetLevel(0.5), m_dryLevel(0.5), m_phase(0.0)
{
}

ChorusEffect::~ChorusEffect()
{
}

void ChorusEffect::Start()
{
    m_bufferSize = static_cast<size_t>((m_depth * 2 + 0.05 * m_sampleRate)); // Buffer large enough for modulation
    m_delayBuffer[0].resize(m_bufferSize, 0.0);
    m_delayBuffer[1].resize(m_bufferSize, 0.0);
    m_phase = 0.0;
}

void ChorusEffect::SetDepth(double depth)
{
    m_depth = depth * m_sampleRate; // Convert from seconds to samples
}

void ChorusEffect::SetRate(double rate)
{
    m_rate = rate;
}

void ChorusEffect::SetWet(double wetLevel)
{
    m_wetLevel = wetLevel;
}

void ChorusEffect::SetDry(double dryLevel)
{
    m_dryLevel = dryLevel;
}

void ChorusEffect::Process(double* input, double* output)
{
    for (int c = 0; c < 2; ++c)
    {
        // Write input sample to delay buffer
        m_delayBuffer[c].push_back(input[c]);

        // Remove oldest sample if buffer is too large
        if (m_delayBuffer[c].size() > m_bufferSize)
        {
            m_delayBuffer[c].erase(m_delayBuffer[c].begin());
        }

        // Calculate modulated delay time
        double modulatedDelay = m_depth * (1.0 + sin(2 * M_PI * m_phase)) / 2.0;
        size_t delaySamples = static_cast<size_t>(modulatedDelay);

        // Read delayed sample
        size_t readIndex = m_delayBuffer[c].size() > delaySamples ? m_delayBuffer[c].size() - 1 - delaySamples : 0;
        double delayedSample = m_delayBuffer[c][readIndex];

        // Combine dry and wet signals
        output[c] = m_dryLevel * input[c] + m_wetLevel * delayedSample;
    }

    // Advance phase
    m_phase += m_rate / m_sampleRate;
    if (m_phase >= 1.0)
    {
        m_phase -= 1.0;
    }
}