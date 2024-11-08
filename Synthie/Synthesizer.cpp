#include "stdafx.h"
#include "Synthesizer.h"
#include "Instrument.h"
#include "ToneInstrument.h"
#include "CAdditiveSynthesizer.h"
#include "xmlhelp.h"
#include <vector>
#include <algorithm>
#include "EffectsManager.h"
#include "NoiseGateEffect.h"
#include "CompressorEffect.h"
#include "ChorusEffect.h"
#include "FlangerEffect.h"
#include "CPianoInstrument.h"
#include "CDrums.h"
#include "COrgan.h"


const int NUMEFFECTSCHANNELS = 5;


CSynthesizer::CSynthesizer()
: m_time(0)
{
	CoInitialize(NULL);
	m_channels = 2;
	m_sampleRate = 44100.;
	m_samplePeriod = 1 / m_sampleRate;
	m_bpm = 120;            
	m_beatspermeasure = 4;
	m_secperbeat = 0.5;     

	m_noiseGate = new NoiseGateEffect();
	m_compressor = new CompressorEffect();
	m_chorus = new ChorusEffect();
	m_flanger = new FlangerEffect();
}





CSynthesizer::~CSynthesizer()
{
	// Delete effects
	//delete m_echo;
	//delete m_reverb;
}

//! Start the synthesizer
void CSynthesizer::Start()
{
	m_instruments.clear();
	m_currentNote = 0;
	m_measure = 0;
	m_beat = 0;
	m_time = 0;
	//m_pianoinstfactory.Interpolate(0.34);


	m_flanger->SetSampleRate(GetSampleRate());
	m_flanger->Start();

	m_chorus->SetSampleRate(GetSampleRate());
	m_chorus->Start();


	m_compressor->SetSampleRate(GetSampleRate());
	m_compressor->Start();

	m_noiseGate->SetSampleRate(GetSampleRate());
	m_noiseGate->Start();
}



void CSynthesizer::ApplyEffects(double channelframes[][2], double* outputFrame)
{
	double tempFrame[2] = { 0,0};


	for (int i = 0; i < NUMEFFECTSCHANNELS; i++) {
		double effectOutput[2] = { 0.0, 0.0 };
		if (i == 0) {
			effectOutput[0] = channelframes[0][0];
			effectOutput[1] = channelframes[0][1];
		}
		else if (i == 1) {
			m_noiseGate->Process(channelframes[i], effectOutput);
		}
		else if (i == 2) {
			m_compressor->Process(channelframes[i], effectOutput);
		}
		else if (i == 3) {
			m_chorus->Process(channelframes[i], effectOutput);
		}
		else if (i == 4) {
			m_flanger->Process(channelframes[i], effectOutput);
		}

		tempFrame[0] += effectOutput[0];
		tempFrame[1] += effectOutput[1];

	}

	outputFrame[0] = tempFrame[0];
	outputFrame[1] = tempFrame[1];
}

bool CSynthesizer::Generate(double * frame)
{

	//
	// Phase 1: Determine if any notes need to be played.
	//

	while (m_currentNote < (int)m_notes.size())
	{
		// Get a pointer to the current note
		CNote *note = &m_notes[m_currentNote];

		// If the measure is in the future we can't play
		// this note just yet.
		if (note->Measure() > m_measure)
			break;

		// If this is the current measure, but the
		// beat has not been reached, we can't play
		// this note.
		if (note->Measure() == m_measure && note->Beat() > m_beat)
			break;

		//
		// Play the note!
		//

		// Create the instrument object
		CInstrument *instrument = NULL;
		if (note->Instrument() == L"ToneInstrument")
		{
			instrument = new CToneInstrument(GetBeatsPerMinute());
		}

		else if (note->Instrument() == L"PianoInstrument")
		{
			instrument = new CPianoInstrument();
		}

		else if (note->Instrument() == L"Drums")
		{
			instrument = new CDrums();
		}

		else if (note->Instrument() == L"Organ")
		{
			instrument = new COrgan();
		}
		else if (note->Instrument() == L"Additive")
		{
			instrument = new CAdditiveSynthesizer(600);
		}
		// Configure the instrument object
		if (instrument != NULL)
		{
			instrument->SetSampleRate(GetSampleRate());
			instrument->SetNote(note);
			instrument->SetEffectsManager(note->GetEffectsManager());
			instrument->Start();
			

			m_instruments.push_back(instrument);
		}

		m_currentNote++;
	}
	//
	// Phase 2: Clear all channels to silence 
	

    double channelframes[NUMEFFECTSCHANNELS][2];

    for(int i=0;  i<NUMEFFECTSCHANNELS;  i++)
    {
        for(int c=0;  c<GetNumChannels();  c++)
        {
             channelframes[i][c] = 0;
        }
    }

	//
	// Phase 3: Play an active instruments
	//

	//
	// We have a list of active (playing) instruments.  We iterate over 
	// that list.  For each instrument we call generate, then add the
	// output to our output frame.  If an instrument is done (Generate()
	// returns false), we remove it from the list.
	//

	for (list<CInstrument *>::iterator node = m_instruments.begin(); node != m_instruments.end();)
	{
		// Since we may be removing an item from the list, we need to know in 
		// advance, what is after it in the list.  We keep that node as "next"
		list<CInstrument *>::iterator next = node;
		next++;

		// Get a pointer to the allocated instrument
		CInstrument *instrument = *node;

		// Call the generate function
		if (instrument->Generate())
		{
			EffectsManager* effectsManager = instrument->GetEffectsManager();

			for (int i = 0; i < NUMEFFECTSCHANNELS; i++) {
				double wetLevel = effectsManager->WetLevel(i);
				double dryLevel = effectsManager->DryLevel(i);

				if (wetLevel > 0 || dryLevel > 0) {
					double instrumentFrame[2] = { instrument->Frame(0), instrument->Frame(1) };
					double effectOutput[2] = { dryLevel * instrumentFrame[0], dryLevel * instrumentFrame[1] };

					if (i == 1) {
						m_noiseGate->SetWet(wetLevel);
						m_noiseGate->SetDry(dryLevel);
						m_noiseGate->Process(instrumentFrame, effectOutput);
					}
					else if (i == 2) {
						m_compressor->SetWet(wetLevel);
						m_compressor->SetDry(dryLevel);
						m_compressor->Process(instrumentFrame, effectOutput);
					}
					else if (i == 3) {
						m_chorus->SetWet(wetLevel);
						m_chorus->SetDry(dryLevel);
						m_chorus->Process(instrumentFrame, effectOutput);
					}
					else if (i == 4) {
						m_flanger->SetWet(wetLevel);
						m_flanger->SetDry(dryLevel);
						m_flanger->Process(instrumentFrame, effectOutput);
					}

					for (int c = 0; c < GetNumChannels(); c++)
					{
						 channelframes[i][c] += effectOutput[c];
					}

				}
			}


		}
		else
		{
			// If we returned false, the instrument is done.  Remove it
			// from the list and delete it from memory.
			m_instruments.erase(node);
			delete instrument;
		}

		// Move to the next instrument in the list
		node = next;
	}

	// Phase 3-effect: Apply effects
	double effectFrame[2] = { 0.0, 0.0 };


	ApplyEffects(channelframes, effectFrame);

	// Copy the effectFrame back to frame
	for (int c = 0; c < m_channels; c++)
	{
		frame[c] = effectFrame[c];
	}

	//
	// Phase 4: Advance the time and beats
	//

	// Time advances by the sample period
	m_time += GetSamplePeriod();

	// Beat advances by the sample period divided by the 
	// number of seconds per beat.  The inverse of seconds
	// per beat is beats per second.
	m_beat += GetSamplePeriod() / m_secperbeat;

	// When the measure is complete, we move to
	// a new measure.  We might be a fraction into
	// the new measure, so we subtract out rather 
	// than just setting to zero.
	if (m_beat > m_beatspermeasure)
	{
		m_beat -= m_beatspermeasure;
		m_measure++;
	}
	//
	// Phase 5: Determine when we are done
	//

	// We are done when there is nothing to play.  We'll put something more 
	// complex here later.
	return !m_instruments.empty() || m_currentNote < (int)m_notes.size();
}


// Get the time since we started generating audio
double CSynthesizer::GetTime()
{
	return m_time;
}
void CSynthesizer::Clear()
{
	m_instruments.clear();
	m_notes.clear();
}
void CSynthesizer::XmlLoadScore(IXMLDOMNode * xml)
{
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

		// Get the value of the attribute.  A CComVariant is a variable
		// that can have any type. It loads the attribute value as a
		// string (UNICODE), but we can then change it to an integer 
		// (VT_I4) or double (VT_R8) using the ChangeType function 
		// and then read its integer or double value from a member variable.
		CComVariant value;
		attrib->get_nodeValue(&value);

		if (name == L"bpm")
		{
			value.ChangeType(VT_R8);
			m_bpm = value.dblVal;
			m_secperbeat = 1 / (m_bpm / 60);
		}
		else if (name == L"beatspermeasure")
		{
			value.ChangeType(VT_I4);
			m_beatspermeasure = value.intVal;
		}

	}


	CComPtr<IXMLDOMNode> node;
	xml->get_firstChild(&node);
	for (; node != NULL; NextNode(node))
	{
		// Get the name of the node
		CComBSTR name;
		node->get_nodeName(&name);
		if (name == L"instrument")
		{
			XmlLoadInstrument(node);
		}
	}
}
void CSynthesizer::OpenScore(CString & filename)
{
	Clear();

	//
	// Create an XML document
	//

	CComPtr<IXMLDOMDocument>  pXMLDoc;
	bool succeeded = SUCCEEDED(CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER,
		IID_IXMLDOMDocument, (void**)&pXMLDoc));
	if (!succeeded)
	{
		AfxMessageBox(L"Failed to create an XML document to use");
		return;
	}

	// Open the XML document
	VARIANT_BOOL ok;
	succeeded = SUCCEEDED(pXMLDoc->load(CComVariant(filename), &ok));
	if (!succeeded || ok == VARIANT_FALSE)
	{
		AfxMessageBox(L"Failed to open XML score file");
		return;
	}

	//
	// Traverse the XML document in memory!!!!
	// Top level tag is <score>
	//

	CComPtr<IXMLDOMNode> node;
	pXMLDoc->get_firstChild(&node);
	for (; node != NULL; NextNode(node))
	{
		// Get the name of the node
		CComBSTR nodeName;
		node->get_nodeName(&nodeName);
		if (nodeName == "score")
		{
			XmlLoadScore(node);
		}
	}
	sort(m_notes.begin(), m_notes.end());
}
void CSynthesizer::XmlLoadInstrument(IXMLDOMNode * xml)
{
	wstring instrument = L"";

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

		if (name == "instrument")
		{
			instrument = value.bstrVal;
		}
	}

//	std::shared_ptr<EffectsManager> effectManager = std::make_shared<EffectsManager>(NUMEFFECTSCHANNELS);

	CComPtr<IXMLDOMNode> node;
	xml->get_firstChild(&node);
	for (; node != NULL; NextNode(node))
	{
		// Get the name of the node
		CComBSTR name;
		node->get_nodeName(&name);

		if (name == L"note")
		{
			XmlLoadNote(node, instrument);
		}
	}

}
void CSynthesizer::XmlLoadNote(IXMLDOMNode * xml, std::wstring & instrument)
{
	m_notes.push_back(CNote());
	m_notes.back().XmlLoad(xml, instrument);
}


