#pragma once

#include <vector>
#include <map>
#include "Sample.h"
#include "Dynamics.h"
#include "Envelope.h"
#include "Pedal.h"
#include "Polyphony.h"
#include "Instrument.h"

class CPianoInstrument : public CInstrument {
public:
    CPianoInstrument(int maxVoices);
    ~CPianoInstrument();

    void LoadSample(const std::wstring& filename, int midiNote);
    void StartNote(int midiNote, double velocity);
    void StopNote(int midiNote);
    void SetPedal(bool pressed);
    void Generate(double* frame, int channels);
    void SetDynamicRange(double minLevel, double maxLevel);
    void SetEnvelope(double attack, double release);
    void SetNote(CNote* note) override;
    void Start() override;
    bool Generate() override;

private:
    std::map<int, Sample> m_samples;
    Polyphony m_polyphony;
    Pedal m_pedal;
    Dynamics m_dynamics;
    Envelope m_envelope;
    CNote* m_currentNote;
};