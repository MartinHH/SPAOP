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

#ifndef WONDEROSCSENDERTHREAD_H_INCLUDED
#define WONDEROSCSENDERTHREAD_H_INCLUDED


#include <mutex>
#include <condition_variable>
#include <deque>
#include <chrono>
#include <memory>

#include "Thread.h"
#include "WonderOscSender.h"
#include "OscSenderThread.h"
#include "ServerThread.h"
#include "MessageWithPath.h"

#define MSG_WAIT_INTERVAL 1 /**< How long the thread will block before
                                checking for isStopped again (in ms) */

namespace wonderlo {
    
/**
 *  A thread implementing the wonder::OscSenderThread interface.
 *  Messages to be sent are added to an internal delivery queue (guarded by
 *  a mutex) from where the thread will pick them up and send them.
 *
 *  Calls from WonderOscSender to the two wonderSend methods (which are
 *  reimplemented here) will enque fully assembled MessageWithPath objects
 *  in the internal queue.
 */
class WonderOscSenderThread :   public thread::Thread,
                                public wonder::OscSenderThread,
                                public WonderOscSender
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
    WonderOscSenderThread(const std::string &host, const std::string &port,
                          int proto = LO_UDP);
    
    /** Destructor. */
    virtual ~WonderOscSenderThread();
    
    void start();
    
    void stop();
    
    void join();
    
protected:
    /** Within its run method, this thread will continously wait for the internal
     *  message queue to be filled and the send any messages that are added to the
     *  queue.
     *  To ensure that the thread will exit when stop is called, the waiting for
     *  incoming messages will be interrupted every MSG_WAIT_INTERVAL milliseconds.
     *  (Internally, this is done by calling wait_for on the queue's
     *  std::condition_variable.)
     */
    void run();
    
    
    /** This method is used by this thread within its run method to send the
     *  MessageWithPath objects it picks up from the message queue. By overriding this
     *  method, the way the thread sends out those messages can be changed.
     *  This default implementation simply calls lowrappers::Address::send(msg.path, msg).
     *
     *  @param msg The message to be sent.
     */
    void sendMethod(const MessageWithPath& msg);
    
    /** Enqueues a message into the internal message delivery queue.
     *  
     *  @param msg the message to be enqued. Will be deleted by the sending thread.
     */
    void enqueMessage(const MessageWithPath* msg);
    
    /** Overriding VSSender::wonderSend, this changes the way OSC
     *  messages are sent from the various methods inherited from
     *  wonder::OscSender.
     *  This reimplementation simply enques a copy of the message in
     *  an internal message queue from where the sending thread will
     *  pick them up and send them.
     *  
     *  @param msg The Message to be sent.
     *
     *  @see lowrappers::Address::sendFrom, WonderOscSender::wonderSend
     */
    void wonderSend(const MessageWithPath &msg);
    
private:
    std::mutex qMutex_;                         // guards the msg-queue
    std::condition_variable qCondition_;
    std::deque<const MessageWithPath*> msgQ_;   // the message-queue
};
    
}



#endif  // WONDEROSCSENDERTHREAD_H_INCLUDED
