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

#ifndef __stream_multicaster__StreamMulticaster__
#define __stream_multicaster__StreamMulticaster__

#include <iostream>
#include <stdint.h>
#include "WonderOscServerSender.h"
#include "GenericServerThreadListener.h"

namespace wonderlo {

/** 
 *  A StreamMulticaster forwards any OSC messages that are sent to its StreamInPort
 *  to a multicast group. Its purpose is to connect to one of WONDER's OSC "streams"
 *  ("visual" / "timer" / "render" / "score") and forward the stream to multiple
 *  recievers.
 *
 *  Two threads listen on two ports:
 *      - the StreamInPort: any messages recieved here are forwarded to the multicast
 *        group.
 *        If a /WONDER/stream/<em>streamName</em>/ping message is received on this
 *        port, it is both forwarded to the multicast group and replied to with the
 *        corresponding /WONDER/stream/<em>streamName</em>/pong message.
 *      - the ConnectPort: if a /WONDER/stream/<em>streamName</em>/connect message
 *        (with any parameter constellation) is recieved here, the StreamMulticaster
 *        sends a /WONDER/stream/<em>streamName</em>/connect(String name) message
 *        to the stream source (i.e. cWONDER).
 *        This triggers cWONDER to resend any messages that are sent on the first
 *        connect message. Those are forwarded to the multicast group, so whenever
 *        a new client sends a /WONDER/stream/<em>streamName</em>/connect message to
 *        the ConnectPort, he will receive the reply he would have recieved if he had
 *        connected to cWONDER directly.
 *        @warning Due to a bug in cWONDER (version 3.1.0), cWONDER will resend all
 *              information on repeated /WONDER/stream/<em>streamName</em>/connect
 *              to the client that last registered for the corresponding stream (not
 *              to the client that was reconnected).
 *              If you want to use this feature, make sure you connect last or fix
 *              that bug.
 */
class StreamMulticaster
{
public:
    /** Constructor.
     *
     *  @param sourceHost The hostname/IP of cWONDER.
     *  @param sourcePort The port of cWONDER.
     *  @param mcGroup The IP of the multicast group that the stream shall be
     *      forwarded to.
     *  @param mcPort The port of the multicast group that the stream shall be
     *      forwarded to.
     *  @param mcIfaceIp The IP of the local interface where the multicast messages shall
     *      be sent from.
     *  @param connectPort The port to be used for incoming
     *      /WONDER/stream/<em>streamName</em>/connect messages.
     *  @param streamInPort The port to be used for recieving the incoming stream.
     *  @param streamName The name of the stream that shall be forwarded. As of
     *      WONDER version 3.1.0, this can be "visual", "timer", "render" or
     *      "score".
     *  @param ttl The time-to-live value for the messages sent to the multicast
     *      group.
     */
    StreamMulticaster(const std::string& sourceHost, const std::string& sourcePort,
                      const std::string& mcGroup, const std::string& mcPort,
                      const std::string& mcIfaceIp, const std::string& connectPort,
                      const std::string& streamInPort, const std::string& streamName,
                      const uint8_t ttl=VISUAL_MC_TTL);
    
    /** Destructor. */
    virtual ~StreamMulticaster();
    
    /** Returns the port where incoming connect messages may be sent to.
     *
     *  @return The port where incoming connect messages may be sent to.
     */
    int getConnectPort() const;

    /** Returns the port where the incoming stream is recieved.
     *
     *  @return The port where the incoming stream is recieved.
     */
    int getStreamInPort() const;

    /** Returns the hostname of the stream source (i.e. cWONDER).
     *
     *  @return The hostname of the stream source (i.e. cWONDER).
     */
    std::string getStreamSourceHost() const;
    
    /** Returns the port of the stream source (i.e. cWONDER).
     *
     *  @return The port of the stream source (i.e. cWONDER).
     */
    std::string getStreamSourcePort() const;
    
    /** Returns the IP of the multicast group.
     *
     *  @return The IP of the multicast group.
     */
    std::string getMulticastGroup() const;
    
    /** Returns the port of the multicast group.
     *
     *  @return The port of the multicast group.
     */
    std::string getMulticastPort() const;
    
    /** Returns the interface identifier for the network interface used for
     *  sending to the multicast group.
     *
     *  @return The interface identifier for the network interface used for
     *      sending to the multicast group.
     */
    std::string getMulticastIface() const;
    
private:
    lowrappers::ServerThread connectIn_;
    lowrappers::ServerThread streamIn_;
    lowrappers::Address streamSource_;
    lowrappers::Address mcGroup_;
    const std::string streamPath_;          // "/WONDER/stream/streamName/"
    
    //==============================================================================
    // vvvvv handlers for incoming OSC-messages:
    
    int forwardMessage(const char *path, const char *types, lo_arg **argv,
                       int argc, lo_message msg);
    
    int onStreamPingI(const char *path, const char *types,lo_arg **argv,
                            int argc, lo_message msg);
    
    int onStreamConnect(const char *path, const char *types,lo_arg **argv,
                            int argc, lo_message msg);
    
    int onUnknownConnectIn(const char *path, const char *types, lo_arg **argv,
                           int argc, lo_message msg);
};

}

#endif /* defined(__stream_multicaster__StreamMulticaster__) */
