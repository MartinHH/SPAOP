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

#ifndef PINGCONTROL_H_INCLUDED
#define PINGCONTROL_H_INCLUDED

#include <memory>

#include "ConnectionTimer.h"
#include "PingControl.h"
#include "WonderHeader.h"

#define MAX_PINGS_LOST 5

namespace wonder {

/**
 *  A class for connection timeout control.
 */
class PingControl   :   public ConnectionTimer::Listener
{
public:
    
    /**
     *  A listener class for callbacks in case lost connection is
     *  detected.
     */
    class Listener
    {
    public:
        
        /** Destructor. */
        virtual ~Listener(){}
        
        /** Called when more than maxLost pings did not arrive
         *  as expected.
         *
         *  @param connectionID The ID that was passed in the
         *      PingControl's constructor. This enables the listener
         *      to listen to several instances of PingControl.
         */
        virtual void connectionLost(const int connectionID) = 0;
    };

    /** Constructor.
     *
     *  @param timerFactory A ConnectionTimer::Factory that the constructor
     *      will use to create the PingControl's internal ConncectionTimer.
     *  @param listener The Listener that will be notified when a lost
     *      connection is detected.
     *  @param connectionID An ID that will be passed to the listener in case
     *      the listener's connectionLost method is called.
     *  @param maxLost How many pings may be lost before the connection is
     *      considered to be lost (and the listener's connectionLost method
     *      is called).
     */
    PingControl(ConnectionTimer::Factory* timerFactory,
                Listener* listener, const int connectionID,
                const int maxLost=MAX_PINGS_LOST);
    
    /** Destructor. */
    virtual ~PingControl();
    
    /** Starts the ping control, resetting the lost pings count. The onPing() method
     *  will start ping control as well (without resetting the lost pings count). If
     *  it is uncertain if the counterpart will send pings at all, consider not calling
     *  start() and rely on onPing() to start PingControl.
     *
     *  @see onPing().
     */
    void start();
    
    /** Stops the ping control. Be aware that further calls of onPing() will restart
     *  PingControl.
     *
     *  @see onPing().
     */
    void stop();
    
    /** Called when an incoming ping is received. (Depending on how this class is
     *  used, the incoming ping could also be a "pong".)
     *  If this PingControl is not started yet, a call to this method will also
     *  start PingControl.
     */
    void onPing();
    
private:
    
    PingControl();
    PingControl(const PingControl& other);
    PingControl &operator= (const PingControl& other);
    
    std::unique_ptr<ConnectionTimer> timer_;
    Listener* listener_;
    const int connectionID_;
    const int maxLost_;
    int lostCount_;
    
    
    //==============================================================================
    //vvvvv  Implementing ConnectionTimer::Listener
    
    void onTimeout();
};

}

#endif  // PINGCONTROL_H_INCLUDED
