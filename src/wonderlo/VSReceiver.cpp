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

#include "VSReceiver.h"

namespace wonderlo {

wonder::VisualStreamReceiver::Factory* VSReceiver::getFactory()
{
    static VSReceiver::Factory f; // thread-safe in C++11
    return &f;
}
    
VSReceiver::VSReceiver(lo_err_handler err_h):
    lowrappers::ServerThread(err_h),
    listener_(nullptr),
    pingHandler_(nullptr)
{
    addMethods();
}

VSReceiver::VSReceiver(const std::string &port, lo_err_handler err_h):
    lowrappers::ServerThread(port, err_h),
    listener_(nullptr),
    pingHandler_(nullptr)
{
    addMethods();
}
    
VSReceiver::VSReceiver(const std::string& group, const std::string &port,
                           const std::string& ifaceIp, lo_err_handler err_h):
    lowrappers::ServerThread(group, port, ifaceIp, err_h),
    listener_(nullptr),
    pingHandler_(nullptr)
{
    addMethods();
}

VSReceiver::~VSReceiver()
{
}

wonder::OscSender* VSReceiver::createSender(const std::string &host,
                                            const std::string &port)
{
    return new WonderOscServerSender(host, port, *this);
}
    
wonder::OscSenderThread* VSReceiver::createSenderThread(const std::string &host,
                                                        const std::string &port)
{
    return new WonderOscServerSenderThread(host, port, *this);
}

void VSReceiver::setListener(VisualStreamReceiver::Listener *listener)
{
    listener_ = listener;
}
    
void VSReceiver::setPingHandler(VisualStreamReceiver::PingHandler *pingHandler)
{
    pingHandler_ = pingHandler;
}
    
void VSReceiver::start()
{
    lowrappers::ServerThread::start();
}

void VSReceiver::stop()
{
    lowrappers::ServerThread::stop();
}
    
void VSReceiver::join()
{
    lowrappers::ServerThread::join();
}
    
int VSReceiver::port()
{
    return ServerThread::port();
}
    
wonder::VisualStreamReceiver* VSReceiver::Factory::createVisualStreamReceiver()
{
    VSReceiver* rv = new VSReceiver(NULL);
    return rv->isValid() ? rv : nullptr;
}
    
wonder::VisualStreamReceiver* VSReceiver::Factory::createVisualStreamReceiver(const std::string& port)
{
    VSReceiver* rv = new VSReceiver(port, NULL);
    return rv->isValid() ? rv : nullptr;
}
    
wonder::VisualStreamReceiver* VSReceiver::Factory::createVisualStreamMulticastReceiver(
                                                                    const std::string &group,
                                                                    const std::string &port,
                                                                    const std::string &iface)
{
    VSReceiver* rv = new VSReceiver(group, port, iface, NULL);
    return rv->isValid() ? rv : nullptr;
}
    
void VSReceiver::addMethods()
{
    addListenerAndStoreFunctor("/WONDER/source/activate", "i",
                 lowrappers::ListenerMaker(*this, &VSReceiver::onSourceActivateI));
    addListenerAndStoreFunctor("/WONDER/source/deactivate", "i",
                 lowrappers::ListenerMaker(*this, &VSReceiver::onSourceDeactivateI));
    addListenerAndStoreFunctor("/WONDER/source/position", "iff",
                 lowrappers::ListenerMaker(*this, &VSReceiver::onSourcePositionIff));
    addListenerAndStoreFunctor("/WONDER/source/angle", "if",
                 lowrappers::ListenerMaker(*this, &VSReceiver::onSourceAngleIf));
    addListenerAndStoreFunctor("/WONDER/source/type", "ii",
                 lowrappers::ListenerMaker(*this, &VSReceiver::onSourceTypeIi));
    addListenerAndStoreFunctor("/WONDER/source/name", "is",
                 lowrappers::ListenerMaker(*this, &VSReceiver::onSourceNameIs));
    addListenerAndStoreFunctor("/WONDER/source/color", "iiii",
                 lowrappers::ListenerMaker(*this, &VSReceiver::onSourceColorIiii));
    addListenerAndStoreFunctor("/WONDER/source/groupId", "ii",
                 lowrappers::ListenerMaker(*this, &VSReceiver::onSourceGroupIdIi));
    addListenerAndStoreFunctor("/WONDER/source/rotationDirection", "ii",
                 lowrappers::ListenerMaker(*this, &VSReceiver::onSourceRotatingDirectionIi));
    addListenerAndStoreFunctor("/WONDER/source/scalingDirection", "ii",
                 lowrappers::ListenerMaker(*this, &VSReceiver::onSourceScalingDirectionIi));
    addListenerAndStoreFunctor("/WONDER/source/dopplerEffect", "ii",
                 lowrappers::ListenerMaker(*this, &VSReceiver::onSourceDopplerEffectIi));
    addListenerAndStoreFunctor("/WONDER/listener/position", "iff",
                               lowrappers::ListenerMaker(*this, &VSReceiver::onListenerPositionIff));
    addListenerAndStoreFunctor("/WONDER/global/maxNoSources", "i",
                 lowrappers::ListenerMaker(*this, &VSReceiver::onGlobalMaxNoSourcesI));
    addListenerAndStoreFunctor("/WONDER/global/renderPolygon", NULL,
                 lowrappers::ListenerMaker(*this, &VSReceiver::onGlobalRenderpolygonSiVarFs));
    addListenerAndStoreFunctor("/WONDER/project/xmlDump", "is",
                 lowrappers::ListenerMaker(*this, &VSReceiver::onProjectXmlDumpIs));
    addListenerAndStoreFunctor("/WONDER/stream/visual/ping", "i",
                 lowrappers::ListenerMaker(*this, &VSReceiver::onStreamVisualPingI));
    addListenerAndStoreFunctor("/WONDER/stream/visual/pong", "i",
                 lowrappers::ListenerMaker(*this, &VSReceiver::onStreamVisualPongI));
    addListenerAndStoreFunctor("/WONDER/stream/visual/connect", NULL,
                 lowrappers::ListenerMaker(*this, &VSReceiver::onStreamVisualConnect));
    addListenerAndStoreFunctor("/WONDER/stream/visual/disconnect", NULL,
                 lowrappers::ListenerMaker(*this, &VSReceiver::onStreamVisualDisconnect));
    addListenerAndStoreFunctor("/WONDER/plugin/standalone", "i",
                lowrappers::ListenerMaker(*this, &VSReceiver::onPluginStandaloneI));
    addListenerAndStoreFunctor("/WONDER/reply", "sis",
                 lowrappers::ListenerMaker(*this, &VSReceiver::onReplySis));
}
    
void VSReceiver::addListenerAndStoreFunctor(const char* path, const char* types,
                                            lowrappers::ServerThread::Listener *listener)
{
    // add listener to auto-deletion list:
    functorList_.push_back(std::unique_ptr<lowrappers::ServerThread::Listener>(listener));
    
    // add listener to server:
    addListener(path, types, listener);
}

//==============================================================================
// Wrapper-Methods for wonder::VisualStreamReceiver::Listener
    
int VSReceiver::onSourceActivateI(const char *path, const char *types,lo_arg **argv,
                                  int argc, lo_message msg)
{
    if(listener_ == nullptr){
        return 0;
    } else {
        return listener_->onSourceActivate(argv[0]->i);
    }
}

int VSReceiver::onSourceDeactivateI(const char *path, const char *types,lo_arg **argv,
                                    int argc, lo_message msg)
{
    if(listener_ == nullptr){
        return 0;
    } else {
        return listener_->onSourceDeactivate(argv[0]->i);
    }
}

int VSReceiver::onSourcePositionIff(const char *path, const char *types,lo_arg **argv,
                                    int argc, lo_message msg)
{
    if(listener_ == nullptr){
        return 0;
    } else {
        return listener_->onSourcePosition(argv[0]->i, argv[1]->f, argv[2]->f);
    }
}

int VSReceiver::onSourceAngleIf(const char *path, const char *types,lo_arg **argv,
                                int argc, lo_message msg)
{
    if(listener_ == nullptr){
        return 0;
    } else {
        return listener_->onSourceAngle(argv[0]->i, argv[1]->f);
    }
}

int VSReceiver::onSourceTypeIi(const char *path, const char *types,lo_arg **argv,
                               int argc, lo_message msg)
{
    if(listener_ == nullptr){
        return 0;
    } else {
        return listener_->onSourceType(argv[0]->i, argv[1]->i);
    }
}

int VSReceiver::onSourceNameIs(const char *path, const char *types,lo_arg **argv,
                               int argc, lo_message msg)
{
    if(listener_ == nullptr){
        return 0;
    } else {
        return listener_->onSourceName(argv[0]->i, std::string(&(argv[1]->s)));
    }
}

int VSReceiver::onSourceColorIiii(const char *path, const char *types,lo_arg **argv,
                                  int argc, lo_message msg)
{
    if(listener_ == nullptr){
        return 0;
    } else {
        return listener_->onSourceColor(argv[0]->i, argv[1]->i,
                                            argv[2]->i, argv[3]->i);
    }
}

int VSReceiver::onSourceGroupIdIi(const char *path, const char *types,lo_arg **argv,
                                  int argc, lo_message msg)
{
    if(listener_ == nullptr){
        return 0;
    } else {
        return listener_->onSourceGroupId(argv[0]->i, argv[1]->i);
    }
}

int VSReceiver::onSourceRotatingDirectionIi(const char *path, const char *types,lo_arg **argv,
                                            int argc, lo_message msg)
{
    if(listener_ == nullptr){
        return 0;
    } else {
        return listener_->onSourceRotatingDirection(argv[0]->i, argv[1]->i);
    }
}

int VSReceiver::onSourceScalingDirectionIi(const char *path, const char *types,lo_arg **argv,
                                           int argc, lo_message msg)
{
    if(listener_ == nullptr){
        return 0;
    } else {
        return listener_->onSourceScalingDirection(argv[0]->i, argv[1]->i);
    }
}

int VSReceiver::onSourceDopplerEffectIi(const char *path, const char *types,lo_arg **argv,
                                        int argc, lo_message msg)
{
    if(listener_ == nullptr){
        return 0;
    } else {
        return listener_->onSourceDopplerEffect(argv[0]->i, argv[1]->i);
    }
}
    
int VSReceiver::onListenerPositionIff(const char *path, const char *types, lo_arg **argv, int argc, lo_message msg)
{
    if(listener_ == nullptr){
        return 0;
    } else {
        return listener_->onListenerPosition(argv[0]->i, argv[1]->f, argv[2]->f);
    }
}
    
int VSReceiver::onGlobalMaxNoSourcesI(const char *path, const char *types,lo_arg **argv,
                                      int argc, lo_message msg)
{
    if(listener_ == nullptr){
        return 0;
    } else {
        return listener_->onGlobalMaxNoSources(argv[0]->i);
    }
}

int VSReceiver::onGlobalRenderpolygonSiVarFs(const char *path, const char *types,lo_arg **argv,
                                           int argc, lo_message msg)
{
    if(listener_ == nullptr || types[0] != 's' || types[1] != 'i'){
        return 0;
    } else {
        
        // create Room object:

        const std::string roomName = &(argv[0]->s);
        const int nOfVerts = argv[1]->i;

        wonder::Room newRoom(roomName, nOfVerts);
        
        for (int i=0; i< nOfVerts; i++){
            wonder::Room::Vertex vertx(argv[2 + i * 3]->f,
                                       argv[2 + i * 3 + 1]->f,
                                       argv[2 + i * 3 + 2]->f);
            newRoom.setVertex(i, vertx);
        }

        return listener_->onGlobalRenderpolygon(newRoom);
    }
}

int VSReceiver::onProjectXmlDumpIs(const char *path, const char *types,lo_arg **argv,
                                   int argc, lo_message msg)
{
    if(listener_ == nullptr){
        return 0;
    } else {
        return listener_->onProjectXmlDump(argv[0]->i, std::string(&(argv[1]->s)));
    }
}
    
int VSReceiver::onStreamVisualPongI(const char *path, const char *types,lo_arg **argv,
                                        int argc, lo_message msg)
{
    if(listener_ == nullptr){
        return 0;
    } else {
        return listener_->onStreamVisualPong(argv[0]->i);
    }
}

int VSReceiver::onStreamVisualConnect(const char *path, const char *types, lo_arg **argv,
                                          int argc, lo_message msg)
{
    if(listener_ == nullptr){
        return 0;
    } else {
        // the lo_address returned from lo_message_get_source will be free'd along
        // with msg, so we need to set ownership to false:
        WonderOscServerSender replyAddress(lo_message_get_source(msg), *this, false);
        return listener_->onStreamVisualConnect(&replyAddress);
    }
}
    
int VSReceiver::onStreamVisualDisconnect(const char *path, const char *types, lo_arg **argv,
                                            int argc, lo_message msg)
{
    if(listener_ == nullptr){
        return 0;
    } else {
        return listener_->onStreamVisualDisconnect();
    }
}
    
int VSReceiver::onReplySis(const char *path, const char *types,lo_arg **argv,
                           int argc, lo_message msg)
{
    if(listener_ == nullptr){
        return 0;
    } else {
        return listener_->onReply(std::string(&(argv[0]->s)), argv[1]->i,
                                     std::string(&(argv[2]->s)));
    }
}
    
int VSReceiver::onPluginStandaloneI(const char *path, const char *types, lo_arg **argv,
                                    int argc, lo_message msg)

{
    if(listener_ == nullptr){
        return 0;
    } else {
        return listener_->onPluginStandalone(argv[0]->i);
    }
}
    
//==============================================================================
// Wrapper-Methods for wonder::VisualStreamReceiver::PingHandler:
    
    
int VSReceiver::onStreamVisualPingI(const char *path, const char *types,lo_arg **argv,
                                        int argc, lo_message msg)
{
    if(pingHandler_ == nullptr){
        return 0;
    } else {
        // the lo_address returned from lo_message_get_source will be free'd along
        // with msg, so we need to set ownership to false:
        WonderOscServerSender replyAddress(lo_message_get_source(msg),  *this, false);
        return pingHandler_->onStreamVisualPing(argv[0]->i, &replyAddress);
    }
}
    
}
