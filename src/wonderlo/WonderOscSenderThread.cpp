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

#include "WonderOscSenderThread.h"

namespace wonderlo {
    
WonderOscSenderThread::WonderOscSenderThread(const std::string &host,
                                             const std::string &port,
                                             int proto):
    WonderOscSender(host, port, proto),
    qMutex_(),
    qCondition_(),
    msgQ_()
{
}
    
WonderOscSenderThread::~WonderOscSenderThread()
{
    // empty the queue (just in case...):
    qMutex_.lock();
    while(!msgQ_.empty()){
        const MessageWithPath* msg = msgQ_.front();
        delete msg;
        msgQ_.pop_front();
    }
    qMutex_.unlock();
}

void WonderOscSenderThread::start()
{
    Thread::start();
}
    
void WonderOscSenderThread::stop()
{
    Thread::stop();
}
 
void WonderOscSenderThread::join()
{
    Thread::join();
}
    
void WonderOscSenderThread::run()
{
    while(!isStopped()){
        
        std::unique_ptr<const MessageWithPath> msg(nullptr);
        
        std::unique_lock<std::mutex> lock(qMutex_); // lock the mutex
        
        while (!isStopped() && msgQ_.empty()) {     // wait
            qCondition_.wait_for(lock, std::chrono::milliseconds(MSG_WAIT_INTERVAL));
        }
        
        if (!msgQ_.empty()) {   // take the first element and remove it:
            msg = std::unique_ptr<const MessageWithPath>(msgQ_.front());
            msgQ_.pop_front();
        }
        
        lock.unlock();          // unlock the mutex
        
        if(msg != nullptr){     // send the message
            sendMethod(*msg);
        }
        
        // msg will now be deleted by scope...
    }
    
}

void WonderOscSenderThread::sendMethod(const wonderlo::MessageWithPath& msg)
{
    send(msg.path(), msg);
}
    
void WonderOscSenderThread::enqueMessage(const MessageWithPath* msg)
{
    // aquire mutex & add msg:
    qMutex_.lock();
    msgQ_.push_back(msg);
    qMutex_.unlock();
    
    // notify the sender-thread:
    qCondition_.notify_one();
}
    
void WonderOscSenderThread::wonderSend(const MessageWithPath &msg)
{
    // a copy is needed as msg will be deleted right after the call:
    const MessageWithPath* msgCopy = new MessageWithPath(msg);
    
    enqueMessage(msgCopy);
}
    
}