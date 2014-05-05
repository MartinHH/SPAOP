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

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Source.h"
#include "JuceConnectionTimer.h"
#include "SourceController.h"
#include "VSReceiver.h"
#include "XmlFactory.h"


#define NUMSTEPS(min, max, prec) ( (int) (((max)-(min))/(prec)) + 1)


//==============================================================================
/**
    The SPAOP-plugin's processor class. Initializes its
    wonder::SourceController instance and works as the interface to the host
    software.
*/
class SpaopAudioProcessor   :   public AudioProcessor,
                                wonder::SourceController::Listener
{
public:
    //==============================================================================
    SpaopAudioProcessor();
    ~SpaopAudioProcessor();

    //==============================================================================
    // vvvvv Methods inherited from juce::AudioProcessor:
    
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void releaseResources();

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    AudioProcessorEditor* createEditor();
    bool hasEditor() const;

    //==============================================================================
    const String getName() const;

    int getNumParameters();

    float getParameter (int index);
    void setParameter (int index, float newValue);
    
    float getParameterByText(int parameterIndex, const String& parameterText);
    
    const String getParameterName (int index);
    const String getParameterText (int index);
    const String getParameterTextByValue (int index, float value);
    
    int getParameterNumSteps(int index);
    float getParameterDefaultValue (int index);
    
    const String getInputChannelName (int channelIndex) const;
    const String getOutputChannelName (int channelIndex) const;
    bool isInputChannelStereoPair (int index) const;
    bool isOutputChannelStereoPair (int index) const;

    bool acceptsMidi() const;
    bool producesMidi() const;
    bool silenceInProducesSilenceOut() const;
    double getTailLengthSeconds() const;

    //==============================================================================
    int getNumPrograms();
    int getCurrentProgram();
    void setCurrentProgram (int index);
    const String getProgramName (int index);
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    void getStateInformation (MemoryBlock& destData);
    void setStateInformation (const void* data, int sizeInBytes);
    
    // ^^^^^ juce::AudioProcessor methods end here.
    //==============================================================================
    //==============================================================================
    // vvvvv Non-juce::AudioProcessor methods:
    
    
    /** Callback from the timer. If the timer runs out, we did not receive a ping
        message for the incoming "visual" stream which means: the connections is
        lost.
     */
    void timerCallback();
    
    /** Returns a pointer to the wonder::SourceController object that contains the
     *  main logic and the communication threads of this plugin.
     *
     *  @return A pointer to the wonder::SourceController object.
     */
    wonder::SourceController* getSourceController() const;
    
    /** Returns the zoom factor for the GUI. (As this parameters shall be restorable
     *  via getStateInformation and setStateInformation, it needs to be stored
     *  within this class.(
     *
     *  @return The zoom factor for the GUI.
     */
    float getZoomFactor() const;
    
    /** Sets the zoom factor for the GUI. (As this parameters shall be restorable
     *  via getStateInformation and setStateInformation, it needs to be stored
     *  within this class.)
     *
     *  @param zoomFactor The zoom factor for the GUI.
     */
    void setZoomFactor(float zoomFactor);

    /** Returns true if the GUI shall show all active sources. (As this parameters
     *  shall be restorable via getStateInformation and setStateInformation, it needs
     *  to be stored within this class.)
     *
     *  @return true if the GUI shall show all active sources.
     */
    bool showOtherSources() const;
    
    /** Set the GUI's showOtherSources parameter (As this parameters shall be
     *  restorable via getStateInformation and setStateInformation, it needs to be
     *  stored within this class.)
     *
     *  @param showOtherSources true if the GUI shall show all active sources.
     */
    void setShowOtherSources(bool showOtherSources);

private:
    //==============================================================================
    ScopedPointer<wonder::SourceController> sourceController_;
    
    // GUI parameters that need to be stored:
    float zoomFactor_;
    bool showOtherSources_;
    
    //==============================================================================
    //vvvvv  Implementing SourceControl::Listener
    
    void incomingParameterChange (wonder::Source::AutomatedParameters index,
                                  float newValue);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpaopAudioProcessor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
