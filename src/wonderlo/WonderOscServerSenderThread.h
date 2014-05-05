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

#ifndef WONDEROSCSERVERSENDERTHREAD_H_INCLUDED
#define WONDEROSCSERVERSENDERTHREAD_H_INCLUDED

#include "WonderOscSenderThread.h"


#define MSG_WAIT_INTERVAL 1 /**< How long the thread will block before
                                checking for isStopped again (in ms) */

namespace wonderlo {
    
/**
 *  A WonderOscSenderThread thread, sending its messages from a specified
 *  lowrappers::ServerThread's port.
 */
class WonderOscServerSenderThread   :   public WonderOscSenderThread
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
    WonderOscServerSenderThread(const std::string &host, const std::string &port,
                                lowrappers::ServerThread &server, int proto = LO_UDP);
    
    /** Destructor. */
    virtual ~WonderOscServerSenderThread();
    
protected:
    /** Overriding WonderOscSenderThread::sendMethod, this changes the way how the
     *  sending thread sends out the messages it picks up from the message queue.
     *  This implementation uses the lowrappers::Address::sendFrom method.
     *
     *  @param msg The message to be sent.
     */
    void sendMethod(const MessageWithPath& msg);

private:
    lowrappers::ServerThread& server_;
};
    
}


#endif  // WONDEROSCSERVERSENDERTHREAD_H_INCLUDED
