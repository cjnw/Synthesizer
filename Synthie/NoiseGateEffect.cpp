#include "stdafx.h"
#include "NoiseGateEffect.h"
#include <cmath>

NoiseGateEffect::NoiseGateEffect()
    : m_threshold(0.05),         // Higher threshold to gate more sound
    m_attackTime(0.01),        // Fast attack to open gate quickly
    m_releaseTime(0.3),        // Longer release for more sustained gating effect
    m_gain(0.0)                // Start with the gate closed
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
    m_gain = 0.0; // Ensure gate starts fully closed
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

    // Determine if we should open or close the gate based on the threshold
    if (inputLevel > m_threshold)
    {
        // Signal is above threshold, open the gate (gain approaches 1)
        m_gain = m_gain * m_attackCoef + (1.0 - m_attackCoef);
    }
    else
    {
        // Signal is below threshold, close the gate (gain approaches 0)
        m_gain = m_gain * m_releaseCoef;
    }

    // Apply gain to input signal with wet/dry mix and makeup gain
    double makeupGain = 1.5; // Increase for more prominent gating
    output[0] = input[0] * (m_gain * wetLevel * makeupGain) + (dryLevel * input[0]);
    output[1] = input[1] * (m_gain * wetLevel * makeupGain) + (dryLevel * input[1]);
}