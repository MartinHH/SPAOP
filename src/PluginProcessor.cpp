/*
 * Copyright 2014 Martin Hansen
 *
 * This file is part of SPAOP (Spatial Audio Object Positioner).
 * 
 * SPAOP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * SPAOP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with SPAOP.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SpaopAudioProcessor::SpaopAudioProcessor():
    sourceController_(new wonder::SourceController(wonderlo::VSReceiver::getFactory(),
                                      this,
                                      wonderjuce::JuceConnectionTimer::getFactory(),
                                      wonderjuce::XmlFactory::getParser(),
                                      MAX_WONDER_SOURCES,
                                      "SPAOP")),
    zoomFactor_(INITIAL_ZOOM_FACTOR),
    showOtherSources_(true)
{
}

SpaopAudioProcessor::~SpaopAudioProcessor()
{
}

//==============================================================================
const String SpaopAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int SpaopAudioProcessor::getNumParameters()
{
    return wonder::Source::totalNumParams;
}

float SpaopAudioProcessor::getParameter (int index)
{
    return sourceController_->getSource().getParameter(index);
}

void SpaopAudioProcessor::setParameter (int index, float newValue)
{
    sourceController_->setParameterAndSendChange(index, newValue);
}

float SpaopAudioProcessor::getParameterByText(int parameterIndex,
                                    const juce::String &parameterText)
{
    float unnormalized = parameterText.getFloatValue();
    return wonder::Source::normalizeParameter(parameterIndex, unnormalized);
}

const String SpaopAudioProcessor::getParameterName (int index)
{
    return sourceController_->getSource().getParameterName(index);
}

const String SpaopAudioProcessor::getParameterText (int index)
{
    return sourceController_->getSource().getParameterText(index);
}

const String SpaopAudioProcessor::getParameterTextByValue (int index, float value)
{
    return wonder::Source::getParameterTextByValue(index, value);
}

float SpaopAudioProcessor::getParameterDefaultValue(int index)
{
    return wonder::Source::getParameterDefaultValue(index);
}

int SpaopAudioProcessor::getParameterNumSteps(int index)
{
    switch (index)
    {
        case wonder::Source::typeParam:
            return 2;
#ifdef SPAOP_DISCRETE
        case wonder::Source::xPosParam:
            return NUMSTEPS(COORD_MIN, COORD_MAX, COORD_PRECISION);
        case wonder::Source::yPosParam:
            return NUMSTEPS(COORD_MIN, COORD_MAX, COORD_PRECISION);
        case wonder::Source::angleParam:
            return NUMSTEPS(ANGLE_MIN, ANGLE_MAX, ANGLE_PRECISION);
#endif
        case wonder::Source::dopplParam:
            return 2;
        default:
            break;
    }
    
    return AudioProcessor::getParameterNumSteps(index);
}

const String SpaopAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String SpaopAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool SpaopAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool SpaopAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool SpaopAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SpaopAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SpaopAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double SpaopAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SpaopAudioProcessor::getNumPrograms()
{
    return 0;
}

int SpaopAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SpaopAudioProcessor::setCurrentProgram (int index)
{
}

const String SpaopAudioProcessor::getProgramName (int index)
{
    return String::empty;
}

void SpaopAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SpaopAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SpaopAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void SpaopAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    
    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
}

//==============================================================================
bool SpaopAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* SpaopAudioProcessor::createEditor()
{
    return new SpaopAudioProcessorEditor (this);
}

//==============================================================================
void SpaopAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // Storing values via XML:
    
    using namespace wonderjuce; // to shorten the calls to XmlFactory
    
    // Create an outer XML element:
    XmlElement xml ("SPAOPSETTINGS");
    
    // add source data to it:
    xml.addChildElement(XmlFactory::createSourceXml(sourceController_->getSource()));
    
    // add Room data:
    xml.addChildElement(XmlFactory::createRoomXml(*(sourceController_->getRoom())));
    
    // add connection settings:
    xml.addChildElement(XmlFactory::createConnectionXml(sourceController_));
    
    // add gui settings:
    xml.addChildElement(XmlFactory::createGuiXml(zoomFactor_, showOtherSources_));
    
    // then use this helper function to stuff it into the binary blob and return it..
    copyXmlToBinary (xml, destData);
    
    // Note about memory & deletion:
    // The destructor of XmlElement xml will delete all its child components, hence
    // the XmlElement allocated by source_->getXmlSourceElement() will be deleted too.
}

void SpaopAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    
    using namespace wonderjuce; // to shorten the calls to XmlFactory
    
    ScopedPointer<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState != nullptr){
        
        // make sure that it's actually our type of XML object..
        if (xmlState->hasTagName ("SPAOPSETTINGS")){
            
            // set source:
            wonder::Source source;
            XmlFactory::updateSourceFromXml(xmlState->getChildByName("source"), &source);
            sourceController_->setSource(source);
            
            // set room:
            wonder::Room newRoom =
            XmlFactory::createRoomFromXml(xmlState->getChildByName("room"));
            sourceController_->setRoom(newRoom);
            
            // set connection:
            XmlFactory::updateSourceControllerFromXml(xmlState->getChildByName("connection"),
                                                       sourceController_);
            
            // set gui:
            XmlElement* guiElement = xmlState->getChildByName("gui");
            if (guiElement != nullptr) {
                showOtherSources_ = guiElement->getBoolAttribute("show", showOtherSources_);
                zoomFactor_ = guiElement->getDoubleAttribute("zoom", zoomFactor_);
            }
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpaopAudioProcessor();
}

// ^^^^^ JUCE-AudioProcessor methods end here.
//==============================================================================
//==============================================================================
// vvvvv Non-JUCE-AudioProcessor methods:

wonder::SourceController* SpaopAudioProcessor::getSourceController() const
{
    return sourceController_;
}

void SpaopAudioProcessor::incomingParameterChange(
                wonder::Source::AutomatedParameters index, float newValue)
{
    sendParamChangeMessageToListeners (index, newValue);
}

float SpaopAudioProcessor::getZoomFactor() const
{
    return zoomFactor_;
}

void SpaopAudioProcessor::setZoomFactor(float zoomFactor)
{
    zoomFactor_ = zoomFactor;
}

bool SpaopAudioProcessor::showOtherSources() const
{
    return showOtherSources_;
}

void SpaopAudioProcessor::setShowOtherSources(bool showOtherSources)
{
    showOtherSources_ = showOtherSources;
}
