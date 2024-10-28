#include "stdafx.h"
#include "CPianoInstrument.h"

CPianoInstrument::CPianoInstrument(int maxVoices)
    : CInstrument(), m_polyphony(maxVoices), m_envelope(0.05, 0.05), m_currentNote(nullptr)
{
}

CPianoInstrument::~CPianoInstrument() {
}

void CPianoInstrument::LoadSample(const std::wstring& filename, int midiNote) {
    Sample sample;
    if (sample.Load(filename)) {
        m_samples[midiNote] = sample;
    }
}

void CPianoInstrument::StartNote(int midiNote, double velocity) {
    if (m_samples.find(midiNote) != m_samples.end()) {
        m_envelope.Start(1.0);
        m_polyphony.AddVoice(m_samples[midiNote], velocity, 1.0);
    }
}

void CPianoInstrument::StopNote(int midiNote) {
    m_polyphony.StopVoice(midiNote);
}

void CPianoInstrument::SetPedal(bool pressed) {
    if (pressed) {
        m_pedal.Press();
        m_pedal.GetNoise(true).Start();
    }
    else {
        m_pedal.Release();
        m_pedal.GetNoise(false).Start();
    }
}

void CPianoInstrument::Generate(double* frame, int channels) {
    double time = 0;
    for (int i = 0; i < channels; ++i) {
        frame[i] = 0;
    }

    m_polyphony.Generate(frame, channels);

    for (int i = 0; i < channels; ++i) {
        frame[i] = m_dynamics.ApplyDynamics(frame[i]);
    }

    double envelopeValue = m_envelope.Apply(time);
    for (int i = 0; i < channels; ++i) {
        frame[i] *= envelopeValue;
    }

    if (m_pedal.IsPressed()) {
        double noiseFrame[2] = { 0.0, 0.0 };
        m_pedal.GenerateNoiseFrame(noiseFrame, true, channels);
        for (int i = 0; i < channels; ++i) {
            frame[i] += noiseFrame[i];
        }
    }
}

void CPianoInstrument::SetDynamicRange(double minLevel, double maxLevel) {
    m_dynamics.SetDynamicRange(minLevel, maxLevel);
}

void CPianoInstrument::SetEnvelope(double attack, double release) {
    m_envelope = Envelope(attack, release);
}

void CPianoInstrument::SetNote(CNote* note) {
    m_currentNote = note;
    if (m_currentNote) {
        StartNote(m_currentNote->GetPitch(), m_currentNote->GetVelocity());
    }
}

void CPianoInstrument::Start() {
    if (m_currentNote) {
        StartNote(m_currentNote->GetPitch(), m_currentNote->GetVelocity());
    }
}

bool CPianoInstrument::Generate() {
    double frame[2] = { 0.0, 0.0 };
    Generate(frame, 2);

    m_time += 1.0 / 44100.0; 
    return m_envelope.IsActive(m_time);
}