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


#ifndef GENERICSERVERTHREADLISTENER_H_INCLUDED
#define GENERICSERVERTHREADLISTENER_H_INCLUDED

#include "ServerThread.h"

namespace lowrappers {
    
/**
 *  A generic functor class for the ServerThread::Listener interface.
 *
 *  @see lowrappers::ListenerMaker for a convenient way to use it.
 */
template<class T>
class GenericServerThreadListener   :   public ServerThread::Listener
{
public:
    /** The type of function pointer that can be passed to the 
     *  constructor.
     */
    typedef int (T::*fpType)(const char *path, const char *types,
                             lo_arg **argv, int argc, lo_message msg);
    
    /** Constructor. */
    GenericServerThreadListener(T& object, fpType fp) :
        object_(object), fp_(fp)
    {}

    /** Destructor. */
    virtual ~GenericServerThreadListener(){}
    
    virtual int callback(const char *path, const char *types,
                       lo_arg **argv, int argc, lo_message msg)
    {
        return (object_.*fp_)(path, types, argv, argc, msg);
    }
    
private:
    T& object_;
    fpType fp_;
};

/** A function that conveniently wraps up a GenericServerThreadListener.
 *
 *  For example, let's say you have this class:
 *  \code{.cpp}
 *  class Foo
 *  {
 *      // ...
 *      int bar (const char *path, const char *types,lo_arg **argv, int argc, lo_message msg);
 *      // ...
 *  };
 *  \endcode
 *  You can now create a ServerThread::Listener whose callback method calls a Foo
 *  instance's bar method like this:
 *  \code{.cpp}
 *  Foo* foo = new Foo();
 *  ServerThread::Listener* listener = ListenerMaker(foo, &Foo::bar);
 *  \endcode
 *
 *  Keep in mind that this creates a (tiny) object that must be deleted when its
 *  not needed anymore!
 *
 *  @param object The object whose method shall be called as a callback
 *      from a ServerThread.
 *  @param fp A function pointer pointing to the method of object that
 *      shall be called from a ServerThread. Must match the signature
 *      of ServerThread::Listener::callback, i.e. return int and accept
 *      parameters (const char*, const char*, lo_arg**, int, lo_message).
 *  @return A ServerThread::Listener whose callback method will call
 *      object's fp method.
 */
template<class T>
ServerThread::Listener* ListenerMaker(T& object, int (T::*fp)(const char *path,
                const char *types,lo_arg **argv, int argc, lo_message msg))
{
    return new GenericServerThreadListener<T>(object, fp);
}
    
}

#endif  // GENERICSERVERTHREADLISTENER_H_INCLUDED
