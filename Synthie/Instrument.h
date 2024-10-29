#pragma once
#include "AudioNode.h"
#include "Note.h"
class CInstrument :
	public CAudioNode
{
public:
	CInstrument();
	CInstrument(double);
	virtual ~CInstrument();
	virtual void SetNote(CNote *note) = 0;

	void SetEffectsManager(std::shared_ptr<EffectsManager> manager) { m_effectsManager = EffectsManager(*manager); }
    double Send(int i) {return m_effectsManager.WetLevel(i);}
	EffectsManager* GetEffectsManager() { return &m_effectsManager; }


private:
	EffectsManager m_effectsManager;

};

