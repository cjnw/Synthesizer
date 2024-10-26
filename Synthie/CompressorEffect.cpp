#include "stdafx.h"
#include "CompressorEffect.h"
#include <cmath>

CompressorEffect::CompressorEffect()
    : m_threshold(0.4), m_ratio(6.0), m_attackTime(0.002), m_releaseTime(0.3), m_gain(1.0)
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
    m_gain = 1.0;
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

    // Calculate the amount of gain reduction
    double gainReduction = 1.0;
    if (inputLevel > m_threshold)
    {
        // Apply compression
        double excess = inputLevel - m_threshold;
        double compressedExcess = excess / m_ratio;
        double targetLevel = m_threshold + compressedExcess;
        gainReduction = targetLevel / inputLevel;
    }

    // Smooth gain changes
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

    // Apply gain reduction to input signal
    output[0] = input[0] * m_gain;
    output[1] = input[1] * m_gain;
}