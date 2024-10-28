#pragma once
#include <vector>
#include "Sample.h"

class Polyphony {
public:
    Polyphony(int maxVoices);
    void AddVoice(const Sample& sample, double velocity, double duration);
    void StopVoice(int index);
    void Generate(double* frame, int channels);

private:
    struct Voice {
        Sample sample;
        double velocity;
        double duration;
        bool active;
    };

    std::vector<Voice> m_voices;
    int m_maxVoices;
};
