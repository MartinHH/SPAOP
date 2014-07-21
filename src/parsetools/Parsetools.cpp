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

#include "Parsetools.h"

namespace parsetools {
    
std::vector<const std::string> splitString(const std::string &s, char delim)
{
    std::stringstream ss(s);
    std::string str;
    std::vector<const std::string> resu;
    
    while (std::getline(ss, str, delim)) {
        resu.push_back(str);
    }
    
    return resu;
}
    
bool isValidIP(const std::string& ip)
{
    struct in_addr addr;
    int rv = inet_pton(AF_INET, ip.c_str(), &addr);
    return rv == 1;
}
    
bool isValidPort(const std::string& port)
{
    std::stringstream ss(port);
    int p;
    
    if(!(ss>>p)){
        return false;
    }
    
    std::string remainder;
    if((ss>>remainder)){
        return false;
    }
    
    std::cout << p << std::endl;
    return p >= PORT_NO_MIN && p <= PORT_NO_MAX;
}
    
}