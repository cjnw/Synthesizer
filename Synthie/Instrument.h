#pragma once
#include "AudioNode.h"
#include "Note.h"
const int NUMEFFECTSCHANNELS = 4;
class CInstrument :
	public CAudioNode
{
public:
	CInstrument();
	CInstrument(double);
	virtual ~CInstrument();
	virtual void SetNote(CNote *note) = 0;
    double Send(int i) {return sends[i];}

private:
    double sends[NUMEFFECTSCHANNELS];
};

