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

#ifndef OSCSENDERTHREAD_H_INCLUDED
#define OSCSENDERTHREAD_H_INCLUDED

#include "OscSender.h"

namespace wonder {

/**
 *  An interface for a thread implementing the OscSender interface.
 *  An OscSenderThread object accepts calls to the OscSender methods and will
 *  send the corresponding OscMessages from its own thread.
 *
 *  @see wonderlo::WonderOscSender and wonderlo::WonderOscServerSenderThread
 *      for liblo-based implementations of this interface.
 *
 *  @see VisualStreamReceiver::createSenderThread for an implementation-independent
 *      way to create instances of this interface.
 */
class OscSenderThread   :   public virtual OscSender
{
public:
    
    /** Starts the thread. */
    virtual void start() = 0;
    
    /** Stops the thread. */
    virtual void stop() = 0;
    
    /** Joins the thread. */
    virtual void join() = 0;
};

}

#endif  // OSCSENDERTHREAD_H_INCLUDED
