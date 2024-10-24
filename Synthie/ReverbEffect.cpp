#include "stdafx.h"
#include "ReverbEffect.h"
#include <cmath>

ReverbEffect::ReverbEffect()
    : m_roomSize(0.5), m_damping(0.5), m_wetLevel(0.3), m_dryLevel(0.7), m_allpassFeedback(0.5)
{
    // Initialize indices and feedback
    for (int i = 0; i < 4; ++i)
    {
        m_combFeedback[i] = 0.7; // Feedback for comb filters
    }
    // Initialize indices
    for (int c = 0; c < 2; ++c)
    {
        for (int i = 0; i < 4; ++i)
        {
            m_combIndices[c][i] = 0;
        }
        for (int i = 0; i < 2; ++i)
        {
            m_allpassIndices[c][i] = 0;
        }
    }
}

ReverbEffect::~ReverbEffect()
{
}

void ReverbEffect::SetRoomSize(double size)
{
    m_roomSize = size;
}

void ReverbEffect::SetDamping(double damping)
{
    m_damping = damping;
}

void ReverbEffect::SetWet(double wetLevel)
{
    m_wetLevel = wetLevel;
}

void ReverbEffect::SetDry(double dryLevel)
{
    m_dryLevel = dryLevel;
}

void ReverbEffect::Start()
{
    InitializeDelays();
}

void ReverbEffect::InitializeDelays()
{
    // Comb filter delay lengths (in samples)
    size_t combDelayLengths[4] = {
        static_cast<size_t>(m_sampleRate * 0.0297 * m_roomSize),
        static_cast<size_t>(m_sampleRate * 0.0371 * m_roomSize),
        static_cast<size_t>(m_sampleRate * 0.0411 * m_roomSize),
        static_cast<size_t>(m_sampleRate * 0.0437 * m_roomSize)
    };

    // All-pass filter delay lengths (in samples)
    size_t allpassDelayLengths[2] = {
        static_cast<size_t>(m_sampleRate * 0.005 * m_roomSize),
        static_cast<size_t>(m_sampleRate * 0.0017 * m_roomSize)
    };

    for (int c = 0; c < 2; ++c)
    {
        // Initialize comb filters
        for (int i = 0; i < 4; ++i)
        {
            m_combDelayBuffer[c][i].resize(combDelayLengths[i], 0.0);
            m_combIndices[c][i] = 0;
        }
        // Initialize all-pass filters
        for (int i = 0; i < 2; ++i)
        {
            m_allpassDelayBuffer[c][i].resize(allpassDelayLengths[i], 0.0);
            m_allpassIndices[c][i] = 0;
        }
    }
}

void ReverbEffect::Process(double* input, double* output)
{
    for (int c = 0; c < 2; ++c)
    {
        double in = input[c];

        // --- Comb Filters ---
        double combOut = 0.0;
        for (int i = 0; i < 4; ++i)
        {
            std::vector<double>& buffer = m_combDelayBuffer[c][i];
            size_t& index = m_combIndices[c][i];

            // Ensure buffer is not empty
            if (buffer.empty())
            {
                // Handle error or skip processing
                continue;
            }

            double delayedSample = buffer[index];

            // Rest of the comb filter processing...

            // Update index safely
            index = (index + 1) % buffer.size();
        }

        // --- All-pass Filters ---
        double allpassOut = combOut;
        for (int i = 0; i < 2; ++i)
        {
            std::vector<double>& buffer = m_allpassDelayBuffer[c][i];
            size_t& index = m_allpassIndices[c][i];

            // Ensure buffer is not empty
            if (buffer.empty())
            {
                // Handle error or skip processing
                continue;
            }

            double delayedSample = buffer[index];

            // Rest of the all-pass filter processing...

            // Update index safely
            index = (index + 1) % buffer.size();
        }

        // --- Mix Wet and Dry Signals ---
        output[c] = m_dryLevel * in + m_wetLevel * allpassOut;
    }
}