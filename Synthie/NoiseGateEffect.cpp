#include "stdafx.h"
#include "NoiseGateEffect.h"
#include <cmath>

NoiseGateEffect::NoiseGateEffect()
    : m_threshold(0.05), m_attackTime(0.005), m_releaseTime(0.2), m_gain(1.0)
{
}

NoiseGateEffect::~NoiseGateEffect()
{
}

void NoiseGateEffect::Start()
{
    // Calculate coefficients based on attack and release times
    m_attackCoef = exp(-1.0 / (m_attackTime * m_sampleRate));
    m_releaseCoef = exp(-1.0 / (m_releaseTime * m_sampleRate));
    m_gain = 1.0;
}

void NoiseGateEffect::SetThreshold(double threshold)
{
    m_threshold = threshold;
}

void NoiseGateEffect::SetAttackTime(double attackTime)
{
    m_attackTime = attackTime;
}

void NoiseGateEffect::SetReleaseTime(double releaseTime)
{
    m_releaseTime = releaseTime;
}

void NoiseGateEffect::Process(double* input, double* output)
{
    // Calculate the input level (RMS of both channels)
    double inputLevel = sqrt(0.5 * (input[0] * input[0] + input[1] * input[1]));

    // Determine if we should open or close the gate
    if (inputLevel > m_threshold)
    {
        // Signal is above threshold, open the gate (gain towards 1)
        m_gain = m_gain * m_attackCoef + (1.0 - m_attackCoef);
    }
    else
    {
        // Signal is below threshold, close the gate (gain towards 0)
        m_gain = m_gain * m_releaseCoef;
    }

    // Apply gain to input signal
    output[0] = input[0] * m_gain;
    output[1] = input[1] * m_gain;
}