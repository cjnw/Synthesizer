#include "stdafx.h"
#include "FlangerEffect.h"
#include <cmath>
#include <vector>
#include <algorithm> 

double M_PI = 3.14159265358979323846;

inline double clamp(double value, double min, double max)
{
    return (value < min) ? min : (value > max) ? max : value;
}

FlangerEffect::FlangerEffect()
    : m_bufferSize(0), m_depth(0.002 * 44100), m_rate(0.25), m_feedback(0.5), m_phase(0.0), m_writeIndex(0)
{
    m_lastOutput[0] = m_lastOutput[1] = 0.0;
}

FlangerEffect::~FlangerEffect()
{
}

void FlangerEffect::Start()
{
    // Initialize buffer size based on depth
    m_bufferSize = static_cast<size_t>(m_depth + 0.05 * m_sampleRate);
    m_delayBuffer[0].resize(m_bufferSize, 0.0);
    m_delayBuffer[1].resize(m_bufferSize, 0.0);
    m_writeIndex = 0;
    m_phase = 0.0;
}

void FlangerEffect::SetDepth(double depth)
{
    m_depth = depth * m_sampleRate; // Depth in samples
}

void FlangerEffect::SetRate(double rate)
{
    m_rate = rate;
}

void FlangerEffect::SetFeedback(double feedback)
{
    // Clamping feedback to prevent excessive buildup
    m_feedback = clamp(feedback, -0.7, 0.7);
}

void FlangerEffect::Process(double* input, double* output)
{
    for (int c = 0; c < 2; ++c)
    {
        // Write input sample + feedback to delay buffer at write index
        double bufferInput = input[c] + m_lastOutput[c] * m_feedback;
        bufferInput = clamp(bufferInput, -1.0, 1.0);
        m_delayBuffer[c][m_writeIndex] = bufferInput;

        // Calculate modulated delay time with a lower depth
        double modulatedDelay = m_depth * (1.0 + sin(2 * M_PI * m_phase)) / 2.0;
        size_t intDelay = static_cast<size_t>(modulatedDelay);
        double fracDelay = modulatedDelay - intDelay;

        // Calculate read index for delay, wrapping around the buffer
        size_t readIndex = (m_writeIndex + m_bufferSize - intDelay) % m_bufferSize;
        size_t nextReadIndex = (readIndex + 1) % m_bufferSize;

        // Interpolate between samples for a smooth delayed sample
        double delayedSample = (1.0 - fracDelay) * m_delayBuffer[c][readIndex] +
            fracDelay * m_delayBuffer[c][nextReadIndex];

        // Combine dry and wet signals with a lower wet mix to avoid clipping
        output[c] = (dryLevel * input[c] + wetLevel * delayedSample) * 0.8;

        // Store the last output for feedback
        m_lastOutput[c] = delayedSample;
    }

    // Advance write index and phase
    m_writeIndex = (m_writeIndex + 1) % m_bufferSize;
    m_phase += m_rate / m_sampleRate;
    if (m_phase >= 1.0)
    {
        m_phase -= 1.0;
    }
}