#include "stdafx.h"
#include "EchoEffect.h"

EchoEffect::EchoEffect()
    : m_writeIndex(0), m_delaySamples(0), m_wetLevel(0.5), m_dryLevel(0.5)
{
}

EchoEffect::~EchoEffect()
{
}

void EchoEffect::Start()
{
    m_delayBuffer[0].resize(m_delaySamples, 0.0);
    m_delayBuffer[1].resize(m_delaySamples, 0.0);
    m_writeIndex = 0;
}

void EchoEffect::SetDelay(double delayTime)
{
    m_delaySamples = static_cast<size_t>(delayTime * m_sampleRate);
}

void EchoEffect::SetWet(double wetLevel)
{
    m_wetLevel = wetLevel;
}

void EchoEffect::SetDry(double dryLevel)
{
    m_dryLevel = dryLevel;
}

void EchoEffect::Process(double* input, double* output)
{
    for (int c = 0; c < 2; ++c)
    {
        size_t readIndex = (m_writeIndex + m_delayBuffer[c].size() - m_delaySamples) % m_delayBuffer[c].size();
        double delayedSample = m_delayBuffer[c][readIndex];

        // Combine dry and wet signals
        output[c] = m_dryLevel * input[c] + m_wetLevel * delayedSample;

        // Write current sample to delay buffer
        m_delayBuffer[c][m_writeIndex] = input[c];
    }
    m_writeIndex = (m_writeIndex + 1) % m_delayBuffer[0].size();
}