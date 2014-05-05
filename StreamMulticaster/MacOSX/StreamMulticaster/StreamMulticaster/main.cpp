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

#include <iostream>
#include "StreamMulticaster.h"
#include "WonderHeader.h"

int main(int argc, const char * argv[])
{
    
    const std::string streamType = "visual";
    
    wonderlo::StreamMulticaster mc(CWONDER_DEFAULT_IP_STR, CWONDER_DEFAULT_PORT_STR,
                                   VISUAL_MC_GROUP_STR, VISUAL_MC_PORT_STR,
                                   VISUAL_MC_IFACE_IP_STR, MULTICASTER_PORT_STR,
                                   MULTICASTER_STREAM_IN_PORT_STR, streamType);
    
    std::cout << "This is a " << streamType << " stream multicaster:" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Stream data from " << mc.getStreamSourceHost() << ":"
    << mc.getStreamSourcePort();
    std::cout << " is received on port " << mc.getStreamInPort() << std::endl;
    std::cout << "and forwarded to multicast group " << mc.getMulticastGroup() << ":"
    << mc.getMulticastPort() << " on interface " << mc.getMulticastIface() << std::endl;
    std::cout << std::endl;
    
    std::cout << "/WONDER/" << streamType << "/stream/connect messages are accepted on port "
    << mc.getConnectPort() << std::endl;
    std::cout << std::endl;
    
    std::cout << "Press enter to stop" << std::endl;
    std::cin.ignore();
    
    return 0;
}


