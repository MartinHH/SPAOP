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

#include "StreamMulticaster.h"

namespace wonderlo {
    
StreamMulticaster::StreamMulticaster(const std::string& sourceHost, const std::string& sourcePort,
                                     const std::string& mcGroup, const std::string& mcPort,
                                     const std::string& mcIfaceIp, const std::string& connectPort,
                                     const std::string& streamInPort, const std::string& streamName,
                                     const uint8_t ttl):
    connectIn_(connectPort, NULL),
    streamIn_(streamInPort, NULL),
    streamSource_(sourceHost, sourcePort),
    mcGroup_(mcGroup, mcPort),
    streamPath_(("/WONDER/stream/" + streamName + "/"))
{
    // set the multicast parameters:
    mcGroup_.setIfaceByIp(mcIfaceIp);
    mcGroup_.setTtl(ttl);
    
    // add ping handler:
    streamIn_.addListener((streamPath_ + "ping").c_str(), "i",
                          lowrappers::ListenerMaker(*this, &StreamMulticaster::onStreamPingI));
    
    // add forwarder for any other message sent to streamIn_:
    streamIn_.addListener(NULL, NULL,
                          lowrappers::ListenerMaker(*this, &StreamMulticaster::forwardMessage));
    
    // add connect-forwarder to connectIn_ (type is ignored):
    connectIn_.addListener((streamPath_ + "connect").c_str(), NULL,
                           lowrappers::ListenerMaker(*this, &StreamMulticaster::onStreamConnect));
    
    // add unkown command replier to connectIn_:
    connectIn_.addListener(NULL, NULL,
                           lowrappers::ListenerMaker(*this, &StreamMulticaster::onUnknownConnectIn));
        
    // start the threads:
    connectIn_.start();
    streamIn_.start();
}
    
StreamMulticaster::~StreamMulticaster()
{
    // stop the threads:
    connectIn_.stop();
    streamIn_.stop();
    
    // join them:
    connectIn_.join();
    streamIn_.join();
}

int StreamMulticaster::getConnectPort() const
{
    return connectIn_.port();
}

int StreamMulticaster::getStreamInPort() const
{
    return streamIn_.port();
}

std::string StreamMulticaster::getStreamSourceHost() const
{
    return streamSource_.hostname();
}

std::string StreamMulticaster::getStreamSourcePort() const
{
    return streamSource_.port();
}

std::string StreamMulticaster::getMulticastGroup() const
{
    return mcGroup_.hostname();
}

std::string StreamMulticaster::getMulticastPort() const
{
    return mcGroup_.port();
}

std::string StreamMulticaster::getMulticastIface() const
{
    return mcGroup_.getIface();
}
    
int StreamMulticaster::main(const std::string& cWonderAddr, const std::string& type)
{
    std::string cWip = CWONDER_DEFAULT_IP_STR;
    std::string cWport = CWONDER_DEFAULT_PORT_STR;
    
    if(cWonderAddr.length() > 0) {
        const std::vector<const std::string> cw =
            parsetools::splitString(cWonderAddr, ':');
        
        if(cw.size() != 2) {
            std::cerr << "cWONDER address format must be ip:port." << std::endl;
            return -1;
        }
        
        if(parsetools::isValidIP(cw[0])){
            cWip = cw[0];
        } else {
            std::cerr << cw[0] << " is not a valid ip." << std::endl;
            return -1;
        }
        
        if(parsetools::isValidPort(cw[1])){
            cWport = cw[1];
        } else {
            std::cerr << cw[1] << " is not a valid port." << std::endl;
            return -1;
        }
    }
    
    StreamMulticaster mc(cWip, cWport, VISUAL_MC_GROUP_STR, VISUAL_MC_PORT_STR,
                         VISUAL_MC_IFACE_IP_STR, MULTICASTER_PORT_STR,
                         MULTICASTER_STREAM_IN_PORT_STR, type);
    
    std::cout << "This is a " << type << " stream multicaster:" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Stream data from " << mc.getStreamSourceHost() << ":"
    << mc.getStreamSourcePort();
    std::cout << " is received on port " << mc.getStreamInPort() << std::endl;
    std::cout << "and forwarded to multicast group " << mc.getMulticastGroup() << ":"
    << mc.getMulticastPort() << " on interface " << mc.getMulticastIface() << std::endl;
    std::cout << std::endl;
    
    std::cout << "/WONDER/" << type << "/stream/connect messages are accepted on port "
    << mc.getConnectPort() << std::endl;
    std::cout << std::endl;
    
    std::cout << "Press enter to stop" << std::endl;
    std::cin.ignore();
    
    return 0;
}
    
//==============================================================================
// vvvvv handlers for incoming OSC-messages:
    
int StreamMulticaster::forwardMessage(const char *path, const char *types, lo_arg **argv,
                                      int argc, lo_message msg)
{
    // forward the message to multicast group:
    mcGroup_.sendFrom(connectIn_, std::string(path), msg);
    
    return 0;
}
    
int StreamMulticaster::onStreamPingI(const char *path, const char *types, lo_arg **argv,
                                     int argc, lo_message msg)
{
    // send pong reply:
    lowrappers::Address replyTo(lo_message_get_source(msg), false);
    replyTo.sendFrom(streamIn_, streamPath_ + "pong" , "i", argv[0]->i);
    
    return 1; // returning 1 ensures that the call will also be passed to forwardMessage
}
    
int StreamMulticaster::onStreamConnect(const char *path, const char *types, lo_arg **argv,
                                       int argc, lo_message msg)
{
    // whenever a connect message is recieved (by the connectIn_-thread, this
    // StreamMulticaster sends another connect message from its streamIn_ to
    // cWONDER, provoking a "reconnect":
    streamSource_.sendFrom(streamIn_, streamPath_ + "connect", "s", "StreamMulticaster");
    
    return 0;
}
 
int StreamMulticaster::onUnknownConnectIn(const char *path, const char *types, lo_arg **argv,
                                          int argc, lo_message msg)
{
    // create a reply adress from msg:
    WonderOscSender replyTo(lo_message_get_source(msg), false);
    
    // send reply:
    const std::string pathStr(path);
    replyTo.sendReply(pathStr, 1, "Unknown command: " + pathStr);
    
    return 0;
}
    
}