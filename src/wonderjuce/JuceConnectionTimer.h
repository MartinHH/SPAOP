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

#ifndef JUCECONNECTIONTIMER_H_INCLUDED
#define JUCECONNECTIONTIMER_H_INCLUDED

#include "ConnectionTimer.h"
#include "JuceHeader.h"

/**
 *  A namespace for (reusable) WONDER-related classes that are based on the
 *  JUCE library.
 */
namespace wonderjuce {

/**
 *  An implementation of wonder::ConnectionTimer using the Timer class of JUCE.
 */
class JuceConnectionTimer   :   public Timer,
                                public wonder::ConnectionTimer
{
public:
    static wonder::ConnectionTimer::Factory* getFactory();
    
    JuceConnectionTimer(int intervalMs);
    ~JuceConnectionTimer();
    
    void setListener(Listener* listener);
    
    void start();
    
    void stop();
    
    void timerCallback();
    
private:
    class Factory   :   public wonder::ConnectionTimer::Factory
    {
        ConnectionTimer* createConnectionTimer(int intervalMs);
    };
    
    Listener* listener_;
    const int interval_;
};

}

#endif  // JUCECONNECTIONTIMER_H_INCLUDED
