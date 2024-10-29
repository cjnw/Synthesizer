#include "stdafx.h"
#include "EffectsManager.h"
#include <string>


EffectsManager::EffectsManager(int channels) {
	wetLevels.resize(channels, 0.0);
	dryLevels.resize(channels, 0.0);
	wetLevels[0] = 0;  // Default wet level for primary output channel
	dryLevels[0] = 1;  // Default dry level for primary output channel
}

int GetEffectIndexFromName(std::wstring name) {
	if (name == L"noise-gate") {
		return 1;
	}
	else if (name == L"compressor") {
		return 2;
	}
	else if (name == L"chorus") {
		return 3;
	}
	else if (name == L"flanger") {
		return 4;
	}

	return -1;
}



void EffectsManager::addEffectXML(IXMLDOMNode* xml) {
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    xml->get_attributes(&attributes);
    long len;
    attributes->get_length(&len);

    std::wstring effectName = L"";
    double wetLevel = 1.0;
    double dryLevel = 1.0;

    for (int i = 0; i < len; i++) {
        CComPtr<IXMLDOMNode> attrib;
        attributes->get_item(i, &attrib);

        CComBSTR name;
        attrib->get_nodeName(&name);

        CComVariant value;
        attrib->get_nodeValue(&value);

        if (name == L"which") effectName = value.bstrVal;
        else if (name == L"wet") {
            value.ChangeType(VT_R8);
            wetLevel = value.dblVal;
        }
        else if (name == L"dry") {
            value.ChangeType(VT_R8);
            dryLevel = value.dblVal;
        }
    }
    int effectIndex = GetEffectIndexFromName(effectName);
    SetWetLevel(effectIndex, wetLevel);
    SetDryLevel(effectIndex, dryLevel);
}