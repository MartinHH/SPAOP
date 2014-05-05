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

#include "PingControl.h"

namespace wonder {

PingControl::PingControl(ConnectionTimer::Factory* timerFactory,
                         PingControl::Listener* listener,
                         const int connectionID,
                         const int maxLost):
    timer_(timerFactory->createConnectionTimer(PING_TIMEOUT_INTERVAL)),
    listener_(listener),
    connectionID_(connectionID),
    maxLost_(maxLost),
    lostCount_(0)
{
    timer_->setListener(this);
}
    
PingControl::~PingControl()
{
}

void PingControl::start()
{
    lostCount_ = 0;
    timer_->start();
}
    
void PingControl::stop()
{
    lostCount_ = 0;
    timer_->stop();
}
    
void PingControl::onPing()
{
    timer_->stop();
    timer_->start();
}
    
//==============================================================================
//vvvvv  Implementing ConnectionTimer::Listener
    
void PingControl::onTimeout()
{
    lostCount_++;
    if(lostCount_ > maxLost_){
        timer_->stop();
        listener_->connectionLost(connectionID_);
    }
}

    
}