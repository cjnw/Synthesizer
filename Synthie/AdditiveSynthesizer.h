#ifndef ADDITIVESYNTHESIZER_H
#define ADDITIVESYNTHESIZER_H
#pragma once
#include "Polyphony.h"
#include "Sample.h"
#include "SineWave.h"
#include "ToneInstrument.h"
#include "Notes.h"
#include <vector>
#include <utility> // for std::pair
#include "Instrument.h"
#include <SineWave.h>
#include <AR.h>


class AdditiveSynthesizer : CToneInstrument {
public:
    void AddNote(CNote* c);

private:
	std::vector<CSineWave> m_sinewaves;
	CSineWave m_sinewave;
	CAR m_ar;
	double m_frequency;
	double m_duration;
	std::wstring m_drawbars;
	double m_time;
	double m_vibratoDepth;
	double m_vibratoPhaseIncrement;
	double m_vibratoRate;
	double m_vibratoPhase;
};

#endif