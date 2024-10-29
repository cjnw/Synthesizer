#include "stdafx.h"
#include "AdditiveSynthesizer.h"


AdditiveSynthesizer::AdditiveSynthesizer(double fundamentalFrequency, const std::vector<float>& harmonics)
    : m_sampleRate(44100), m_playing(false) {
    int harmonicIndex = 1;  // Start at the first harmonic (fundamental is 1)
    for (float amplitude : harmonics) {
        CSineWave sineWave;
        sineWave.SetAmplitude(amplitude);
        sineWave.SetFreq(fundamentalFrequency * harmonicIndex);
        m_harmonics.push_back(sineWave);
        harmonicIndex++;
    }
}

void AdditiveSynthesizer::Play(double duration) {
    m_playing = true;
}

void AdditiveSynthesizer::Stop() {
    m_playing = false;
}

void AdditiveSynthesizer::Generate(double* frame, int channels) {
    if (!m_playing) {
        for (int i = 0; i < channels; ++i) {
            frame[i] = 0.0;
        }
        return;
    }

    for (CSineWave& sineWave : m_harmonics) {
        double sampleValue = sineWave.Generate();
        for (int i = 0; i < channels; ++i) {
            frame[i] += sampleValue;
        }
    }
}