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


#ifndef SERVERTHREAD_H_INCLUDED
#define SERVERTHREAD_H_INCLUDED

#include <iostream>
#include "lo/lo.h"
#include "Thread.h"

#define SEVER_THREAD_RCV_INTERVAL 10 /**< How long the thread will be receive-
                                        blocked before checking for isStopped()
                                        again (in ms) */

/**
 * A namespace for C++ wrappers for the Liblo library. Liblo is a library
 * for sending and receiving OSC messages.
 *
 */
namespace lowrappers {

/**
 * A server thread with Liblo-style callback capabilities. This is NOT a
 * wrapper for Liblo's lo_server_thread because the lo_server_thread relies
 * on the pthread library. For better compability with Windows, this class
 * uses the thread::Thread that relies on std::thread (the tradeoff for this
 * is: C++11 is now required).
 *
 * @warning Differently from the lo_server_thread where a call to
 *      lo_server_thread will automatically join the thread, stop() and
 *      join() both need to be called for a clean shutdown.
 */
class ServerThread  :   public thread::Thread
{
    /** Address is friend class so it can access the internal lo_server
     *  for sending messages from its port.
     */
    friend class Address;
public:
    
    /**
     * A listener interface for listeners to be called by the ServerThread
     * when incoming OSC messages are received.
     */
    class Listener
    {
    public:
        virtual ~Listener(){}
        
        /** 
         * The callback method that will be called from the ServerThread.
         *
         * @param path The OSC path of the incoming message that triggered
         *      the call.
         * @param types The Liblo-style types string indicating the types
         *      of the recieved parameters (e.g. "iii" for three int
         *      parameters).
         * @param argv The parameters recieved within the message (whose
         *      types will match the types string.
         * @param argc The length of the argv array.
         * @param msg The full message as lo_message.
         * @return 0 if the message has been handled and shall not be passed
         *      to other callback methods, non-zero if the server shall try
         *      passing the message to further matching callback methods.
         */
        virtual int callback(const char *path, const char *types,
                             lo_arg **argv, int argc, lo_message msg) = 0;
    };
    
    /**
     * Constructor. An unused port will be chosen by the system, its number
     * may be retrieved with ServerThread::port so it can be passed to clients.
     *
     * @param err_h A function that will be called in the event of an error
     *      being raised. The function prototype is defined in lo_types.h
     */
    ServerThread(lo_err_handler err_h);
    
    /**
     * Constructor.
     *
     * @param port A decimal port number, service name or UNIX domain socket
     *      path may be passed.
     * @param err_h A function that will be called in the event of an error
     *      being raised. The function prototype is defined in lo_types.h
     */
    ServerThread(const std::string &port, lo_err_handler err_h);
    
    /**
     * Constructor. Creates a ServerThread that joins a specified multicast
     * group.
     *
     * @param group The multicast group to join.
     * @param port A decimal port number, service name or UNIX domain socket
     *      path may be passed.
     * @param err_h A function that will be called in the event of an error
     *      being raised. The function prototype is defined in lo_types.h
     */
    ServerThread(const std::string& group, const std::string &port, lo_err_handler err_h);
    
    /**
     * Constructor. Creates a ServerThread that joins a specified multicast
     * group, listening on a specified interface.
     *
     * @param group The multicast group to join.
     * @param port A decimal port number, service name or UNIX domain socket
     *      path may be passed. Note that differently from lo_server_tread_new,
     *      passing NULL is not allowed.
     * @param ifaceIp A string specifying the network interface to use by its IP.
     * @param err_h A function that will be called in the event of an error
     *      being raised. The function prototype is defined in lo_types.h
     */
    ServerThread(const std::string& group, const std::string &port,
                 const std::string& ifaceIp, lo_err_handler err_h);
    
    /** Destructor. */
    virtual ~ServerThread();
    
    /**
     * Checks if initialization was successfull.
     *
     * @returns true, if this is a valid ServerThread.
     */
    bool isValid() const;
    
    /**
     * An URL describing the address of the server thread.
     *
     * @returns An URL describing the address of the server thread.
     */
    std::string url() const;
    
    /**
     * The port number that the server thread has bound to.
     *
     * @return The port number that the server thread has bound to.
     */
    int port() const;
    
    /**
     * Adds a lo_method_handler to this server thread. See the Liblo
     * documentation for details about lo_method_handlers.
     * For an object-oriented alternative, see addListener.
     *
     * @param path The OSC path to register the method to. If NULL
     *      is passed the method will match all paths.
     * @param typespec The typespec the method accepts. Incoming
     *      messages with similar typespecs (e.g. ones with numerical
     *      types in the same position) will be coerced to the typespec
     *      given here.
     * @param h The lo_method_handler callback function that will be
     *      called if a matching message is received.
     * @param user_data A value that will be passed to the callback
     *      function, h, when its invoked matching from this method.
     *
     * @return true, if the method handler was added successfully,
     *      false otherwise.
     */
    bool addMethod(const char *path, const char *typespec,
                   lo_method_handler h, void *user_data);
    
    /**
     * Adds a Listener to this server thread.
     *
     * (Note that there is a functor class GenericServerThreadListener
     * so any object's method that matches the signature of
     * ServerThread::Listener::callback can be passed.)
     *
     * @param path The OSC path to register the method to. If NULL
     *      is passed the method will match all paths.
     * @param typespec The typespec the method accepts. Incoming
     *      messages with similar typespecs (e.g. ones with numerical
     *      types in the same position) will be coerced to the typespec
     *      given here.
     * @param listener The Listener whose callback() method will be
     *      called if a matching message is received.
     *
     * @return true, if the method Listener was added successfully,
     *      false otherwise.
     */
    bool addListener(const char *path, const char *typespec,
                     Listener* listener);
    
    /**
     * Delete a method from this server thread. This may aso be used
     * to delete Listener callback methods.
     *
     * @param path The OSC path of the method to delete.
     * @param typespec The typespec the method accepts.
     */
    void delMethod(const char *path, const char *typespec);
    
private:
    ServerThread(const ServerThread& other);              // copying not allowed
    ServerThread &operator= (const ServerThread other);   // assignment not allowed
    
    /** Static lo_method_hander that is used to add a Listener instance's callback
     *  method to a lo_server_thread. It shall only be called from the public
     *  addListener method.
     */
    static int genericCallback(const char *path, const char *types, lo_arg **argv,
                             int argc, lo_message msg, void *user_data);
    
    void run(); // inherited from thread::Thread
    
    lo_server server;
};
    
}

#endif  // SERVERTHREAD_H_INCLUDED
