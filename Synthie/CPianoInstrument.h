#pragma once
#include "Instrument.h"
#include "Sample.h"
#include <map>
#include <vector>

class CPianoInstrument : public CInstrument
{
public:
    CPianoInstrument();
    CPianoInstrument(int maxVoices);
    virtual ~CPianoInstrument();

    virtual void Start() override;
    virtual bool Generate() override;
    virtual void SetNote(CNote* note) override;

    void SetPedal(bool pressed);
    void LoadSample(const std::wstring& filename, int midiNote);

private:
    struct Voice {
        Sample sample;
        double velocity;
        double duration;
        bool active;
    };

    // Piano-related attributes
    std::map<int, Sample> m_samples;  // MIDI note to Sample mapping
    std::vector<Voice> m_voices;      // Active voices for polyphony
    int m_maxVoices;

    // Envelope and dynamics
    double m_attack;
    double m_release;
    double m_time;

    // Pedal and effects
    bool m_isPedalPressed;
    Sample m_pedalDownNoise;
    Sample m_pedalUpNoise;

    // Frame buffer for stereo output
    double m_frame[2];

    // Pointer to the current note
    CNote* m_currentNote;

    // Internal helper methods
    void StartNote(int midiNote, double velocity);
    void AddVoice(Sample& sample, double velocity, double duration);
    double ApplyDynamics(double input, double velocity);
    double ApplyEnvelope(double time);
    bool IsEnvelopeActive(double time) const;
    void GeneratePedalNoise(bool isPressing);
};
