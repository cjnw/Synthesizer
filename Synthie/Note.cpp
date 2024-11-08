#include "stdafx.h"
#include "Note.h"
#include "xmlhelp.h"


CNote::CNote()
{
	m_gain = 1;
}


CNote::~CNote()
{
}
void CNote::XmlLoad(IXMLDOMNode * xml, std::wstring & instrument)
{
	std::shared_ptr<EffectsManager> effectManager = std::make_shared<EffectsManager>(5);
	this->SetEffectsManager(effectManager);
	// Remember the xml node and the instrument.
	m_node = xml;
	m_instrument = instrument;

	// Get a list of all attribute nodes and the
	// length of that list
	CComPtr<IXMLDOMNamedNodeMap> attributes;
	xml->get_attributes(&attributes);
	long len;
	attributes->get_length(&len);

	// Loop over the list of attributes
	for (int i = 0; i<len; i++)
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

		if (name == "measure")
		{
			// The file has measures that start at 1.  
			// We'll make them start at zero instead.
			value.ChangeType(VT_I4);
			m_measure = value.intVal - 1;
		}
		else if (name == "beat")
		{
			// Same thing for the beats.
			value.ChangeType(VT_R8);
			m_beat = value.dblVal - 1;
		}
		else  if (name == "gain")
		{
			value.ChangeType(VT_R8);
			m_gain = value.dblVal;
		}
	}



	CComPtr<IXMLDOMNode> node;
	xml->get_firstChild(&node);
	for (; node != NULL; NextNode(node))
	{
		// Get the name of the node
		CComBSTR name;
		node->get_nodeName(&name);

		if (name == L"effect")
		{
			effectManager->addEffectXML(node);
		}

	}

	effectManager->SetWetLevel(0, m_gain);
	effectManager->SetDryLevel(0, m_gain);


}
bool CNote::operator<(const CNote &b)
{
	if (m_measure < b.m_measure)
		return true;
	if (m_measure > b.m_measure)
		return false;
	if (m_beat < b.m_beat)
		return true;

	return false;
}