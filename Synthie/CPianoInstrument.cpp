#include "stdafx.h"
#include "CPianoInstrument.h"
#include "Note.h"

CPianoInstrument::CPianoInstrument()
    : m_maxVoices(16), m_attack(0.05), m_release(0.05), m_time(0), m_isPedalPressed(false)
{
    m_voices.resize(m_maxVoices);
    m_frame[0] = m_frame[1] = 0.0;
}

CPianoInstrument::CPianoInstrument(int maxVoices)
    : m_maxVoices(maxVoices), m_attack(0.05), m_release(0.05), m_time(0), m_isPedalPressed(false)
{
    m_voices.resize(m_maxVoices);
    m_frame[0] = m_frame[1] = 0.0;
}

CPianoInstrument::~CPianoInstrument() {}

void CPianoInstrument::LoadSample(const std::wstring& filename, int midiNote) {
    if (midiNote >= 21 && midiNote <= 108) {
        Sample sample;
        if (sample.Load(filename)) {
            m_samples[midiNote] = sample;
        }
    }
}

void CPianoInstrument::Start() {
    m_time = 0;
    m_frame[0] = m_frame[1] = 0.0;
    if (m_currentNote) {
        StartNote(m_currentNote->GetPitch(), m_currentNote->GetVelocity());
    }
}

bool CPianoInstrument::Generate() {
    // Reset m_frame for stereo output
    m_frame[0] = 0.0;
    m_frame[1] = 0.0;

    // Generate samples for all active voices
    for (auto& voice : m_voices) {
        if (voice.active) {
            double sampleValue = voice.sample.Generate();
            m_frame[0] += ApplyDynamics(sampleValue, voice.velocity);
            m_frame[1] += ApplyDynamics(sampleValue, voice.velocity);
            voice.duration -= m_samplePeriod;
            if (voice.duration <= 0) voice.active = false;
        }
    }

    // Apply envelope to the frame
    double envelopeValue = ApplyEnvelope(m_time);
    m_frame[0] *= envelopeValue;
    m_frame[1] *= envelopeValue;

    // Apply pedal noise if pressed
    if (m_isPedalPressed) GeneratePedalNoise(true);

    // Update time for envelope progression
    m_time += m_samplePeriod;
    return IsEnvelopeActive(m_time);
}

void CPianoInstrument::SetNote(CNote* note) {
    m_currentNote = note;
    if (m_currentNote) {
        StartNote(m_currentNote->GetPitch(), m_currentNote->GetVelocity());
    }
}

void CPianoInstrument::SetPedal(bool pressed) {
    m_isPedalPressed = pressed;
    GeneratePedalNoise(pressed);
}

void CPianoInstrument::StartNote(int midiNote, double velocity) {
    // Check if the MIDI note is mapped to a sample
    if (m_samples.find(midiNote) != m_samples.end()) {
        Sample& sample = m_samples[midiNote];
        AddVoice(sample, velocity, 1.0);  // Adding the note with a default duration of 1.0 seconds
    }
}

void CPianoInstrument::AddVoice(Sample& sample, double velocity, double duration) {
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

double CPianoInstrument::ApplyDynamics(double input, double velocity) {
    double minLevel = 0.0, maxLevel = 1.0;
    return (input * velocity < minLevel) ? minLevel : (input * velocity > maxLevel) ? maxLevel : input * velocity;
}

double CPianoInstrument::ApplyEnvelope(double time) {
    if (time < m_attack) return time / m_attack;
    else if (time > 1.0 - m_release) return (1.0 - time) / m_release;
    return 1.0;
}

bool CPianoInstrument::IsEnvelopeActive(double time) const {
    return time < 1.0;
}

void CPianoInstrument::GeneratePedalNoise(bool isPressing) {
    double noise = isPressing ? m_pedalDownNoise.Generate() : m_pedalUpNoise.Generate();
    m_frame[0] += noise;
    m_frame[1] += noise;
}
