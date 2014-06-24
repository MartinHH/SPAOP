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

#ifndef OSCSENDER_H_INCLUDED
#define OSCSENDER_H_INCLUDED

#include <iostream>
#include <stdint.h>
#include "Source.h"
#include "Room.h"

namespace wonder {

/**
 *  An abstract class describing the interface for a class that sends out the
 *  WONDER-specific OSC-messages.
 *  This seperates the actual OSC implementation from the WONDER-specific code
 *  and enables the developer to switch from one OSC implementation to another
 *  without touching the code within namespace wonder.
 *
 *  @see wonderlo::WonderOscSender (and further classesderived from it) for
 *      liblo-based implementations of this interface.
 *
 *  @see VisualStreamReceiver::createSender for an implementation-independent
 *      way to create instances of this interface.
 */
class OscSender
{
public:
    
    /** Destructor. */
    virtual ~OscSender(){}
    
    /** Sets the destination address where this OscSender will send to.
     *
     *  @param newHost A valid hostname or ip string (what exactly is supported
     *      (IPv6?) may depend on what OSC library is used to implement this
     *      interface).
     *  @param newPort A valid port number or service name (what exactly is supported
     *      may depend on what OSC library is used to implement this interface).
     */
    virtual void setAddress(const std::string &newHost, const std::string &newPort) = 0;
    
    /** Returns an url representing the destination address where this OscSender
     *  will send to.
     *
     *  @return An url representing the destination address where this OscSender
     *      will send to.
     */
    virtual std::string url() const = 0;
    
    /** Returns the hostname (or IP) of the destination address where this OscSender
     *  will send to.
     *
     *  @return The hostname (or IP) of the destination address where this OscSender
     *      will send to.
     */
    virtual std::string hostname() const = 0;
    
    /** Returns the destination port (or service name) where this OscSender
     *  will send to.
     *
     *  @return The destination port (or service name) where this OscSender
     *      will send to.
     */
    virtual std::string port() const = 0;
    
    /** Returns the time-to-live value for messages sent by this sender.
     *
     *  @return The time-to-live value for messages sent by this sender.
     */
    virtual int getTtl() const = 0;
    
    /** Sets the time-to-live value for messages sent by this sender.
     *  This is required for sending UDP multicast messages.
     *
     *  @param ttl The time-to-live value for messages sent by this sender.
     */
    virtual void setTtl(uint8_t ttl) = 0;
    
    /** Returns the name of the network interface that will be used for sending
     *  messages. This is relevant for sending multicast messages.
     *
     *  @return The name of the network interface that will be used for sending
     *      to this address. Might be "(null)" if no interface has been set.
     */
    virtual std::string getIface() const = 0;
    
    /** Sets the network interface to use for sending to this address by its IP.
     *
     *  @param ip The IP of the local interface where the messages shall be sent
     *      from.
     */
    virtual void setIfaceByIp(const std::string& ip) = 0;
    
    /** Sends the /WONDER/source/activate message. This will activate
     *  the specified source within WONDER.
     *
     *  @param sourceID The wonder-specific ID of the source to be
     *      activated. May range from 0 to N-1 where N is the total
     *      number of sources supported by the current WONDER
     *      installation.
     *  @see sendSourceDeactivate
     */
    virtual void sendSourceActivate(int sourceID) = 0;
    
    /** Sends the /WONDER/source/deactivate message. This will deactivate
     *  the specified source within WONDER.
     *
     *  @param sourceID The wonder-specific ID of the source to be
     *      deactivated. May range from 0 to N-1 where N is the total
     *      number of sources supported by the current WONDER
     *      installation.
     *  @see sendSourceActivate
     */
    virtual void sendSourceDeactivate(int sourceID) = 0;
    
    /** Sends the /WONDER/source/type message. This will set the source's
     *  type (either "planewave" or "point").
     *
     *  @param sourceID The wonder-specific ID of the source to be
     *      changed. May range from 0 to N-1 where N is the total
     *      number of sources supported by the current WONDER
     *      installation.
     *  @param type 0 for a planewave source, 1 for a point source.
     */
    virtual void sendSourceType(int sourceID, int type) = 0;
    
    /** Sends the /WONDER/source/position message. This will set the
     *  source's position immediately.
     *
     *  @param sourceID The wonder-specific ID of the source to be
     *      moved. May range from 0 to N-1 where N is the total
     *      number of sources supported by the current WONDER
     *      installation.
     *  @param x The x-axis coordinate (in meters).
     *  @param y The y-axis coordinate (in meters).
     */
    virtual void sendSourcePosition(int sourceID, float x, float y) = 0;
    
    /** Sends the /WONDER/source/position message with a duration
     *  parameter. This will immediately make the source start move
     *  from its current position to the specified position where
     *  it will arrive within the specified duration.
     *
     *  @param sourceID The wonder-specific ID of the source to be
     *      moved. May range from 0 to N-1 where N is the total
     *      number of sources supported by the current WONDER
     *      installation.
     *  @param x The x-axis coordinate (in meters).
     *  @param y The y-axis coordinate (in meters).
     *  @param duration How long it will take for the source to arrive
     *      at the specified position (in seconds).
     */
    virtual void sendSourcePosition(int sourceID, float x, float y,
                                        float duration) = 0;
    
    /** Sends the /WONDER/source/position message with a duration and
     *  a timestamp parameter. This will make the source start move
     *  from its current position at the moment specified by the timestamp
     *  to the specified position where it will arrive within the specified
     *  duration.
     *
     *  @param sourceID The wonder-specific ID of the source to be
     *      moved. May range from 0 to N-1 where N is the total
     *      number of sources supported by the current WONDER
     *      installation.
     *  @param x The x-axis coordinate (in meters).
     *  @param y The y-axis coordinate (in meters).
     *  @param duration How long it will take for the source to arrive
     *      at the specified position (in seconds).
     *  @param timestamp The moment when the source will start to move (in
     *      seconds from "now" - "now" being the moment when this message
     *      is received by cWONDER).
     *
     *  @warning For this command to work, an instance of the time base helper
     *      application jfWONDER must be running and connected to cWONDER.
     *      Generally, the WONDER system can be run without jfWONDER running - 
     *      in that case, there is no time base and this command will never be
     *      executed. Since there are no warnings issued by cWONDER about
     *      jfWONDER not running, make sure that jfWONDER is running when using
     *      this command.
     */
    virtual void sendSourcePosition(int sourceID, float x, float y,
                                    float duration, float timestamp) = 0;
    
    /** Sends the /WONDER/source/angle message. This sets a planewave
     *  source's direction.
     *
     *  @param sourceID The wonder-specific ID of the source to be
     *      changed. May range from 0 to N-1 where N is the total
     *      number of sources supported by the current WONDER
     *      installation.
     *  @param angle The direction of the planewave source as an angle
     *      (in degrees). The zero degrees position is "to the right"
     *      (aka eastward aka 3 o'clock direction), an increase in angle
     *      rotates the direction clockwise.
     */
    virtual void sendSourceAngle(int sourceID, float angle) = 0;
    
    /** Sends the /WONDER/source/name message, changing the source's name.
     *  The name is only relevant for UI representaions of the source,
     *  within the rendering, only the ID is used.
     *
     *  @param sourceID The wonder-specific ID of the source to be
     *      changed. May range from 0 to N-1 where N is the total
     *      number of sources supported by the current WONDER
     *      installation.
     *  @param name The new name of the source.
     */
    virtual void sendSourceName(int sourceID, const std::string& name) = 0;
    
    /** Sends the /WONDER/source/color message, changing the colour to be used for
     *  that source in any GUI.
     *
     *  @param sourceID The wonder-specific ID of the source to be
     *      changed. May range from 0 to N-1 where N is the total
     *      number of sources supported by the current WONDER
     *      installation.
     *  @param r The red value of the new colour.
     *  @param g The green value of the new colour.
     *  @param b The blue value of the new colour.
     */
    virtual void sendSourceColor(int sourceID, uint8_t r, uint8_t g, uint8_t b) = 0;
    
    /** Sends the /WONDER/source/color message, changing the colour to be used for
     *  that source in any GUI.
     *
     *  @param sourceID The wonder-specific ID of the source to be
     *      changed. May range from 0 to N-1 where N is the total
     *      number of sources supported by the current WONDER
     *      installation.
     *  @param colour The new colour.
     */
    virtual void sendSourceColor(int sourceID, Colour colour){
        sendSourceColor(sourceID, colour.r, colour.g, colour.b);
    }
    
    /** Sends the /WONDER/source/rotationDirection message, changing the
     *  rotation direction of the source.
     *  As of WONDER version 3.1.0, this is only used by xWONDER in the
     *  context of rotating source groups.
     *
     *  @param sourceID The wonder-specific ID of the source to be
     *      changed. May range from 0 to N-1 where N is the total
     *      number of sources supported by the current WONDER
     *      installation.
     *  @param inverted 1 for "inverted", 0 for "not inverted".
     */
    virtual void sendSourceRotatingDirection(int sourceID, int inverted) = 0;
    
    /** Sends the /WONDER/source/scalingDirection message, changing the
     *  scaling direction of the source.
     *  As of WONDER version 3.1.0, this is only used by xWONDER in the
     *  context of source groups.
     *
     *  @param sourceID The wonder-specific ID of the source to be
     *      changed. May range from 0 to N-1 where N is the total
     *      number of sources supported by the current WONDER
     *      installation.
     *  @param inverted 1 for "inverted", 0 for "not inverted".
     */
    virtual void sendSourceScalingDirection(int sourceID, int inverted) = 0;
    
    /** Sends the /WONDER/source/dopplerEffect message, switching the
     *  doppler effect for this source on or off.
     *
     *  @param sourceID The wonder-specific ID of the source to be
     *      changed. May range from 0 to N-1 where N is the total
     *      number of sources supported by the current WONDER
     *      installation.
     *  @param on 1 for on, 0 for off.
     */
    virtual void sendSourceDopplerEffect(int sourceID, int on) = 0;
    
    /** Sends the /WONDER/listener/position message, changing the position of
     *  the listener which is used to determine which speakers are used for focused
     *  sources.
     *
     *  @param listenerID The ID of the listener.
     *  @param x The x-coordinate of the listener's position.
     *  @param y The y-coordinate of the listener's position.
     *
     *  @warning So far, this command is only supported by a version of WONDER
     *      developed at HAW Hamburg.
     */
    virtual void sendListenerPosition(int listenerID, float x, float y) = 0;
    
    /** Sends the /WONDER/global/renderpolygon message. The message contains
     *  information about the positioning of the WFS speaker array of the
     *  current setup.
     *
     *  @param roomName A string containing the name of the transmitted room
     *      setup.
     *  @param noOfVertices The number of vertices transmitted.
     *  @param vertices An array of three-dimensional vertex coordinates of
     *      noOfVertices length. The array is expected to be ordered in a way
     *      that vertices[x] and vertices[(x+1)%noOfVertices()] are the two
     *      endpoints of an array of speakers.
     *  @see Room
     */
    virtual void sendGlobalRenderpolygon(const std::string& roomName,
                                         int noOfVertices,
                                         Room::Vertex* vertices) = 0;
    
    /** Sends the /WONDER/stream/visual/connect message. Once this is sent
     *  (to cWONDER), cWONDER will start sending the "visual stream" to
     *  the IP & port where this message was sent from.
     */
    virtual void sendStreamVisualConnect() = 0;
    
    /** Sends the /WONDER/stream/visual/connect message. Once this is sent
     *  (to cWONDER), cWONDER will start sending the "visual stream" to
     *  the IP & port where this message was sent from.
     *
     *  @param name A name that identifies the sender. cWONDER keeps a list
     *      of connected stream clients. This list will contain the client's
     *      IP & port along with this name.
     */
    virtual void sendStreamVisualConnect(const std::string& name) = 0;
    
    /** Sends the /WONDER/stream/visual/connect message. Once this is sent
     *  (to cWONDER), cWONDER will start sending the "visual stream" to
     *  the specified address
     *
     *  @param host A valid hostname. (Since cWONDER uses the liblo library
     *      for sending OSC, what exactly is supported (IPv6?) depends on the
     *      liblo version being used for the WONDER system in use.)
     *  @param port A valid port. (Since cWONDER uses the liblo library
     *      for sending OSC, you should see the liblo documentation for details
     *      on what exactly is supported.)
     */
    virtual void sendStreamVisualConnect(const std::string& host,
                                         const std::string& port) = 0;
    
    /** Sends the /WONDER/stream/visual/disconnect message. Once this is sent
     *  (to cWONDER), cWONDER is expected to stop sending the "visual stream"
     *  to the IP & port where this message was sent from.
     *
     *  @warning Currently not supported by cWONDER. Although
     *      /WONDER/stream/visual/disconnect is listed in WONDER's
     *      OSC_and_Commandline_Overview.ods spreadsheet, the command isn't
     *      actually supported (at least not by WONDER version 3.1.0).
     */
    virtual void sendStreamVisualDisconnect() = 0;
    
    /** Sends the /WONDER/stream/visual/ping message. This is sent by instances
     *  who send out a "visual stream" (like cWONDER or VisualHub).
     *
     *  @param count An integer value chosen by the sender.
     */
    virtual void sendStreamVisualPing(int count) = 0;
    
    /** Sends the /WONDER/stream/visual/pong message. This is sent by clients
     *  who receive a "visual stream" as reply to /WONDER/stream/visual/ping.
     *
     *  @param count The integer value that was received with the incoming
     *      /WONDER/stream/visual/ping message.
     */
    virtual void sendStreamVisualPong(int count) = 0;
    
    /** Sends the /WONDER/reply message. This is can be used for any replies
     *  to incoming messages, confirming successfull execution or
     *  transmitting an error message.
     *
     *  @param replyToMessage The OSC path of the message that triggered this
     *      reply.
     *  @param state 0 for confirmations of successfull operations, != 0 for error
     *      messages.
     *  @param message The actual reply text.
     */
    virtual void sendReply(const std::string& replyToMessage, int state,
                           const std::string& message) = 0;
    
    /** Sends the /WONDER/project/createWithScore message. This tells cWONDER
     *  to create a new project that includes a score from the ScorePlayer.
     *
     *  Check the corresponding /WONDER/reply message for possible error codes.
     *  (Codes may be: 0 for success, 1 for empty project name, 2 for "project
     *  already exists, 3 for any other error (including file i/o)).
     *
     *  @param projectName The name of the new project.
     */
    virtual void sendProjectCreateWithScore(const std::string& projectName) = 0;
    
    /** Sends the /WONDER/project/create message. This tells cWONDER to create a
     *  new project (which does not include a score from the ScorePlayer).
     *
     *  Check the corresponding /WONDER/reply message for possible error codes.
     *  (Codes may be: 0 for success, 1 for empty project name, 2 for "project
     *  already exists, 3 for any other error (including file i/o)).
     *
     *  @param projectName The name of the new project.
     */
    virtual void sendProjectCreate(const std::string& projectName) = 0;
    
    /** Sends the /WONDER/project/load message. This tells cWONDER to restore
     *  an existing project.
     *
     *  Check the corresponding /WONDER/reply message for possible error codes.
     *  (Codes may be: 0 for success, 1 for "file not found", 2 or 3 for various
     *  project loading errors (see the message's string for details)).
     *
     *  @param projectName The name of the project to be loaded.
     */
    virtual void sendProjectLoad(const std::string& projectName) = 0;

    /** Sends the /WONDER/project/save message without a parameter. This tells
     *  cWONDER to store the current project under its current name.
     */
    virtual void sendProjectSave() = 0;
    
    /** Sends the /WONDER/project/save message with a projectName parameter.
     *  This tells cWONDER to store the current project under the given name.
     *
     *  @param projectName The name under which the project shall be stored.
     */
    virtual void sendProjectSave(const std::string& projectName) = 0;
    
    /** Sends the /WONDER/plugin/standalone message. This is used to change the
     *  communication mode of the SourceController class:
     *  In "standalone" mode, the SourceController sends the source controlling
     *  messages to its peers (via multicast) only.
     *  If standalone is off ("linked to wonder" mode), the SourceController sends
     *  those messages to cWONDER.
     *
     *  @param standAlone true for "standalone" mode, false for "linked to wonder"
     *      mode.
     */
    virtual void sendPluginStandalone(const bool standAlone) = 0;
    
    /** Sends out all source-related information for one Source object (type,
     *  position, angle, colour, doppler effect setting and name).
     *
     *  @param src The Source whose parameters shall be sent.
     */
    void sendFullSourceInfo(const Source& src)
    {
        const int id_ = src.getID();
        sendSourceType(id_, src.getType());
        sendSourcePosition(id_, src.getXPos(), src.getYPos());
        sendSourceAngle(id_, src.getAngle());
        sendSourceColor(id_, src.getRed(), src.getGreen(), src.getBlue());
        sendSourceDopplerEffect(id_, src.dopplerIsEnabled());
        sendSourceName(id_, src.getName());
    }
};

}
    
#endif  // OSCSENDER_H_INCLUDED
