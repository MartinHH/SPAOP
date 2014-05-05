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

#include "Address.h"

namespace lowrappers {

Address::Address(const std::string &host, const std::string &port,
                 int proto):
    addr_(lo_address_new_with_proto(proto, host.c_str(), port.c_str())),
    mutex_(),
    isOwner_(true)
{
}

Address::Address(lo_address addr, bool isOwner):
    addr_(addr),
    mutex_(),
    isOwner_(isOwner)
{
}
    
Address::~Address()
{
    if(isOwner_ && addr_){
        lo_address_free(addr_);
    }
}

Address::Address(const Address& other):
    addr_(lo_address_new_with_proto(lo_address_get_protocol(other.addr_),
                                    lo_address_get_port(other.addr_),
                                    lo_address_get_hostname(other.addr_))),
    mutex_(),
    isOwner_(true)
{
}
    
void Address::setAddress(const std::string &newHost, const std::string &newPort){
    
    const int proto = lo_address_get_protocol(addr_);
    lo_address newAddr = lo_address_new_with_proto(proto, newHost.c_str(), newPort.c_str());
    
    std::lock_guard<std::mutex> lock(mutex_);
    
    lo_address oldAddr = addr_;
    lo_address_set_ttl(newAddr, lo_address_get_ttl(oldAddr));
    addr_ = newAddr;
    
    if(isOwner_ && oldAddr){
        lo_address_free(oldAddr);
    }
}
    
int Address::getTtl() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return lo_address_get_ttl(addr_);
}

void Address::setTtl(uint8_t ttl)
{
    std::lock_guard<std::mutex> lock(mutex_);
    lo_address_set_ttl(addr_, ttl);
}
   
std::string Address::getIface() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return std::string(lo_address_get_iface(addr_));
}
    
void Address::setIfaceByIp(const std::string &ip)
{
    std::lock_guard<std::mutex> lock(mutex_);
    lo_address_set_iface(addr_, NULL, ip.c_str());
}
    
std::string Address::url() const{
    mutex_.lock();
    char* c_url = lo_address_get_url(addr_);
    mutex_.unlock();
    std::string url(c_url ? c_url : "Error getting url info");
    if(c_url) free(c_url);
    return url;
}
    
std::string Address::hostname() const
{
    mutex_.lock();
    const char* c_host = lo_address_get_hostname(addr_);
    mutex_.unlock();
    
    std::string host(c_host ? c_host : "Error getting hostname");
    return host;
}
    
std::string Address::port() const
{
    mutex_.lock();
    const char* c_port = lo_address_get_port(addr_);
    mutex_.unlock();
    
    std::string port(c_port ? c_port : "Error getting port");
    return port;
}
    
int Address::send(const std::string &path, const std::string types, ...) const
{
    va_list args;
    va_start(args, types);  // Be careful: 'va_start' has undefined behavior
                            // with reference types. While passing type as
                            // const std::string &type worked fine under OSX,
                            // it messed up everything under Windows (Visual
                            // Studio 2013) - so do not change it back!

    return sendVAList(path, types, args);
}
    
int Address::sendVAList(const std::string &path, const std::string &type, va_list args) const
{
    lo_message m = lo_message_new();
    
    // "$$" at the end of the typestring tells lo_message_add_varargs()
    // not to use LO_MARKER checking at the end of the argument list:
    std::string t = type + "$$";
    lo_message_add_varargs(m, t.c_str(), args);
    
    mutex_.lock();
    int rv = lo_send_message(addr_, path.c_str(), m);
    mutex_.unlock();
    
    lo_message_free(m);
    return rv;
}
    
int Address::send(const std::string &path, const Message &msg) const
{
    return send(path, msg.msg_);
}
    
int Address::send(const std::string &path, const lo_message msg) const
{
    mutex_.lock();
    int rv = lo_send_message(addr_, path.c_str(), msg);
    mutex_.unlock();
    
    return rv;
}
    
int Address::sendFrom(const ServerThread &from, const std::string &path,
                        const std::string types, ...) const
{
    va_list args;
    va_start(args, types);
    
    return sendVAListFrom(from, path, types, args);

}
    
int Address::sendFrom(const ServerThread &from, const std::string &path, const Message &msg) const
{
    return sendFrom(from, path, msg.msg_);
}
    
int Address::sendFrom(const lowrappers::ServerThread &from, const std::string &path,
                      const lo_message msg) const
{
    mutex_.lock();
    int rv = lo_send_message_from(addr_,from.server, path.c_str(), msg);
    mutex_.unlock();
    
    return rv;
}
    
int Address::sendVAListFrom(const ServerThread &from, const std::string &path,
                            const std::string &types, va_list args) const
{
    lo_message m = lo_message_new();
    
    // type strings ending in '$$' indicate not to perform
    // LO_MARKER checking (see message.c of liblo)
    std::string t = types + "$$";
    lo_message_add_varargs(m, t.c_str(), args);
    
    mutex_.lock();
    int rv = lo_send_message_from(addr_, from.server,
                                  path.c_str(), m);
    mutex_.unlock();
    
    lo_message_free(m);
    return rv;
}
    
}