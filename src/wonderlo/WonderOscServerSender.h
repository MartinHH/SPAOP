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

#ifndef WONDEROSCSERVERSENDER_H_INCLUDED
#define WONDEROSCSERVERSENDER_H_INCLUDED


#include <iostream>
#include "WonderOscSender.h"

namespace wonderlo {

/**
 *  A lowrappers::Address implementing the wonder::OscSender interface, sending
 *  its messages from a specified lowrappers::ServerThread's port.
 */
class WonderOscServerSender    :   public WonderOscSender
{
public:
    
    /** Constructor.
     *
     *  @param host A valid hostname or ip string (see the liblo documentation for
     *      what exactly is supported).
     *  @param port A validport number or service name (see the liblo documentation for
     *      what exactly is supported).
     *  @param server a ServerThread whose port the outgoing messages will be sent from.
     *  @param proto The protocol to use (LO_UDP, LO_TCP or LO_UNIX). See the liblo
     *      documentation for details - LO_UDP should be used for communication with
     *      the current version of WONDER.
     */
    WonderOscServerSender(const std::string &host, const std::string &port,
                   lowrappers::ServerThread &server, int proto = LO_UDP);
    
    /** Constructor.
     *
     *  @param addr The address to sent to.
     *  @param server a ServerThread whose port the outgoing messages will be sent from.
     *  @param isOwner Specifies who is responsible for freeing the lo_address
     *      addr (by calling lo_address_free). If set to true, this VSSenderobject
     *      will call lo_address_free on addr when it is destroyed or when a new
     *      Address is set. If set to false, the responsibility to call
     *      lo_address_free remains with the caller of this constructor.
     */
    WonderOscServerSender(lo_address addr, lowrappers::ServerThread &server, bool isOwner = true);
    
    /** Copy constructor. Will create a new copy of the lo_address stored within
     *  other, so the new WonderOscServerSender will be owner of its lo_address while other's
     *  ownership status will remain unchanged.
     *
     *  @param other The WonderOscServerSender to be copied.
     */
    WonderOscServerSender(const WonderOscServerSender& other);
    
    /** Destructor. Will call lo_address_delete on the lo_address stored internally
     *  if this WonderOscServerSender object is the owner of the lo_address.
     */
    virtual ~WonderOscServerSender();
    
protected:
    
    /** Overriding VSSender::wonderSend, this changes the way OSC
     *  messages are sent from the various methods inherited from
     *  wonder::OscSender.
     *  This implementation passes the parameters on to
     *  lowrappers::Address::sendVAListFrom, sending from the port
     *  of the server passed in the WonderOscServerSender constructor.
     *
     *  @param path The OSC path of the message.
     *  @param msg The Message to be sent.
     *
     *  @see lowrappers::Address::sendFrom, WonderOscSender::wonderSend
     */
    void wonderSend(const MessageWithPath &msg);
    
private:
    WonderOscServerSender &operator= (const WonderOscServerSender other);
    
    lowrappers::ServerThread &server_;
};

}

#endif  // WONDEROSCSERVERSENDER_H_INCLUDED
