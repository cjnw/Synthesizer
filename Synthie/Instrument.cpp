#include "stdafx.h"
#include "Instrument.h"


CInstrument::CInstrument()
{
	m_wet = 0;
}

// Constructor to set the beats per minute
CInstrument::CInstrument(double bpm)
{
	m_bpm = bpm;
}

CInstrument::~CInstrument()
{
}
