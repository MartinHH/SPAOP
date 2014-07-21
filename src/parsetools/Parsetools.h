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

#ifndef PARSETOOLS_H_INCLUDED
#define PARSETOOLS_H_INCLUDED

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#ifdef _WIN32
#include <Ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif

#define PORT_NO_MIN 1024    /**< Lower bound of "legal" port numbers. */
#define PORT_NO_MAX 65535   /**< Upper bound of "legal" port numbers. */

/**
 *  A namespace for tools for manipulation and validation of user input
 *  strings.
 *
 *  @warning The functions contained in this namespace are not designed
 *      for efficiency. If you intend to use them more frequently than
 *      for just a few user inputs, think twice.
 */
namespace parsetools {
    
/** Splits a string around the matches of a given delimiter char.
 *
 *  @param s The string to be split.
 *  @param delim The delimiter used for splitting.
 *
 *  @return A vector containing the resulting substrings.
 */
std::vector<const std::string> splitString(const std::string& s, char delim);
    
/** Checks whether a given string represents a valid IP(v4). (Internally,
 *  inet_pton is called and checked for its return code.)
 *
 *  @param ip The string to be checked.
 *
 *  @return true, if the input string represents a valid IPv4 IP.
 */
bool isValidIP(const std::string& ip);

/** Checks whether a given string represents a valid user port number. The
 *  well-known ports (0-1023) are considered to be invalid, so valid port
 *  numbers range from 1024 to 65535.
 *
 *  @param port The string to be checked.
 *
 *  @return true, if the input string represents a number between 1024 and
 *      65535.
 */
bool isValidPort(const std::string& port);
    
}



#endif  // PARSETOOLS_H_INCLUDED
