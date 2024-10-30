#include "stdafx.h"
#include "ChorusEffect.h"
#include <cmath>
#include <vector>

double pi = 3.14159265358979323846;

ChorusEffect::ChorusEffect()
    : m_bufferSize(0), m_depth(0.002 * 44100), m_rate(0.5), m_phase(0.0), m_writeIndex(0)
{
}

ChorusEffect::~ChorusEffect()
{
}

void ChorusEffect::Start()
{
    // Initialize buffer size with a margin
    m_bufferSize = static_cast<size_t>(m_depth + 0.05 * m_sampleRate);
    m_delayBuffer[0].resize(m_bufferSize, 0.0);
    m_delayBuffer[1].resize(m_bufferSize, 0.0);
    m_writeIndex = 0;
    m_phase = 0.0;
}

void ChorusEffect::SetDepth(double depth)
{
    m_depth = depth * m_sampleRate; // Depth in samples
}

void ChorusEffect::SetRate(double rate)
{
    m_rate = rate;
}

void ChorusEffect::Process(double* input, double* output)
{
    for (int c = 0; c < 2; ++c)
    {
        // Write current sample to delay buffer at write index
        m_delayBuffer[c][m_writeIndex] = input[c];

        // Calculate the modulated delay time
        double modulatedDelay = m_depth * (1.0 + sin(2 * pi * m_phase)) / 2.0;
        size_t intDelay = static_cast<size_t>(modulatedDelay);
        double fracDelay = modulatedDelay - intDelay;

        // Calculate read index, wrapping if necessary
        size_t readIndex = (m_writeIndex + m_bufferSize - intDelay) % m_bufferSize;
        size_t nextReadIndex = (readIndex + 1) % m_bufferSize;

        // Interpolate between the delay buffer samples for a smooth output
        double delayedSample = (1.0 - fracDelay) * m_delayBuffer[c][readIndex] +
            fracDelay * m_delayBuffer[c][nextReadIndex];

        // Combine dry and wet signals
        output[c] = dryLevel * input[c] + wetLevel * delayedSample;
    }

    // Advance write index and phase
    m_writeIndex = (m_writeIndex + 1) % m_bufferSize;
    m_phase += m_rate / m_sampleRate;
    if (m_phase >= 1.0)
    {
        m_phase -= 1.0;
    }
}