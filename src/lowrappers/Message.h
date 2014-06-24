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


#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED

#include <iostream>
#include "lo/lo.h"

namespace lowrappers {

/**
 *  A wrapper of Liblo's lo_message. These messages can be sent via Address::send
 *  and Address::sendFrom.
 */
class Message
{
    /** Address is friend class so it can access the internal lo_message object
     *  for sending.
     */
    friend class Address;
public:
    
    /** Constructor. */
    Message();
    
    /** Destructor. */
    virtual ~Message();
    
    /** Copy constructor. Creates a copy of this message. (Internally, 
     *  lo_message_clone is called.)
     *
     *  @param other The message to be copied.
     */
    Message(const Message& other);

    /** Adds content to the message. Number and types of arguments must match
     *  the types string, e.g. add("iff", 1, 2.0, 3.0) or add(s, "some string").
     *
     *  @param types A Liblo-style typestring.
     *  @param ... The content to be added to the message. Number
     *      and types of arguments must match the types string.
     */
    void add(const std::string types, ...);
    
    /** Adds content to the message. Number and types of arguments must match
     *  the types string, e.g. add("iff", 1, 2.0, 3.0) or add(s, "some string").
     *  Parameters are passed as a va_list. This allows other varargs methods
     *  to pass on their arguments.

     *  @param types A Liblo-style typestring
     *  @param args The data values to be transmitted. The types of the arguments
     *      passed here (and their length) must agree with the types specified
     *      in the type parameter.
     */
    void addVAList(const std::string &types, va_list args);
    
private:
    Message &operator= (const Message other);   // assignment not allowed
    
    lo_message msg_;
};

}

#endif  // MESSAGE_H_INCLUDED
