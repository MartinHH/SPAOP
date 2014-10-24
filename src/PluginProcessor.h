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

#include <memory>

#include "../JuceLibraryCode/JuceHeader.h"
#include "Source.h"
#include "JuceConnectionTimer.h"
#include "SourceController.h"
#include "VSReceiver.h"
#include "XmlFactory.h"


#define NUMSTEPS(min, max, prec) ( (int) (((max)-(min))/(prec)) + 1)


//==============================================================================
/**
 *  The SPAOP-plugin's processor class. Initializes its
 *  wonder::SourceController instance and works as the interface to the host
 *  software.
 *
 *  @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html)
 */
class SpaopAudioProcessor   :   public AudioProcessor,
                                wonder::SourceController::Listener
{
public:
    //==============================================================================
    /** Constructor. */
    SpaopAudioProcessor();
    
    /** Destructor. */
    ~SpaopAudioProcessor();

    //==============================================================================
    // vvvvv Methods inherited from juce::AudioProcessor:
    
    /** @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html) */
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    
    /** @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html) */
    void releaseResources();

    /** @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html) */
    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

    //==============================================================================
    
    /** @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html) */
    AudioProcessorEditor* createEditor();
    
    /** @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html) */
    bool hasEditor() const;

    //==============================================================================
    /** @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html) */
    const String getName() const;

    /** Returns the number of automatable parameters of this plugin. This equals the
     *  value of wonder::Source::totalNumParams.
     *
     *  @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html)
     */
    int getNumParameters();

    /** Forwards the call to the wonder::Source::getParameter method of the source
     *  controlled by this plugin.
     *
     *  @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html),
     *      wonder::Source::getParameter
     */
    float getParameter (int index);
    
    /** Forwards the call to the wonder::SourceController::setParameterAndSendChange.
     *
     *  @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html),
     *      wonder::SourceController::setParameterAndSendChange
     */
    void setParameter (int index, float newValue);
    
    /** Forwards the call to the wonder::Source::getParameterByText method of the source
     *  controlled by this plugin. This overrides a method of juce::AudioProcessor that
     *  is not contained in the official version of JUCE but only in the fork of JUCE
     *  that was created for SPAOP.
     *
     *  @see wonder::Source::getParameterByText
     */
    float getParameterByText(int parameterIndex, const String& parameterText);
    
    /** Forwards the call to the wonder::Source::getParameterName method of the source
     *  controlled by this plugin.
     *
     *  @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html),
     *      wonder::Source::getParameterName
     */
    const String getParameterName (int index);
    
    /** Forwards the call to the wonder::Source::getParameterText method of the source
     *  controlled by this plugin.
     *
     *  @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html),
     *      wonder::Source::getParameterText
     */
    const String getParameterText (int index);
    
    /** Forwards the call to the wonder::Source::getParameterTextByValue method of the
     *  source controlled by this plugin. This overrides a method of juce::AudioProcessor
     *  that is not contained in the official version of JUCE but only in the fork of JUCE
     *  that was created for SPAOP.
     *
     *  @see wonder::Source::getParameterTextByValue
     */
    const String getParameterTextByValue (int index, float value);
    
    int getParameterNumSteps(int index);
    
    /** Forwards the call to wonder::Source::getParameterDefaultValue.
     *
     *  @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html),
     *      wonder::Source::getParameterDefaultValue
     */
    float getParameterDefaultValue (int index);
    
    /** @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html) */
    const String getInputChannelName (int channelIndex) const;
    
    /** @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html) */
    const String getOutputChannelName (int channelIndex) const;
    
    /** @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html) */
    bool isInputChannelStereoPair (int index) const;
    
    /** @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html) */
    bool isOutputChannelStereoPair (int index) const;

    /** @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html) */
    bool acceptsMidi() const;
    
    /** @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html) */
    bool producesMidi() const;
    
    /** @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html) */
    bool silenceInProducesSilenceOut() const;
    
    /** @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html) */
    double getTailLengthSeconds() const;

    //==============================================================================
    
    /** Returns 0.
     *
     *  @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html)
     */
    int getNumPrograms();
    
    /** Returns 0.
     *
     *  @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html)
     */
    int getCurrentProgram();
    
    /** Does nothing.
     *
     *  @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html)
     */
    void setCurrentProgram (int index);
    
    /** Returns an empty juce::String.
     *
     *  @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html)
     */
    const String getProgramName (int index);
    
    /** Does nothing.
     *
     *  @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html)
     */
    void changeProgramName (int index, const String& newName);

    //==============================================================================
    
    /** Stores the current state of the plugin (including the state of the source
     *  controlled by this plugin) into the MemoryBlock destData.
     *  All attributes are stored in XML format.
     *
     *  @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html)
     */
    void getStateInformation (MemoryBlock& destData);
    
    /** Retores the a state of the plugin (including the state of the source
     *  controlled by this plugin) from a memory block (as stored with
     *  getStateInformation).
     *
     *  @see [juce::AudioProcessor](http://www.juce.com/api/classAudioProcessor.html)
     */
    void setStateInformation (const void* data, int sizeInBytes);
    
    // ^^^^^ juce::AudioProcessor methods end here.
    //==============================================================================
    //==============================================================================
    // vvvvv Non-juce::AudioProcessor methods:
    
    /** Sets the coordinates, sending the corresponding OSC-message.
     *  For coordinate changes, this is to be prefered since it will result in
     *  only one OSC message (while two separate calls to setParameterNotifyingHost
     *  might result in two messages).
     *
     *  @param normalizedX The new value for the x-coordinate, normalized within
     *      [0.0 , 1.0].
     *  @param normalizedY The new value for the y-coordinate, normalized within
     *      [0.0 , 1.0].
     *
     *  @see setParameter
     */
    void setCoordinates(float normalizedX, float normalizedY);
    
    /** Sets the coordinates, notifying both the host and the OSC-messaging.
     *  For coordinate changes, this is to be prefered since it will result in
     *  only one OSC message (while two separate calls to setParameterNotifyingHost
     *  might result in two messages).
     *
     *  @param normalizedX The new value for the x-coordinate, normalized within
     *      [0.0 , 1.0].
     *  @param normalizedY The new value for the y-coordinate, normalized within
     *      [0.0 , 1.0].
     *
     *  @see juce::AudioProcessor::setParameterNotifyingHost
     */
    void setCoordinatesNotifyingHost(float normalizedX, float normalizedY);
    
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
    std::shared_ptr<wonder::SourceController> getSourceController() const;
    
    /** Returns the wonder::Source object corresponding to the ID assigned to this
     *  instance of the plugin.
     *
     *  @return The wonder::Source object corresponding to the ID assigned to this
     *      instance of the plugin.
     */
    const wonder::Source& getSource() const;
    
    /** Tries to "lock" the currently assigned ID. This will register this plugin
     *  for callbacks from incoming OSC messages from the WONDER system and will
     *  enable the forwarding of automation data from the host DAW to the WONDER
     *  system.
     *  To sum it up: this must be called to make the plugin work.
     *  Once the ID is locked, it cannot be changed (until unlockID() is called).
     *
     *  @see unlockID(), idIsLocked().
     */
    void lockID();
    
    /** "Unlocks" the currently assigned ID. This will unregister this plugin
     *  for callbacks from incoming OSC messages from the WONDER system and will
     *  disable the forwarding of automation data from the host DAW to the WONDER
     *  system.
     *  This must be called if you want to change the ID of a plugin and its ID
     *  is currently locked.
     *
     *  @see lockID(), idIsLocked().
     */
    void unlockID();
    
    /** Returns true if the SourceID of this plugin is currently locked.
     *
     *  @return true if the SourceID of this plugin is currently locked.
     *
     *  @see lockID(), unlockID().
     */
    bool idIsLocked() const { return idIsLocked_; }
    
    /** Tries to set the (WONDER-specific) ID of the source controlled by this
     *  plugin. This will only work if the ID currently is not locked.
     *
     *  @param sourceID the ID of the source to be controlled by this plugin.
     *      Must be >= 0 and < the maximum number of sources (default: 64).
     *
     *  @see idIsLocked(), lockID(), unlockID().
     */
    void setSourceID(int sourceID);
    
    /** Returns the (WONDER-specific) ID of the source controlled by this
     *  plugin.
     *
     *  @return The (WONDER-specific) ID of the source controlled by this
     *      plugin.
     */
    int getSourceID() const { return sourceID_; }
    
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
    
    /** Returns true if the GUI shall the sources' names. (As this parameters
     *  shall be restorable via getStateInformation and setStateInformation, it needs
     *  to be stored within this class.)
     *
     *  @return true if the GUI shall show the sources' names.
     */
    bool showNames() const;
    
    /** Set the GUI's showNames parameter (As this parameters shall be
     *  restorable via getStateInformation and setStateInformation, it needs to be
     *  stored within this class.)
     *
     *  @param showNames true if the GUI shall show the sources' names.
     */
    void setShowNames(bool showNames);
    
    /** Sets the address where outgoing OSC messages will be sent in "linked to
     *  WONDER" mode will be sent (i.e. the address of cWONDER). (Internally,
     *  the call is passed to wonder::SourceController::setCWonderAddress.)
     *
     *  @param ip A valid (IPv4) ip string.
     *  @param port A valid port number between 1024 and 65535.
     *
     *  @return true if the address has been set, false if the parameters were
     *      invalid.
     */
    bool setCWonderAddress(const std::string &ip, const std::string &port);

private:
    //==============================================================================
    
    // the SourceController is created as a reference-counted singleton:
    static std::mutex singletonMutex_;
    static std::shared_ptr<wonder::SourceController> instance_;
    static std::shared_ptr<wonder::SourceController> getControllerSingleton();
    
    std::shared_ptr<wonder::SourceController> sourceController_;
    
    int sourceID_;
    bool idIsLocked_;
    
    // GUI parameters that need to be stored:
    float zoomFactor_;
    bool showOtherSources_;
    bool showNames_;
    
    //==============================================================================
    //vvvvv  Implementing SourceControl::Listener
    
    void incomingParameterChange (wonder::Source::AutomatedParameters index,
                                  float newValue);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpaopAudioProcessor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
