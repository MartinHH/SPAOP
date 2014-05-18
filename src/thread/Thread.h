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


#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED

#include <thread>
#include <mutex>

/**
 *  A namespace for C++ wrappers of basic multithreading objects. Since C++11,
 *  most of that is already available in std, so look there before you look
 *  here.
 */
namespace thread {

/**
 *  This class wraps an std::thread into a Java-style thread object.
 *  Derived classes override the abstract run() method which will be exectued
 *  by the thread represented by this class.
 */
class Thread
{
public:
    
    /** Constructor. */
    Thread();
    
    /** Destructor. */
    virtual ~Thread();
    
    /** Starts the Thread.
     *
     *  Calling this again the thread has been started already will have
     *  no effect. (That includes calling start() again after the thread
     *  has been joined.)
     */
    void start();
    
    /** Sets the internal flag stopped_ to true. */
    void stop();
    
    /** Joins the thread. */
    void join();
    
protected:

    /** This is the method that will be executed by the thread. In case
     *  this is implemented with a loop, isStopped() should be checked
     *  reguarly (and the loop shall be exited if isStopped() returns
     *  true).
     */
    virtual void run()=0;

    /** Returns true if the thread was stopped. Must be checked regularly
     *  from within run().
     */
    bool isStopped() const;
    
private:
    
    /** The function that is passed to the std::thread thread_. Calls
     *  thisThread->run().
     *
     *  @param thisThread Must be this Thread object.
     */
    static void threadFunction(Thread* thisThread);
    
    Thread(const Thread& other);        // copying not allowed
    
    std::thread thread_;                // the thread
    bool running_;                      // flag: is running
    bool stopped_;                      // flag: is stopped
    mutable std::mutex runningMutex_;   // guards the running_ flag
};

}

#endif  // THREAD_H_INCLUDED
