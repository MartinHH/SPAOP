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

#ifndef WONDEROSCSENDER_H_INCLUDED
#define WONDEROSCSENDER_H_INCLUDED

#include <iostream>
#include "Address.h"
#include "OscSender.h"
#include "MessageWithPath.h"

namespace wonderlo {

/**
 *  A lowrappers::Address implementing the wonder::OscSender interface.
 */
class WonderOscSender   :   public lowrappers::Address,
                            public virtual wonder::OscSender
{
public:
    
    /** Constructor.
     *
     *  @param host A valid hostname or ip string (see the liblo documentation for
     *      what exactly is supported).
     *  @param port A validport number or service name (see the liblo documentation for
     *      what exactly is supported).
     *  @param proto The protocol to use (LO_UDP, LO_TCP or LO_UNIX). See the liblo
     *      documentation for details - LO_UDP should be used for communication with
     *      the current version of WONDER.
     */
    WonderOscSender(const std::string &host, const std::string &port,
             int proto = LO_UDP);
    
    /** Constructor.
     *
     *  @param addr The address to sent to.
     *  @param isOwner Specifies who is responsible for freeing the lo_address
     *      addr (by calling lo_address_free). If set to true, this WonderOscSenderobject
     *      will call lo_address_free on addr when it is destroyed or when a new
     *      Address is set. If set to false, the responsibility to call
     *      lo_address_free remains with the caller of this constructor.
     */
    WonderOscSender(lo_address addr, bool isOwner = true);
    
    /** Copy constructor. Will create a new copy of the lo_address stored within
     *  other, so the new WonderOscSender will be owner of its lo_address while other's
     *  ownership status will remain unchanged.
     *
     *  @param other The WonderOscSender to be copied.
     */
    WonderOscSender(const WonderOscSender& other);
    
    /** Destructor. Will call lo_address_delete on the lo_address stored internally
     *  if this WonderOscSender object is the owner of the lo_address.
     */
    virtual ~WonderOscSender();
    
    void setAddress(const std::string &newHost, const std::string &newPort);
    
    std::string url() const;
    
    std::string hostname() const;
    
    std::string port() const;
    
    int getTtl() const;
    
    void setTtl(uint8_t ttl);
    
    std::string getIface() const;
    
    void setIfaceByIp(const std::string& ip);
    
    void sendSourceActivate(int id);
    
    void sendSourceDeactivate(int id);
    
    void sendSourceType(int id, int type);
    
    void sendSourcePosition(int id, float x, float y);
    
    void sendSourcePosition(int id, float x, float y, float duration);
    
    void sendSourcePosition(int id, float x, float y, float duration, float timestamp);
    
    void sendSourceAngle(int id, float angle);
    
    void sendSourceName(int id, const std::string& name);
    
    void sendSourceColor(int id, uint8_t r, uint8_t g, uint8_t b);
    
    void sendSourceRotatingDirection(int id, int inverted);
    
    void sendSourceScalingDirection(int id, int inverted);
    
    void sendSourceDopplerEffect(int id, int on);
    
    void sendListenerPosition(int listenerID, float x, float y);
    
    void sendGlobalRenderpolygon(const std::string& roomName,
                                 int noOfVertices,
                                 wonder::Room::Vertex* vertices);
    
    void sendStreamVisualConnect();
    
    void sendStreamVisualConnect(const std::string& name);
    
    void sendStreamVisualConnect(const std::string& host,
                                 const std::string& port);
    
    void sendStreamVisualDisconnect();
    
    void sendStreamVisualPing(int count);
    
    void sendStreamVisualPong(int count);
    
    void sendReply(const std::string& replyToMessage, int state,
                   const std::string& message);
    
    void sendPluginStandalone(const bool standAlone);
    
protected:
    
    /** This method is used to send OSC messages from the various
     *  methods inherited from wonder::OscSender.
     *  By overriding it, derived classes can change the way those
     *  messages are sent (e.g. from which port they are sent).
     *  This implementation simply passes the parameters on to
     *  lowrappers::Address::send.
     *
     *  @param path The OSC path of the message.
     *  @param msg The Message to be sent.
     *
     *  @see lowrappers::Address::send, WonderOscServerSender::wonderSend
     */
    virtual void wonderSend(const MessageWithPath &msg);
    
    
private:
    
    /** This method constructs a MessageWithPath object from its parameters
     *  and passes it on to wonderSend(const MessageWithPath &msg).
     *
     *  @param path The OSC path of the message.
     *  @param types The types of the data items in the message.
     *  @param ... The data values to be transmitted. The types of
     *      the arguments passed here must agree with the types
     *      specified in the type parameter.
     *
     *  @see lowrappers::Address::sendVAList, WonderOscServerSender::wonderSend
     */
    virtual void wonderSend(const std::string &path,
                            const std::string types, ...);
    
    
    WonderOscSender &operator= (const WonderOscSender other);
};

}
    
#endif  // WONDEROSCSENDER_H_INCLUDED
