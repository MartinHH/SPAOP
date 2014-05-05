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


#include "Thread.h"

namespace thread {

Thread::Thread():
    thread_(),     // empty constructor -> not a thread, just a placeholder
    running_(false),
    stopped_(false),
    runningMutex_()
{
}

Thread::~Thread()
{
}

void Thread::start()
{
    // lock the mutex to ensure only one thread will ever pass
    // the check for running_:
    std::lock_guard<std::mutex> lock(runningMutex_);
    if (running_) {
        return;
    }
    running_ = true;
    
    // start the thread:
    thread_ = std::thread(threadFunction, this);
}

void Thread::stop()
{
    stopped_ = true;
}
    
void Thread::join()
{
    thread_.join();
}
    
bool Thread::isStopped() const
{
    return stopped_;
}
    
void Thread::threadFunction(Thread* thisThread)
{
    thisThread->run();
}

}