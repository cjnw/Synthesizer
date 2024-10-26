#include "stdafx.h"
#include "EffectsManager.h"
#include <string>


EffectsManager::EffectsManager(int channels){

	sends.resize(channels, 0.0); 
	sends[0] = 1;
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
	double sendValue = 0.0;

	// Loop over the list of attributes
	for (int i = 0; i < len; i++)
	{
		// Get attribute i
		CComPtr<IXMLDOMNode> attrib;
		attributes->get_item(i, &attrib);

		// Get the name of the attribute
		CComBSTR name;
		attrib->get_nodeName(&name);

		// Get the value of the attribute.  
		CComVariant value;
		attrib->get_nodeValue(&value);

		if (name == L"which") effectName = value.bstrVal;
		if (name == L"send") {
			value.ChangeType(VT_R8);
			sendValue = value.dblVal;
		}


	}
	int effectIndex = GetEffectIndexFromName(effectName);
	SetSend(effectIndex, sendValue);

}