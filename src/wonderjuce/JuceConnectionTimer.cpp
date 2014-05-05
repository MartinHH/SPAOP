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

#include "JuceConnectionTimer.h"

namespace wonderjuce {
    
wonder::ConnectionTimer::Factory* JuceConnectionTimer::getFactory()
{
    static JuceConnectionTimer::Factory f; // thread-safe in C++11
    return &f;
}

JuceConnectionTimer::JuceConnectionTimer(int interval):
listener_(nullptr), interval_(interval)
{
}

JuceConnectionTimer::~JuceConnectionTimer()
{
}

void JuceConnectionTimer::setListener(Listener *listener)
{
    listener_ = listener;
}

void JuceConnectionTimer::start()
{
    startTimer(interval_);
}

void JuceConnectionTimer::stop()
{
    stopTimer();
}

void JuceConnectionTimer::timerCallback()
{
    if (listener_ != nullptr) {
        listener_->onTimeout();
    }
}
    
wonder::ConnectionTimer* JuceConnectionTimer::Factory::createConnectionTimer(int intervalMs)
{
    return new JuceConnectionTimer(intervalMs);
}
    
}
