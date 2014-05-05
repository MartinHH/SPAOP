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

#ifndef CONNECTIONTIMER_H_INCLUDED
#define CONNECTIONTIMER_H_INCLUDED

namespace wonder {

/** An interface defining a cyclic timer (to be used as a watchdog for connection
 *  timeouts) along with listener and factory interfaces.
 *  The actual timer implementation must be done in a derived class.
 */
class ConnectionTimer
{
public:
    
    /** Destructor. */
    virtual ~ConnectionTimer(){}
    
    /** A listener that must be notified by the timer on timeout. */
    class Listener
    {
    public:
        /** Destructor. */
        virtual ~Listener(){}
        
        /** Must be called by the timer on timeout. */
        virtual void onTimeout() = 0;
    };
    
    /** Sets the listener whose timeout() method must be called
     *  when the timer runs out.
     *
     *  @param listener The listener whose timeout() method must
     *      be called when the timer runs out.
     */
    virtual void setListener(Listener* listener) = 0;
    
    /** Starts the timer. Once started, the timer must repeatedly call its
     *  listener's timeout() method after every timer interval. (A cyclic timer,
     *  not a one-shot timer.)
     *  In case a timer was stopped and restarted, the timer must act as if it was
     *  restarted (meaning the first call to timeout() wil happen after a full
     *  timer interval has passed.
     */
    virtual void start() = 0;
    
    /** Stops the timer. */
    virtual void stop() = 0;
    
    /**
     *  An interface for a factory class that creates ConnectionTimers.
     *  This allows the code within namespace wonder to create new
     *  ConnectionTimers without knowing the actual implementation.
     */
    class Factory
    {
    public:
        /** Destructor. */
        virtual ~Factory(){}
        
        /** Creates a new ConnectionTimer.
         *
         *  @param intervalMs The timer inverval in ms.
         *  @return A new (cyclic) ConnectionTimer with an interval of
         *      intervalMs ms.
         */
        virtual ConnectionTimer* createConnectionTimer(int intervalMs) = 0;
    };
};

}

#endif  // CONNECTIONTIMER_H_INCLUDED
