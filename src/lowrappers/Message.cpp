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


#include "Message.h"

namespace lowrappers {

Message::Message():
    msg_(lo_message_new())
{
}
    
Message::~Message()
{
    lo_message_free(msg_);
}

Message::Message(const Message& other):
    msg_(lo_message_clone(other.msg_))
{
}
    
void Message::add(const std::string types, ...)
{
    va_list args;
    va_start(args, types);  // Be careful: 'va_start' has undefined behavior
                            // with reference types. While passing types as
                            // const std::string &type worked fine under OSX,
                            // it messed up everything under Windows (Visual
                            // Studio 2013) - so do not change it back!
    
    addVAList(types, args);
}

void Message::addVAList(const std::string &types, va_list args)
{
    // type strings ending in '$$' indicate not to perform
    // LO_MARKER checking (see message.c of liblo)
    std::string t = types + "$$";
    lo_message_add_varargs(msg_, t.c_str(), args);
}
    
}
