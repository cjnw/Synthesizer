#pragma once

class Effect
{
public:
    Effect() : m_sampleRate(44100.0), wetLevel(0.5), dryLevel(0.5) {}
    virtual ~Effect() {}

    void SetWet(double level) { wetLevel = level; }
    void SetDry(double level) { dryLevel = level; }


    virtual void Start() {}
    virtual void Process(double* input, double* output) = 0;

    void SetSampleRate(double sampleRate) { m_sampleRate = sampleRate; }

protected:
    double m_sampleRate;

    double wetLevel;
    double dryLevel;

};