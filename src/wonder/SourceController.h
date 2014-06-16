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

#ifndef SOURCECONTROLLER_H_INCLUDED
#define SOURCECONTROLLER_H_INCLUDED

#include <memory>

#include "SourceCollection.h"
#include "WonderHeader.h"
#include "ConnectionTimer.h"
#include "VisualStreamReceiver.h"
#include "OscSender.h"
#include "PingControl.h"
#include "XmlParser.h"

#define ABS(x) ((x) > 0 ? (x) : -(x))

namespace wonder {

/** 
 *  The main communication node that keeps track of all virtual sources and enables
 *  the host DAW to control one of them.
 */
class SourceController 	:   VisualStreamReceiver::Listener,
                            VisualStreamReceiver::PingHandler,
                            PingControl::Listener
{
public:
    
    /** An interface for a listener that listens to various callbacks triggered
     *  by incoming messages or connection timeout alerts.
     */
    class Listener
    {
    public:
        /** Destructor. */
        virtual ~Listener() {}
        
        /** Called when one of the sources automatable parameters is changed by
         *  an incoming message.
         */
        virtual void incomingParameterChange (Source::AutomatedParameters index,
                                              float newValue) = 0;
        
        /** Called when the maximum number of sources allowed by the WONDER
         *  system has been received via the /global/maxNoSources message.
         *
         *  @param maxSources the maximum number of sources the WONDER system
         *      can handle. Wonder IDs must not exceed maxSources-1.
         *  @warning The SourceController class ignores these values. Its
         *      maximum number of sources is statically set at compile time
         *      via the MAX_WONDER_SOURCES define.
         */
        virtual void incomingMaxNoSources(const int maxSources){}
        
        /** Called when the (incoming data) connection is lost. */
        virtual void connectionLost () {}
    };

    /** Constructor. In order to be able to use own derived implementations,
     *  the main components are passed via this constructor.
     *
     *  @param server The server thread listening to incoming OSC messages.
     *  @param dataDest The destination address for outgoing WONDER data.
     *  @param streamSource the address used to subscribe to an incoming
     *      "Visual Stream".
     *  @param listener A listener listening to incoming parameter changes
     *      and connection timeouts.
     *  @param timer A timer to keep track of ping timeouts. Must be set to
     *      an appropriate interval.
     *  @param maxSources the maximum number of sources allowed by the
     *      system.
     *  @param clientName The name of this stream client within the WONDER
     *      world. This will appear in the list of connected clients that
     *      cWONDER sends via the /WONDER/stream/connected message.
     */
    SourceController(VisualStreamReceiver::Factory* vsFactory,
                     Listener* listener,
                     ConnectionTimer::Factory* timerFactory,
                     XmlParser* xmlParser,
                     int maxSources,
                     const std::string clientName = "");
    
    /** Destructor. */
    virtual ~SourceController();
    
    /** Returns the Source object that is controlled by this SourceController.
     *
     *  @return The Source object that is controlled by this SourceController.
     */
    const Source& getSource() const;
    
    /** Returns the SourceCollection object that is controlled by this
     *  SourceController. To ensure that the object can be held by a GUI
     *  component (which might be destructed after this SourceController
     *  has been destructed), the reference-counting std::shared_ptr is
     *  used.
     *
     *  @return The SourceCollection object that is controlled by this
     *      SourceController.
     */
    std::shared_ptr<const SourceCollection> getSources() const;
    
    /** Copies the parameters of a given Source object to the internal
     *  SourceCollection (overwriting the Source with the same ID) and
     *  sets the source's ID as the one controlled by this SourceController.
     *  This is meant to be used for restoring states from saved files
     *  (e.g. the audio-plugin's presets) and is only allowed while the
     *  SourceController is not communicating (neither sending nor listening).
     *
     *  @return true on success - false if the Source object's parameters
     *      could not be copied because the source's ID was out of range or
     *      beauce the SourceController's was communicating.
     *
     *  @warning Take extra care that you really want to copy all of
     *      a Source's paramters (e.g. including isActive()).
     */
    bool setSource(const Source &source);
    
    /** Sets the source's ID. Changing the ID is only allowed while the
     *  SourceController is not communicating (neither sending nor listening).
     *
     *  @param sourceID The new ID of the controlled source.
     *  @return true, if the ID was set succesfully; false, if setting the ID
     *      was impossible (due to an active connection).
     */
    bool setID(int sourceID);
    
    /** Sets the value of the parameter indicated by paramIndex and sends it to
     *  cWONDER (if its value's difference from the value that was sent last is 
     *  relevant). Depending on the parameter, the right message format is chosen.
     *  
     *  @param index A parameter index value as defined in the enum
     *      Source::AutomatedParameters.
     *  @param normalizedValue The new value for that parameter, normalized within
     *      [0.0 , 1.0].
     *
     *  @see COORD_PRECISION, ANGLE_PRECISION
     */
    void setParameterAndSendChange (int paramIndex, float normalizedValue);
    
    /** Sets the coordinates and sends them to cWONDER (if the values' difference
     *  from the values that were sent last is relevant).
     *  For coordinate changes, this is to be prefered since it will only send
     *  one OSC message while two separate calls to setParameterAndSendChange
     *  might result in two messages.
     *
     *  @param normalizedX The new value for the x-coordinate, normalized within
     *      [0.0 , 1.0].
     *  @param normalizedY The new value for the y-coordinate, normalized within
     *      [0.0 , 1.0].
     *
     *  @see COORD_PRECISION
     */
    void setCoordinatesAndSendChange (float normalizedX, float normalizedY);
    
    /** Updates the source's name and sends out the corresponding OSC message
     *  (if sending is enabled).
     *
     *  @param newSourceName The new name of the source.
     */
    void updateSourceName(const std::string& newSourceName);
    
    /** Updates the source's colour and sends out the corresponding OSC message
     *  (if sending is enabled).
     *
     *  @param colour The new colour of the source.
     */
    void updateSourceColour(const Colour colour);
    
    /** Sets whether this SourceController shall communicate with WONDER
     *  ("linked to wonder") or not. If it is linked to WONDER, source control
     *  messages (position, angle, etc) are sent to WONDER, visual stream connect
     *  message is sent to the multicaster. Otherwise (stand-alone-mode), all
     *  messages are sent to the plugin's peers via the multicast group.
     *
     *  @param linked true, if this plugin shall communicate to WONDER, false for
     *      stand-alone-mode.
     *  @param notifyPeers true if a /WONDER/plugin/standalone message shall be
     *      sent to the multicast group to notify the peer plugins about the
     *      change.
     */
    void setLinkedToWonder(bool linked, bool notifyPeers = true);
    
    /** Returns the linkedToWonder status.
     *
     *  @return true, if this SourceController is set to communicate with WONDER.
     *
     *  @see setLinkedToWonder
     */
    bool isLinkedToWonder() const;
    
    /** Sets the ID locked status. If the sourceID is locked, it cannot be changed,
     *  and listening to incoming messages is enabled. If it is not locked, the ID
     *  can be changed, but listening to incoming messages is disabled.
     *  On switching from !isLocked to isLocked, a /WONDER/source/activate message
     *  will be sent, on switching from isLocked to !isLocked, a /WONDER/source/deactivate
     *  message will be sent.
     *
     *  @param isLocked true if changing the ID shall be disabled and listening
     *      to incoming messages enabled.
     */
    void setIdIsLocked(bool isLocked);
    
    /** Returns true if the source ID is locked. */
    bool idIsLocked() const;

    /** Returns the current status of the incoming connection.
     *
     *  @return the current status of the incoming connection.
     *
     *  @see enum ConnectionStates.
     */
    ConnectionStates connectionStatus() const;
    
    /** Returns a string representation of the current status of the incoming
     *  "Visual Stream" connection.
     *
     *  @return A string representation of the current status of the incoming
     *      "Visual Stream" connection.
     */
    std::string connectionStatusString() const;
    
    /** Returns the port this instance is listening on (or would be listening on
     *      in case it isn't listening at the moment).
     *
     *  @return The port this instance is listening on (or would be listening on
     *      in case isn't listening at the moment).
     */
    int rcvPort() const;
    
    /** Returns the Address where the WONDER control message are sent to. */
    const std::string getDataDestHostAndPort() const;
    
    /** Returns a pointer to the Room object stored internally.
     *
     *  @return a pointer to the Room object stored internally.
     */
    std::shared_ptr<const Room> getRoom() const;
    
    /** Sets the interal Room object to the parameters of a given Room object.
     *
     *  @param room The Room object that shall be copied into the internal Room
     *      object.
     */
    void setRoom(const Room& room);

private:
    SourceController(const SourceController& other);            // copying not allowed
    SourceController &operator= (const SourceController other); // assignment not allowed
    
    std::unique_ptr<VisualStreamReceiver> server_;
    std::unique_ptr<OscSenderThread> cWonder_;      // cWONDER
    std::unique_ptr<OscSenderThread> mCaster_;      // the StreamMulticaster
    std::unique_ptr<OscSenderThread> peerGroup_;    // the multicast group
    
    XmlParser* xmlParser_;
    std::shared_ptr<SourceCollection> sources_;     // object might be share with a GUI
                                                    // that might be destructed after
                                                    // this object -> shared_ptr
    int sourceID_;
    std::shared_ptr<Room> room_;
    Listener* listener_;
    PingControl pingControl_;
    const std::string clientName_;      // the name of this client within the WONDER world
    bool linkedToWonder_;               // is sending outgoing OSC messages to WONDER
    bool isLocked_;
    ConnectionStates cStatus_;
    float lastValues_[Source::totalNumParams];  // the last values of automated parameters
                                                // that were sent out (denormalized!)
    
    /** Sets an automated parameter from an unnormalized value,
        notifying the listener.
     */
    void setIncomingParameter (int sourceID, Source::AutomatedParameters index,
                               float unnormalizedValue);
    
    /** Returns true if the difference of a parameter's current value from the value
     *  that was sent out last (stored in lastValues_) is greater than a defined
     *  threshold.
     *
     *  @see COORD_PRECISION, ANGLE_PRECISION
     *
     *  @param index the parameter-index of the parameter to check.
     */
    bool relevantChange(int index);
    
    /** Returns the OscSenderThread to send the source control messages to. Depending
     *  on the communication mode (linked or standalone), this is either cWonder_ or
     *  peerGroup_.
     */
    OscSenderThread& dataDest() const;
    
    /** Returns the OscSenderThread to send the /stream/visual/connect messages to.
     *  Depending on the communication mode (linked or standalone), this is either
     *  mCaster_ or peerGroup_.
     */
    OscSenderThread& streamSource() const;
    
    /**
     *  Sends /WONDER/source/activate and the full source info for the source
     *  controlled by this SourceController.
     */
    void sendOwnState();
    
    //==============================================================================
    //vvvvv  Implementing ConnectionTimer::Listener
    
    void onTimeout();
    
    //==============================================================================
    //vvvvv  Implementing PingControl::Listener
    
    void connectionLost(const int connectionID);
    
    //==============================================================================
    //vvvvv  OSC-message-handlers implementing wonder::VisualStreamReceiver::Listener
    
    int onSourceActivate(int wondID);
    
    int onSourceDeactivate(int wondID);
    
    int onSourcePosition(int wondID, float xPos, float yPos);
    
    int onSourceAngle(int wondID, float angle);
    
    int onSourceType(int wondID, int type);
    
    int onSourceName(int wondID, const std::string& sourceName);
    
    int onSourceColor(int wondID, int r, int g, int b);
    
    int onSourceGroupId(int wondID, int groupID)
    { return 0; /** Not supported (yet?). */}
    
    int onSourceRotatingDirection(int wondID, int rotDir)
    { return 0; /** Not supported (yet?). */}
    
    int onSourceScalingDirection(int wondID, int scalDir)
    { return 0; /** Not supported (yet?). */}
    
    int onSourceDopplerEffect(int wondID, int doppler);
    
    int onListenerPosition(int listenerID, float x, float y)
    { return 0; /** Not supported (yet?). */}
    
    int onGlobalMaxNoSources(int maxSources);
    
    int onGlobalRenderpolygon(Room& room);
    
    int onProjectXmlDump(int err, const std::string& xmlDump);
    
    int onStreamVisualPing(int pingCount, OscSender* replyTo);
    
    int onStreamVisualPong(int pingCount){ return 0; }
    
    int onStreamVisualConnect(OscSender* replyTo);
    
    int onReply(std::string replyToMsg, int state, std::string msg);
    
    int onPluginStandalone(bool standAloneOn);

};
    
}


#endif  // SOURCECONTROLLER_H_INCLUDED
