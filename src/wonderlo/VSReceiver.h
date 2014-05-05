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

#ifndef VSRECEIVER_H_INCLUDED
#define VSRECEIVER_H_INCLUDED

#include <iostream>
#include <list>
#include <memory>

#include "VisualStreamReceiver.h"
#include "WonderOscServerSender.h"
#include "WonderOscServerSenderThread.h"
#include "ServerThread.h"
#include "GenericServerThreadListener.h"

/**
 * A namespace for liblo-based implementations of the OSC communication
 * interfaces defined in namespace wonder.
 */
namespace wonderlo {

/**
 * An OscServer-Thread that calls the message-handling methods of a
 * VisualStreamCallInterface object.
 */
class VSReceiver    :   public lowrappers::ServerThread,
                        public wonder::VisualStreamReceiver
{
    
public:
    
    /** A singleton-getter for a wonder::VisualStreamReceiver::Factory
     *  that creates instances of VSReceiver.
     */
    static wonder::VisualStreamReceiver::Factory* getFactory();
    
    /** Constructor. An unused port will be chosen by the sytem as the
     *  port this VSReceiver will listen on.
     *  
     *  @param err_h A lo_error_handler that will be passed to the
     *      internal lo_server_thread. See the liblo documentation
     *      for more details.
     */
    VSReceiver(lo_err_handler err_h);
    
    /** Constructor.
     *
     *  @param port The port this VSReceiver will listen on.
     *  @param err_h A lo_error_handler that will be passed to the
     *      internal lo_server_thread. See the liblo documentation
     *      for more details.
     */
    VSReceiver(const std::string &port, lo_err_handler err_h);
    
    /**
     * Constructor. Creates a VSReceiver that joins a specified multicast
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
    VSReceiver(const std::string& group, const std::string &port,
               const std::string& ifaceIp, lo_err_handler err_h);
    
    /** Destructor. */
    virtual ~VSReceiver();
    
    /** Returns an OscSender that sends from the port of this server thread to
     *  the specified destination. Returned here is actually an instance of
     *  VSServerSender.
     *
     *  @param host A valid hostname or ip string (see the liblo documentation for
     *      what exactly is supported).
     *  @param port A validport number or service name (see the liblo documentation for
     *      what exactly is supported).
     *  @return A OscSender that sends from the port of this server thread to
     *      the specified destination. Returned here is actually an instance of
     *      VSServerSender.
     */
    wonder::OscSender* createSender(const std::string &host, const std::string &port);
    
    wonder::OscSenderThread* createSenderThread(const std::string &host,
                                                const std::string &port);
    
    /** Sets the Listener. Only one listener is supported (so far),
     *  and it is not confirmed whether it is possible to change the
     *  listener from one Listener instance to another.
     */
    void setListener(VisualStreamReceiver::Listener* listener);
    
    /** Sets the PingHandler. Only one PingHandler is supported (so far),
     *  and it is not confirmed whether it is possible to change the
     *  PingHandler from one instance to another.
     */
    void setPingHandler(PingHandler* pingHandler);
    
    void start();
    
    void stop();
    
    void join();
    
    int port();
    
private:
    VSReceiver(const VSReceiver& other);                // copying not allowed
    VSReceiver &operator= (const VSReceiver other);     // assignment not allowed

    
    VisualStreamReceiver::Listener* listener_;
    VisualStreamReceiver::PingHandler* pingHandler_;
    std::list<std::unique_ptr<lowrappers::ServerThread::Listener>> functorList_; // avoids memory leaks
    
    /** Factory implementation. */
    class Factory   :   public wonder::VisualStreamReceiver::Factory
    {
        wonder::VisualStreamReceiver* createVisualStreamReceiver();
        
        wonder::VisualStreamReceiver* createVisualStreamReceiver(const std::string& port);
        
        wonder::VisualStreamReceiver* createVisualStreamMulticastReceiver(const std::string& group,
                                                                          const std::string& port,
                                                                          const std::string& iface);
    };
    
    /** Adds the Listener's methods to this ServerThread. */
    void addMethods();
    
    /** Adds a listener and  stores its pointer in the functorList_ so that it will be
     *  deleted by scope on destruction of this object
     */
    void addListenerAndStoreFunctor(const char* path, const char* types,
                                    lowrappers::ServerThread::Listener* listener);
    
    //==============================================================================
    // Wrapper-Methods for wonder::VisualStreamReceiver::Listener
    
    int onSourceActivateI(const char *path, const char *types,lo_arg **argv,
                                  int argc, lo_message msg);
    
    int onSourceDeactivateI(const char *path, const char *types,lo_arg **argv,
                                    int argc, lo_message msg);
    
    int onSourcePositionIff(const char *path, const char *types,lo_arg **argv,
                                    int argc, lo_message msg);
    
    int onSourceAngleIf(const char *path, const char *types,lo_arg **argv,
                                int argc, lo_message msg);
    
    int onSourceTypeIi(const char *path, const char *types,lo_arg **argv,
                               int argc, lo_message msg);
    
    int onSourceNameIs(const char *path, const char *types,lo_arg **argv,
                               int argc, lo_message msg);
    
    int onSourceColorIiii(const char *path, const char *types,lo_arg **argv,
                                  int argc, lo_message msg);
    
    int onSourceGroupIdIi(const char *path, const char *types,lo_arg **argv,
                                  int argc, lo_message msg);
    
    int onSourceRotatingDirectionIi(const char *path, const char *types,lo_arg **argv,
                                            int argc, lo_message msg);
    
    int onSourceScalingDirectionIi(const char *path, const char *types,lo_arg **argv,
                                           int argc, lo_message msg);
    
    int onSourceDopplerEffectIi(const char *path, const char *types,lo_arg **argv,
                                        int argc, lo_message msg);
    
    int onGlobalMaxNoSourcesI(const char *path, const char *types,lo_arg **argv,
                                      int argc, lo_message msg);
    
    int onGlobalRenderpolygonSiVarFs(const char *path, const char *types,lo_arg **argv,
                                           int argc, lo_message msg);
    
    int onProjectXmlDumpIs(const char *path, const char *types,lo_arg **argv,
                                   int argc, lo_message msg);
    
    int onStreamVisualPongI(const char *path, const char *types,lo_arg **argv,
                            int argc, lo_message msg);
    
    int onStreamVisualConnect(const char *path, const char *types,lo_arg **argv,
                            int argc, lo_message msg);
    
    int onStreamVisualDisconnect(const char *path, const char *types,lo_arg **argv,
                            int argc, lo_message msg);
    
    int onReplySis(const char *path, const char *types,lo_arg **argv,
                           int argc, lo_message msg);
    
    int onPluginStandaloneI(const char *path, const char *types,lo_arg **argv,
                            int argc, lo_message msg);
    
    //==============================================================================
    // Wrapper-Methods for wonder::VisualStreamReceiver::PingHandler:
    
    int onStreamVisualPingI(const char *path, const char *types,lo_arg **argv,
                            int argc, lo_message msg);
};

}


#endif  // VSRECEIVER_H_INCLUDED
