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


#include "ServerThread.h"

namespace lowrappers {

ServerThread::ServerThread(lo_err_handler err_h):
    server(lo_server_new(NULL,  err_h))
{
}

ServerThread::ServerThread(const std::string &port, lo_err_handler err_h):
    server(lo_server_new(port.c_str(),  err_h))
{
}
    
ServerThread::ServerThread(const std::string& group, const std::string &port,
                           lo_err_handler err_h):
    server(lo_server_new_multicast(group.c_str(), port.c_str(),  err_h))
{
}
    
ServerThread::ServerThread(const std::string& group, const std::string &port,
                           const std::string& ifaceIp, lo_err_handler err_h):
    server(lo_server_new_multicast_iface(group.c_str(), port.c_str(),
                                         NULL, ifaceIp.c_str(), err_h))
{
}
    
ServerThread::~ServerThread()
{
    lo_server_free(server);
}

bool ServerThread::isValid() const
{
    return server != NULL;
}

std::string ServerThread::url() const
{
    char* c_url = lo_server_get_url(server);
    std::string url(c_url ? c_url : "Error getting url info");
    if(c_url) free(c_url);
    return url;
}

int ServerThread::port() const
{
    return lo_server_get_port(server);
}

bool ServerThread::addMethod(const char *path, const char *typespec,
                          lo_method_handler h, void *user_data)
{
    if(!isValid()){
        return false;
    } else {
        lo_method rv = lo_server_add_method (server, path, typespec, h, user_data);
        return rv != NULL;
    }
}
    
bool ServerThread::addListener(const char *path, const char *typespec,
                               Listener *listener)
{
    if(!isValid()){
        return false;
    } else {
        
        // We pass genericCallback as lo_method_handler and the Listener as user_data -
        // have a look at the code of genericCallback for the rest of the trick...:
        lo_method rv = lo_server_add_method(server, path, typespec, genericCallback, listener);
        
        return rv != NULL;
    }
}

void ServerThread::delMethod(const char *path, const char *typespec)
{
    if (isValid()) {
        lo_server_del_method(server, path, typespec);
    }
}
    
void ServerThread::run()
{
    while(!isStopped()){
        if(isValid()){
            lo_server_recv_noblock(server, SEVER_THREAD_RCV_INTERVAL);
        }
    }
}

int ServerThread::genericCallback(const char *path, const char *types, lo_arg **argv,
                             int argc, lo_message msg, void *user_data)
{
    // user_data is a pointer to a Listener (passed to the server thread via the call
    // of lo_server_thread_add_method within addListener) - so we can cast it:
    Listener* listener = (Listener*) user_data;
    return listener->callback(path, types, argv, argc, msg);
}

}
