#ifndef ADDITIVESYNTHESIZER_H
#define ADDITIVESYNTHESIZER_H
#pragma once
#include "Polyphony.h"
#include "Sample.h"
#include "SineWave.h"
#include <vector>
#include <utility> // for std::pair

class AdditiveSynthesizer {
public:
    AdditiveSynthesizer(double fundamentalFrequency, const std::vector<float>& harmonics);
    void Play(double duration);
    void Stop();
    void Generate(double* frame, int channels);

private:
    std::vector<CSineWave> m_harmonics;
    double m_sampleRate;
    bool m_playing;
};

#endif