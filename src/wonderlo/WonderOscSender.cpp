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

#include "WonderOscSender.h"

namespace wonderlo {

WonderOscSender::WonderOscSender(const std::string &host, const std::string &port,
                   int proto):
    lowrappers::Address(host, port, proto)
{
}

WonderOscSender::WonderOscSender(lo_address addr, bool isOwner):
    lowrappers::Address(addr, isOwner)
{
}

WonderOscSender::~WonderOscSender()
{
}

WonderOscSender::WonderOscSender(const WonderOscSender& other):
    lowrappers::Address(other)
{
}

void WonderOscSender::setAddress(const std::string &newHost, const std::string &newPort)
{
    Address::setAddress(newHost, newPort);
}

std::string WonderOscSender::url() const
{
    return Address::url();
}

std::string WonderOscSender::hostname() const
{
    return Address::hostname();
}

std::string WonderOscSender::port() const
{
    return Address::port();
}

int WonderOscSender::getTtl() const
{
    return Address::getTtl();
}
    
void WonderOscSender::setTtl(uint8_t ttl)
{
    Address::setTtl(ttl);
}
    
std::string WonderOscSender::getIface() const
{
    return Address::getIface();
}
    
void WonderOscSender::setIfaceByIp(const std::string& ip)
{
    Address::setIfaceByIp(ip);
}
    
void WonderOscSender::sendSourceActivate(int id)
{
    wonderSend("/WONDER/source/activate", "i", id);
}

void WonderOscSender::sendSourceDeactivate(int id)
{
    wonderSend("/WONDER/WONDER/source/deactivate", "i", id);
}

void WonderOscSender::sendSourceType(int id, int type)
{
    wonderSend("/WONDER/source/type", "ii", id, type);
}

void WonderOscSender::sendSourcePosition(int id, float x, float y)
{
    wonderSend("/WONDER/source/position", "iff", id, x, y);
}

void WonderOscSender::sendSourcePosition(int id, float x, float y, float duration)
{
    wonderSend("/WONDER/source/position", "ifff", id, x, y, duration);
}

void WonderOscSender::sendSourcePosition(int id, float x, float y, float duration, float timestamp)
{
    wonderSend("/WONDER/source/position", "iffff", id, x, y, duration, timestamp);
}

void WonderOscSender::sendSourceAngle(int id, float angle)
{
    wonderSend("/WONDER/source/angle", "if", id, angle);
}

void WonderOscSender::sendSourceName(int id, const std::string& name)
{
    wonderSend("/WONDER/source/name", "is", id, name.c_str());
}

void WonderOscSender::sendSourceColor(int id, uint8_t r, uint8_t g, uint8_t b)
{
    wonderSend("/WONDER/source/color", "iiii", id, r, g, b);
}

void WonderOscSender::sendSourceRotatingDirection(int id, int inverted)
{
    wonderSend("/WONDER/source/rotationDirection", "ii", id, inverted);
}

void WonderOscSender::sendSourceScalingDirection(int id, int inverted)
{
    wonderSend("/WONDER/source/scalingDirection", "ii", id, inverted);
}

void WonderOscSender::sendSourceDopplerEffect(int id, int on)
{
    wonderSend("/WONDER/source/dopplerEffect", "ii", id, on);
}
    
void WonderOscSender::sendListenerPosition(float x, float y)
{
    wonderSend("/WONDER/listener/position", "ff", x, y);
}

void WonderOscSender::sendGlobalRenderpolygon(const std::string& roomName,
                                              int noOfVertices,
                                              wonder::Room::Vertex* vertices)
{
    MessageWithPath msg("/WONDER/global/renderPolygon");
    msg.add("s", roomName.c_str());
    msg.add("i", noOfVertices);
    for(int i=0; i<noOfVertices; i++){
        msg.add("fff", vertices[i].x, vertices[i].y, vertices[i].z);
        
    }
    wonderSend(msg);
}

void WonderOscSender::sendStreamVisualConnect()
{
    wonderSend("/WONDER/stream/visual/connect", "");
}

void WonderOscSender::sendStreamVisualConnect(const std::string& name)
{
    wonderSend("/WONDER/stream/visual/connect", "s", name.c_str());
}

void WonderOscSender::sendStreamVisualConnect(const std::string& host,
                                         const std::string& port)
{
    wonderSend("/WONDER/stream/visual/connect", "ss", host.c_str(), port.c_str());
}

void WonderOscSender::sendStreamVisualDisconnect()
{
    wonderSend("/WONDER/stream/visual/disconnect", "");
}

void WonderOscSender::sendStreamVisualPing(int count)
{
    wonderSend("/WONDER/stream/visual/ping", "i", count);
}

void WonderOscSender::sendStreamVisualPong(int count)
{
    wonderSend("/WONDER/stream/visual/pong", "i", count);
}

void WonderOscSender::sendReply(const std::string &replyToMessage, int state,
                            const std::string &message)
{
    wonderSend("/WONDER/reply", "sis", replyToMessage.c_str(),
               state, message.c_str());
}
    
void WonderOscSender::sendPluginStandalone(const bool standAlone)
{
    wonderSend("/WONDER/plugin/standalone", "i", standAlone);
}

void WonderOscSender::wonderSend(const std::string &path, const std::string types, ...)
{
    va_list args;
    va_start(args, types);  // Be careful: 'va_start' has undefined behavior
                            // with reference types. While passing types as
                            // const std::string &type worked fine under OSX,
                            // it messed up everything under Windows (Visual
                            // Studio 2013) - so do not change it back!
    
    MessageWithPath msg(path);
    msg.addVAList(types, args);
    wonderSend(msg); 
}

void WonderOscSender::wonderSend(const MessageWithPath &msg)
{
    send(msg.path(), msg);
}

}
