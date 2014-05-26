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

#ifndef VISUALSTREAMRECEIVER_H_INCLUDED
#define VISUALSTREAMRECEIVER_H_INCLUDED

#include <iostream>
#include "OscSender.h"
#include "OscSenderThread.h"
#include "Room.h"

namespace wonder {

/**
 *  An interface defining an OSC server thread that receives and handles the
 *  messages sent via WONDER's "Visual Stream".
 *  
 *  @see wonderlo::VSReceiver for a liblo-based implementation.
 */
class VisualStreamReceiver
{
public:
    
    /** Destructor. */
    virtual ~VisualStreamReceiver(){}
    
    /** Returns a OscSender that sends from the port of this server thread to
      * the specified destination.
      *
      * @param host A valid hostname or ip string (what exactly is supported
      *     (IPv6?) may depend on what OSC library is used to implement this
      *     interface).
      * @param port A validport number or service name (what exactly is supported
      *     may depend on what OSC library is used to implement this interface).
      * @return A OscSender that sends from the port of this server thread to
      *     the specified destination.
      */
    virtual OscSender* createSender(const std::string &host,
                                    const std::string &port) = 0;
    
    virtual OscSenderThread* createSenderThread(const std::string &host,
                                                const std::string &port) = 0;
    
    /** A listener interface that can be registered with the VisualStreamReceiver.
     *  On incoming OSC-messages, the VisualStreamReciever will call the listener's
     *  corresponding methods.
     */
    class Listener
    {
    public:
        virtual ~Listener(){}
        
        /** Called when the /WONDER/source/activate message is recieved.
         *
         *  @param sourceID The wonder-specific ID of the source to be
         *      activated (recieved with the incoming message).
         *      May range from 0 to N-1 where N is the total number of
         *      sources supported by the current WONDER installation.
         */
        virtual int onSourceActivate(int sourceID) = 0;
        
        /** Called when the /WONDER/source/deactivate message is recieved.
         *
         *  @param sourceID The wonder-specific ID of the source to be
         *      activated (recieved with the incoming message).
         *      May range from 0 to N-1 where N is the total number of
         *      sources supported by the current WONDER installation.
         */
        virtual int onSourceDeactivate(int sourceID) = 0;
        
        /** Called when the /WONDER/source/position message is recieved
         *  with one integer and two float parameters.
         *
         *  @param sourceID The wonder-specific ID of the source to be
         *      whose position parameters are transmitted.
         *  @param xPos The x-axis coordinate (in meters).
         *  @param yPos The y-axis coordinate (in meters).
         */
        virtual int onSourcePosition(int sourceID, float xPos, float yPos) = 0;
        
        /** Called when the /WONDER/source/angle message is recieved
         *  with one integer and oe float parameter.
         *
         *  @param sourceID The wonder-specific ID of the source to be
         *      whose angle parameters is transmitted.
         *  @param angle The new angle (in Degree).
         */
        virtual int onSourceAngle(int sourceID, float angle) = 0;
        
        /** Called when the /WONDER/source/type message is recieved.
         *
         *  @param sourceID The wonder-specific ID of the source to be
         *      whose type is transmitted.
         *  @param type 0 for a planewave source, 1 for a point source.
         */
        virtual int onSourceType(int sourceID, int type) = 0;
        
        /** Called when the /WONDER/source/name message is recieved.
         *
         *  @param sourceID The wonder-specific ID of the source to be
         *      whose name is transmitted.
         *  @param name The new name of the source.
         */
        virtual int onSourceName(int sourceID, const std::string& sourceName) = 0;
        
        /** Called when the /WONDER/source/color message is recieved.
         *
         *  @param sourceID The wonder-specific ID of the source to be
         *      whose colour is transmitted.
         *  @param r The red value of the new colour, ranging from 0 to 255.
         *  @param g The green value of the new colour, ranging from 0 to 255.
         *  @param b The blue value of the new colour, ranging from 0 to 255.
         */
        virtual int onSourceColor(int sourceID, int r, int g, int b) = 0;
        
        /** Called when the /WONDER/source/groupID message is recieved.
         *
         *  @param sourceID The wonder-specific ID of the source to be
         *      whose groupID is transmitted.
         *  @param groupID The groupID of the source.
         */
        virtual int onSourceGroupId(int sourceID, int groupID) = 0;
        
        /** Called when the /WONDER/source/rotationDirection message is recieved.
         *
         *  @param sourceID The wonder-specific ID of the source to be
         *      whose rotation direction is transmitted.
         *  @param inverted 1 for "inverted", 0 for "not inverted".
         */
        virtual int onSourceRotatingDirection(int sourceID, int inverted) = 0;
        
        /** Called when the /WONDER/source/scalingDirection message is recieved.
         *
         *  @param sourceID The wonder-specific ID of the source to be
         *      whose scaling direction is transmitted.
         *  @param inverted 1 for "inverted", 0 for "not inverted".
         */
        virtual int onSourceScalingDirection(int sourceID, int inverted) = 0;
        
        /** Called when the /WONDER/source/dopplerEffect message is recieved.
         *
         *  @param sourceID The wonder-specific ID of the source to be
         *      whose doppler effect setting is transmitted.
         *  @param on 1 for on, 0 for off.
         */
        virtual int onSourceDopplerEffect(int sourceID, int on) = 0;
        
        
        /** Called when the /WONDER/listener/position message is received.
         *
         *  @param listenerID The ID of the listener.
         *  @param x The x-coordinate of the listener's position.
         *  @param y The x-coordinate of the listener's position.
         *
         *  @warning So far, this command is only supported by a version of WONDER
         *      developed at HAW Hamburg.
         */
        virtual int onListenerPosition(int listenerID, float x, float y) = 0;
        
        /** Called when the /WONDER/global/maxNoSources message is recieved.
         *
         *  @param maxSources The maximum number of sources the current WONDER
         *      setup can handle.
         */
        virtual int onGlobalMaxNoSources(int maxSources) = 0;
        
        /** On receiving the /WONDER/global/renderpolygon message, the
         *  VisualStreamReceiver must create a Room object from the
         *  received information and pass it to the listener.
         *
         *  @param room A Room object describing the current WONDER setup's
         *      speaker positioning.
         */
        virtual int onGlobalRenderpolygon(Room room) = 0;
        
        /** Called when the /WONDER/project/xmlDump message is recieved.
         *
         *  @param err Error parameter sent by cWONDER (1 for error, 0 for no
         *      error).
         *  @param xmlDump An XML formatted string describing a cWONDER project
         *      (as defined in cwonder_project.dtd).
         *  @see XmlParser
         */
        virtual int onProjectXmlDump(int err, const std::string& xmlDump) = 0;
        
        virtual int onStreamVisualPong(int pingCount) = 0;
        
        /** Called when any type of /WONDER/stream/hub/connect message is recieved.
         *  The parameters of the message are ignored, but the Address to reply to
         *  is passed as parameter.
         *
         *  @param replyTo An OscSender sending to the Address the message was
         *      sent from.
         *
         *  @warning The VisualStreamReceiver may delete the OscSender replyTo after
         *      calling this method, so the listener must create its own copy if
         *      the Address shall be stored for further use.
         */
        virtual int onStreamVisualConnect(OscSender* replyTo)
            { return 0; }
        
        virtual int onStreamVisualDisconnect()
            { return 0; }
        
        virtual int onReply(std::string replyToMsg, int state, std::string msg) = 0;
        
        virtual int onPluginStandalone(bool standAloneOn) = 0;
    };
    
    /**
     *  Another listener interface especially for incoming ping messages. This is
     *  seperated from the general Listener as one might want to seperate the
     *  connection control seperately from the connection's communication content.
     */
    class PingHandler
    {
    public:
        virtual ~PingHandler(){}
        
        /** On receiving the /WONDER/stream/visual/ping message, the
         *  VisualStreamReceiver must call this method. The VisualStreamReceiver
         *  may delete the OscSender replyTo after calling this method, so the
         *  PingHandler may not store it for further use.
         *  For each incoming ping message, a pong message must be sent as reply
         *
         *  @param pingCount A value that must be sent back with the pong reply.
         *  @param replyTo An OscSender that sends to the Address where the ping
         *      message was recieved from.
         */
        virtual int onStreamVisualPing(int pingCount, OscSender* replyTo) = 0;
    };
    
    /** Starts this server thread. */
    virtual void start() = 0;

    /** Stops this server thread. */
    virtual void stop() = 0;
    
    /** Joins this server thread. */
    virtual void join() = 0;
    
    /** Sets the Listener to be called on incoming OSC messages. */
    virtual void setListener(Listener* listener) = 0;
    
    /** Sets the PingHandler to be called on incoming /WONDER/stream/visual/ping
        messages.
     */
    virtual void setPingHandler(PingHandler* pingHandler) = 0;
    
    /** Returns the port this server-thread is listening on. */
    virtual int port() = 0;
    
    /**
     *  A factory class that creates VisualStreamReceiver instances.
     *
     *  This allows the code within namespace wonder to create new
     *  VisualStreamReceivers without knowing the actual implementation.
     *  
     *  @see wonderlo::VSReceiver::getFactory
     */
    class Factory
    {
    public:
        /** Destructor. */
        virtual ~Factory(){}
        
        /** Creates a new VisualStreamReciever.
         *
         *  @return A new VisualStreamReciever or a nullptr if it could not be created.
         */
        virtual VisualStreamReceiver* createVisualStreamReceiver() = 0;
        
        /** Creates a new VisualStreamReciever.
         *
         *  @param port The port the new VisualStreamReceiver will listen on.
         *      A validport number or service name (what exactly is supported may
         *      depend on what OSC library is used to implement this interface).
         *  @return A new VisualStreamReciever or a nullptr if it could not be created
         *      (e.g. because of wrong parameter).
         */
        virtual VisualStreamReceiver* createVisualStreamReceiver(const std::string& port) = 0;
        
        /** Creates a new VisualStreamReciever that listens to a mutlicast group.
         *
         *  @param group The multicast group to join
         *  @param port The port the new VisualStreamReceiver will listen on.
         *      A validport number or service name (what exactly is supported may
         *      depend on what OSC library is used to implement this interface).
         *  @param iface A string specifying the name of a network interface to use.
         *
         *  @return A new VisualStreamReciever or a nullptr if it could not be created
         *      (e.g. because of wrong parameters).
         */
        virtual VisualStreamReceiver* createVisualStreamMulticastReceiver(const std::string& group,
                                                                          const std::string& port,
                                                                          const std::string& iface) = 0;
    };
};

}

#endif  // VISUALSTREAMRECEIVER_H_INCLUDED
