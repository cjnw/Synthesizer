#include "stdafx.h"
#include "Polyphony.h"

#include "Polyphony.h"

Polyphony::Polyphony(int maxVoices) : m_maxVoices(maxVoices) {
    m_voices.resize(m_maxVoices);
}

void Polyphony::AddVoice(const Sample& sample, double velocity, double duration) {
    for (auto& voice : m_voices) {
        if (!voice.active) {
            voice.sample = sample;
            voice.velocity = velocity;
            voice.duration = duration;
            voice.active = true;
            break;
        }
    }
}

void Polyphony::StopVoice(int index) {
    if (index >= 0 && index < m_voices.size()) {
        m_voices[index].active = false;
    }
}

void Polyphony::Generate(double* frame, int channels) {
    for (auto& voice : m_voices) {
        if (voice.active) {
            double sampleValue = voice.sample.Generate();
            for (int i = 0; i < channels; i++) {
                frame[i] += sampleValue;
            }

            voice.duration -= 1.0 / 44100.0;
            if (voice.duration <= 0) {
                voice.active = false;
            }
        }
    }
}
