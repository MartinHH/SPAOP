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

#ifndef ADDRESS_H_INCLUDED
#define ADDRESS_H_INCLUDED

#include <iostream>
#include <mutex>
#include <stdint.h>
#include "lo/lo.h"
#include "ServerThread.h"
#include "Message.h"

namespace lowrappers {

/**
 *  A wrapper for liblo's lo_address.
 */
class Address
{
    friend class OscServer;
    
public:
    
    /** Constructor.
     *
     *  @param host A valid hostname or ip string (see the liblo documentation for
     *      what exactly is supported).
     *  @param port A valid port number or service name (see the liblo documentation for
     *      what exactly is supported).
     *  @param proto The protocol to use (LO_UDP, LO_TCP or LO_UNIX). See the liblo
     *      documentation for details.
     */
    Address(const std::string &host, const std::string &port,
            int proto=LO_UDP);
    
    /** Constructor.
     *
     *  @param addr The address to sent to.
     *  @param isOwner Specifies who is responsible for freeing the lo_address
     *      addr (by calling lo_address_free). If set to true, this Address object
     *      will call lo_address_free on addr when it is destroyed or when a new
     *      Address is set. If set to false, the responsibility to call
     *      lo_address_free remains with the caller of this constructor.
     */
    Address(lo_address addr, bool isOwner = true);
    
    /** Destructor. Will call lo_address_delete on the lo_address stored internally
     *  if this Address object is the owner of the lo_address.
     */
    virtual ~Address();
    
    /** Copy constructor. Will create a new copy of the lo_address stored within
     *  other, so the new Address will be owner of its lo_address while other's
     *  ownership status will remain unchanged.
     *
     *  @param other The Adress to be copied.
     */
    Address(const Address& other);
    
    /** Updates this Address to send to a new destination. (Internally, a new
     *  lo_adress is created, replacing the old one.)
     *
     *  @param newHost A valid hostname or ip string (see the liblo documentation for
     *      what exactly is supported).
     *  @param newPort A valid port number or service name (see the liblo documentation for
     *      what exactly is supported).
     */
    void setAddress(const std::string &newHost, const std::string &newPort);
    
    /** Returns the time-to-live value for this Address.
     *
     *  @return The time-to-live value for this Address.
     */
    int getTtl() const;

    /** Sets the time-to-live value for this Address. This is required for sending
     *  UDP multicast messages.
     *
     *  @param ttl The time-to-live value for this Address (between 1 and 255).
     */
    void setTtl(uint8_t ttl);
    
    /** Returns the name of the network interface that will be used for sending
     *  to this address. This is relevant for sending multicast messages.
     *
     *  @return The name of the network interface that will be used for sending
     *      to this address. Might be "(null)" if no interface has been set.
     */
    std::string getIface() const;
    
    /** Sets the network interface to use for sending to this address.
     *
     *  @param ip The IP of the local interface where the messages shall be sent
     *      from.
     */
    void setIfaceByIp(const std::string& ip);
    
    /**
     * An URL describing this address.
     *
     * @return An URL describing this address.
     */
    std::string url() const;
    
    /** The hostname of this adress.
     *
     *  @return the hostname of this adress.
     */
    std::string hostname() const;
    
    /** The port/service name of this adress.
     * 
     *  @returns the port/service name of this adress.
     */
    std::string port() const;
    
    /** Sends a OSC formatted message to this address.
     *
     *  @param path The OSC path of the message.
     *  @param types The types of the data items in the message.
     *  @param ... The data values to be transmitted. The types of the arguments
     *       passed here must agree with the types specified in the type parameter.
     *
     *  return -1 on failure.
     */
    int send(const std::string &path, const std::string types, ...) const;
    
    /** Sends a OSC formatted message to this address.
     *
     *  @param path The OSC path of the message.
     *  @param msg The Message to be sent.
     *
     *  @return -1 on failure.
     */
    int send(const std::string &path, const Message& msg) const;
    
    /** Sends a liblo lo_message to this address.
     *
     *  @param path The OSC path of the message.
     *  @param msg The Message to be sent.
     *
     *  @return -1 on failure.
     */
    int send(const std::string &path, const lo_message msg) const;
    
    /** Sends a OSC formatted message to this address. Parameters are passed
     *  as a va_list. This allows other varargs methods to pass on their
     *  arguments.
     *
     *  @param path The OSC path of the message.
     *  @param types The types of the data items in the message.
     *  @param args The data values to be transmitted. The types of the arguments
     *       passed here (and their length) must agree with the types specified
     *       in the type parameter.
     *
     *  @return -1 on failure.
     */
    int sendVAList(const std::string &path, const std::string &types, va_list args) const;

    /** Sends a OSC formatted message to this address, from the same socket
     *  as the specified server.
     *
     *  @param from The server to send the message from.
     *  @param path The OSC path of the message.
     *  @param types The types of the data items in the message.
     *  @param ... The data values to be transmitted. The types of the arguments
     *       passed here must agree with the types specified in the type parameter.
     *
     *  @return -1 on failure.
     */
    int sendFrom(const ServerThread &from, const std::string &path,
                 const std::string types, ...) const;
    
    /** Sends a OSC formatted message to this address, from the same socket
     *  as the specified server.
     *
     *  @param from The server to send the message from.
     *  @param path The OSC path of the message.
     *  @param msg The Message to be sent.
     *
     *  @return -1 on failure.
     */
    int sendFrom(const ServerThread &from, const std::string &path,
                 const Message& msg) const;
    
    /** Sends a liblo lo_message to this address, from the same socket
     *  that a specified ServerThread is listening on.
     *
     *  @param from The server to send the message from.
     *  @param path The OSC path of the message.
     *  @param msg The Message to be sent.
     *
     *  @return -1 on failure.
     */
    int sendFrom(const ServerThread &from, const std::string &path,
                 const lo_message msg) const;
    
    /** Sends a OSC formatted message to this address, from the same socket
     *  that a specified ServerThread is listening on. Parameters are passed as
     *  a va_list. This allows other varargs methods to pass on their arguments.
     *
     *  @param from The server to send the message from.
     *  @param path The OSC path of the message.
     *  @param types The types of the data items in the message.
     *  @param args The data values to be transmitted. The types of the arguments
     *       passed here (and their length) must agree with the types specified
     *       in the type parameter.
     *
     *  @return -1 on failure.
     */
    int sendVAListFrom(const ServerThread &from, const std::string &path,
                       const std::string &types, va_list args) const;

private:
    Address &operator= (Address& other);    // assignment not allowed
    
    lo_address addr_;
    mutable std::mutex mutex_;
    bool isOwner_;
};

}
#endif /* defined(ADDRESS_H_INCLUDED) */
