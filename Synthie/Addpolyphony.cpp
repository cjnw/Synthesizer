#include "stdafx.h"
#include "Addpolyphony.h"

Addpolyphony::Addpolyphony(int maxVoices) : m_maxVoices(maxVoices) {
    m_voices.resize(m_maxVoices);
}

void Addpolyphony::AddVoice(const AdditiveSynthesizer& synth, double velocity, double duration) {
    for (auto& voice : m_voices) {
        if (!voice.active) {
            voice.synth = std::make_unique<AdditiveSynthesizer>(synth);
            voice.velocity = velocity;
            voice.duration = duration;
            voice.active = true;
            voice.synth->Play(duration);
            break;
        }
    }
}

void Addpolyphony::StopVoice(int index) {
    if (index >= 0 && index < m_voices.size()) {
        m_voices[index].active = false;
    }
}

void Addpolyphony::Generate(double* frame, int channels) {
    for (int i = 0; i < channels; i++) {
        frame[i] = 0.0;  // Initialize the frame
    }

    for (auto& voice : m_voices) {
        if (voice.active) {
            double tempFrame[2] = { 0.0, 0.0 };  // Temporary frame for each voice

            voice.synth->Generate(tempFrame, channels);  // Generate frame for this voice

            // Apply velocity and add to the main frame
            for (int i = 0; i < channels; ++i) {
                frame[i] += tempFrame[i] * voice.velocity;
            }

            voice.duration -= 1.0 / 44100.0;  // Decrement duration
            if (voice.duration <= 0) {
                voice.active = false;
                voice.synth->Stop();  // Stop the synth if duration is done
            }
        }
    }
}