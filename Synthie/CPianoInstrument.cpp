#include "stdafx.h"
#include "CPianoInstrument.h"
CPianoInstrument::CPianoInstrument()
{
    m_duration = 1.0;
    m_isLoud = false;
    m_pedalPressed = false;
}

CPianoInstrument::~CPianoInstrument()
{
}

void CPianoInstrument::Start()
{
    
}

bool CPianoInstrument::Generate()
{
   
    return m_duration > 0;
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
        
    }
}
void CPianoInstrument::SetNote(CNote* note)
{
    
}