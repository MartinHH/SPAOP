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

#ifndef MESSAGEWITHPATH_H_INCLUDED
#define MESSAGEWITHPATH_H_INCLUDED

#include "Message.h"

namespace wonderlo {

/**
 *  A lowrappers::Message object that holds additional information about its
 *  destination OSC path.
 */
class MessageWithPath   :   public lowrappers::Message
{
public:
    /** Constructor.
     *  
     *  @param path The OSC path to which this message shall
     *      be sent.
     */
    MessageWithPath(const std::string& path);
    
    /** Destructor. */
    virtual ~MessageWithPath();
    
    /** Copy constructor. Creates a copy of this message. */
    MessageWithPath(const MessageWithPath& other);
  
    /** Returns the OSC path to which this message shall be sent.
     *  
     *  @return The OSC path to which this message shall be sent.
     */
    const std::string& path() const;
    
private:
    const std::string path_;
};

}

#endif  // MESSAGEWITHPATH_H_INCLUDED
