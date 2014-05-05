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

#include "WonderOscServerSender.h"

namespace wonderlo {

WonderOscServerSender::WonderOscServerSender(const std::string &host, const std::string &port,
                               lowrappers::ServerThread &server, int proto):
    WonderOscSender(host, port, proto),
    server_(server)
{
}

WonderOscServerSender::WonderOscServerSender(lo_address addr, lowrappers::ServerThread &server,
                                             bool isOwner):
    WonderOscSender(addr, isOwner),
    server_(server)
{
}

WonderOscServerSender::WonderOscServerSender(const WonderOscServerSender& other):
    WonderOscSender(other),
    server_(other.server_)
{
}

WonderOscServerSender::~WonderOscServerSender()
{
}

void WonderOscServerSender::wonderSend(const MessageWithPath &msg)
{
    sendFrom(server_, msg.path(), msg);
}

}