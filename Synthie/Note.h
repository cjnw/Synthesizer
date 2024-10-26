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
	bool operator<(const CNote &b);
private:
	std::wstring m_instrument;
	int m_measure;
	double m_beat;
    double m_dryLevel;
    double m_wetLevel;
	CComPtr<IXMLDOMNode> m_node;
	std::shared_ptr<EffectsManager> m_effectsManager;


};

