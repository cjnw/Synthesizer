#include "stdafx.h"
#include "CPianoInstrument.h"
CPianoInstrument::CPianoInstrument()
{
    m_duration = 1.0;
    m_isLoud = false;
    m_pedalPressed = false;
    m_releaseTime = 0.5;
}

CPianoInstrument::~CPianoInstrument()
{
}

void CPianoInstrument::Start()
{
    
}

bool CPianoInstrument::Generate()
{
   
    return !m_activeNotes.empty();
}

void CPianoInstrument::SetNoteDuration(double duration)
{
    m_duration = duration;
}

void CPianoInstrument::SetDynamics(bool loud)
{
    m_isLoud = loud;
}

void CPianoInstrument::SetPedal(bool pressed)
{
    if (pressed != m_pedalPressed) {
        m_pedalPressed = pressed;
        HandlePedalNoise();
        
    }
}
void CPianoInstrument::SetNote(CNote* note)
{
    m_activeNotes.push_back(note);
    
}

void CPianoInstrument::ReleaseNote(CNote* note)
{
    
}

void CPianoInstrument::HandlePedalNoise()
{
    if (m_pedalPressed) {
        
    }
    else {
        
    }
}