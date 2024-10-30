#include "stdafx.h"
#include "CompressorEffect.h"
#include <cmath>

CompressorEffect::CompressorEffect()
    : m_threshold(0.2),       // Threshold at which compression starts
    m_ratio(10),           // Compression ratio
    m_attackTime(0.002),    // Fast attack for quick response to loud sounds
    m_releaseTime(0.3),     // Moderate release for natural decay
    m_gain(1.5)
{
}

CompressorEffect::~CompressorEffect()
{
}

void CompressorEffect::Start()
{
    // Calculate coefficients based on attack and release times
    m_attackCoef = exp(-1.0 / (m_attackTime * m_sampleRate));
    m_releaseCoef = exp(-1.0 / (m_releaseTime * m_sampleRate));
  //  m_gain = 1.0; // Start with no gain reduction
}

void CompressorEffect::SetThreshold(double threshold)
{
    m_threshold = threshold;
}

void CompressorEffect::SetRatio(double ratio)
{
    m_ratio = ratio;
}

void CompressorEffect::SetAttackTime(double attackTime)
{
    m_attackTime = attackTime;
}

void CompressorEffect::SetReleaseTime(double releaseTime)
{
    m_releaseTime = releaseTime;
}

void CompressorEffect::Process(double* input, double* output)
{
    // Calculate the input level (RMS of both channels)
    double inputLevel = sqrt(0.5 * (input[0] * input[0] + input[1] * input[1]));

    // Calculate gain reduction only if inputLevel exceeds threshold
    double gainReduction = 1.0;
    if (inputLevel > m_threshold)
    {
        // Adjusted compression calculation for stronger reduction
        double excess = inputLevel - m_threshold;
        double compressedExcess = excess * (1.0 - 1.0 / m_ratio); // More aggressive compression
        double targetLevel = m_threshold + compressedExcess;
        gainReduction = targetLevel / (inputLevel + 1e-6); // Add epsilon to avoid division by zero
    }

    // Smooth gain changes with attack and release coefficients
    if (gainReduction < m_gain)
    {
        // Attack (gain reducing)
        m_gain = m_gain * m_attackCoef + gainReduction * (1.0 - m_attackCoef);
    }
    else
    {
        // Release (gain increasing)
        m_gain = m_gain * m_releaseCoef + gainReduction * (1.0 - m_releaseCoef);
    }

    // Apply smoothed gain reduction with wet/dry mix and a makeup gain factor
    double makeupGain = 1.5; // Increase for more prominent compression effect
    output[0] = input[0] * (m_gain * wetLevel * makeupGain) + (input[0] * dryLevel);
    output[1] = input[1] * (m_gain * wetLevel * makeupGain) + (input[1] * dryLevel);
}