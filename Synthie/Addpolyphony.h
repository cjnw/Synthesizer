#pragma once
#include "AdditiveSynthesizer.h"
#include <memory>

class Addpolyphony {
public:
    Addpolyphony(int maxVoices);
    void AddVoice(const AdditiveSynthesizer& synth, double velocity, double duration);
    void StopVoice(int index);
    void Generate(double* frame, int channels);

private:
    struct Voice {
        std::unique_ptr<AdditiveSynthesizer> synth;
        double velocity;
        double duration;
        bool active;
    };

    std::vector<Voice> m_voices;
    int m_maxVoices;
};

