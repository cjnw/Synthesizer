#pragma once
#include <string>
#include <memory>
#include "EffectsManager.h"
class CNote
{
public:
	CNote();
	virtual ~CNote();
	int Measure() const { return m_measure; }
	double Beat() const { return m_beat; }
	const std::wstring &Instrument() const { return m_instrument; }
	IXMLDOMNode *Node() { return m_node; }
	void XmlLoad(IXMLDOMNode * xml, std::wstring & instrument);
	void SetEffectsManager(std::shared_ptr<EffectsManager> manager) { m_effectsManager = manager; }
	std::shared_ptr<EffectsManager> GetEffectsManager() {
		return m_effectsManager;
	}

	double getWet() { return m_wetLevel; }

	int GetPitch() const { return m_pitch; }
	void SetPitch(int pitch) { m_pitch = pitch; }

	double GetVelocity() const { return m_velocity; }
	void SetVelocity(double velocity) { m_velocity = velocity; }

	double GetGain() { return m_gain; }

	bool operator<(const CNote &b);
private:
	std::wstring m_instrument;
	int m_measure;
	double m_beat;
    double m_dryLevel;
    double m_wetLevel;
	int m_pitch;      
	double m_gain;
	double m_velocity;
	CComPtr<IXMLDOMNode> m_node;
	std::shared_ptr<EffectsManager> m_effectsManager;


};

