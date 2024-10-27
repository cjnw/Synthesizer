#pragma once
#include "Instrument.h"
#include <map>
#include <string>
#include <vector>


class CPianoInstrument :
    public CInstrument
{
public:
    CPianoInstrument();
    virtual ~CPianoInstrument();

    
    virtual void Start() override;
    virtual bool Generate() override;

    
    void SetNoteDuration(double duration);
    void SetDynamics(bool loud);
    void SetPedal(bool pressed);
    virtual void SetNote(CNote* note) override;

private:
   
    void LoadSample(const std::string& filePath, int midiNote);
    void ReleaseNote(CNote* note);
    void HandlePedalNoise();

   
    double m_duration;
    bool m_isLoud;
    bool m_pedalPressed;

    
    std::map<int, std::vector<short>> m_samples;
    std::vector<CNote*> m_activeNotes; // For polyphony
    double m_releaseTime; // For envelope
};

