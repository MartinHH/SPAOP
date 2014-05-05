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

#include "WonderOscServerSenderThread.h"

namespace wonderlo {
    
WonderOscServerSenderThread::WonderOscServerSenderThread(const std::string &host,
                                                         const std::string &port,
                                                         lowrappers::ServerThread &server,
                                                         int proto):
    WonderOscSenderThread(host, port, proto),
    server_(server)
{
}
    
WonderOscServerSenderThread::~WonderOscServerSenderThread()
{
}
    
void WonderOscServerSenderThread::sendMethod(const wonderlo::MessageWithPath &msg)
{
    sendFrom(server_, msg.path(), msg);
}

    
}