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

#include "SourceController.h"

namespace wonder {

SourceController::SourceController(VisualStreamReceiver::Factory* vsFactory,
                                   ConnectionTimer::Factory* timerFactory,
                                   XmlParser* xmlParser,
                                   int maxSources):
    server_(vsFactory->createVisualStreamReceiver()), // TODO: fixed port?
    cWonder_(server_->createSenderThread(CWONDER_DEFAULT_IP_STR, CWONDER_DEFAULT_PORT_STR)),
    xmlParser_(xmlParser),
    sources_(new SourceCollection(maxSources)),
    room_(new Room()),
    listeners_(maxSources, nullptr),
    pingControl_(timerFactory, this, 0),
    cStatus_(inactive),
    lastValues_(maxSources, std::vector<float>(Source::totalNumParams))
{
    // initialize the lastValues 2d vector:
    for(int i=0; i<lastValues_.size(); i++){
        for (int j=0; j<lastValues_[i].size(); j++) {
            lastValues_[i][j] = Source::denormalizeParameter(j, Source::getParameterDefaultValue(j));
        }
    }
    
    // initialize and start communication threads:
    server_->setListener(this);
    server_->setPingHandler(this);
    server_->start();
    cWonder_->start();
    
    connect();
}
    
SourceController::~SourceController()
{
    // in case any listeners are still registered (should not be the case),
    // a deactivation message is sent:
    for(int i = 0; i < listeners_.size(); i++){
        std::lock_guard<std::mutex> lock(listenersMutex_);
        if (listeners_[i] != nullptr) {
            cWonder_->sendSourceDeactivate(i);
        }
    }
    
    // not supported by cWONDER at the moment, but worth a try anyway:
    cWonder_->sendStreamVisualDisconnect();
    
    // stop and join communication threads:
    cWonder_->stop();
    server_->stop();
    
    cWonder_->join();
    server_->join();
}
    
void SourceController::setListener(int sourceID, Listener *listener)
{
    if(sourceID >= 0 && sourceID < listeners_.size()){
        std::lock_guard<std::mutex> lock(listenersMutex_);
        listeners_[sourceID] = listener;
    }
}
    
void SourceController::removeListener(int sourceID)
{
    if(sourceID >= 0 && sourceID < listeners_.size()){
        std::lock_guard<std::mutex> lock(listenersMutex_);
        listeners_[sourceID] = nullptr;
    }
}
    
bool SourceController::hasListenerForID(int sourceID) const
{
    return sourceID >= 0 && sourceID < listeners_.size() && listeners_[sourceID] != nullptr;
}
    
const Source& SourceController::getSource(int sourceID) const
{
    return sources_->getSource(sourceID);
}
    
const SourceCollection* SourceController::getSources() const
{
    return sources_;
}
    
void SourceController::activateSource(int sourceID)
{
    sources_->activate(sourceID);
    cWonder_->sendSourceActivate(sourceID);
}
    
void SourceController::deactivateSource(int sourceID)
{
    sources_->deactivate(sourceID);
    cWonder_->sendSourceActivate(sourceID);
}
    
bool SourceController::setSource(const wonder::Source &source)
{
    return sources_->setSource(source);
}

void SourceController::setParameterAndSendChange(int sourceID, int paramIndex, float normalizedValue)
{
    // set parameter
    sources_->setParameterNormalized(sourceID, paramIndex, normalizedValue);
    
    // send only if the value's change is relevant:
    if(relevantChange(sourceID, paramIndex)){
        
        const Source& source_ = sources_->getSource(sourceID);
        
        // store the new value as last sent value before sending:
        lastValues_[sourceID][paramIndex] = source_.getDenormalizedParameter(paramIndex);
        
        switch (paramIndex) // chose the right send-method for the parameter:
        {
            case Source::typeParam:
                cWonder_->sendSourceType(source_.getID(), source_.getType());
                break;
            case Source::xPosParam:
            case Source::yPosParam:
            {
                // since we send out both x and y, both must be stored as last sent:
                lastValues_[sourceID][Source::xPosParam] = source_.getXPos();
                lastValues_[sourceID][Source::yPosParam] = source_.getYPos();
                cWonder_->sendSourcePosition(source_.getID(), lastValues_[sourceID][Source::xPosParam],
                                              lastValues_[sourceID][Source::yPosParam]);
                break;
            }
            case Source::angleParam:
                cWonder_->sendSourceAngle(source_.getID(), lastValues_[sourceID][Source::angleParam]);
                break;
            case Source::dopplParam:
                cWonder_->sendSourceDopplerEffect(source_.getID(),
                                                     (int) source_.dopplerIsEnabled());
                break;
            default:
                break;
        }
    }
}
    
void SourceController::setCoordinatesAndSendChange(int sourceID, float normalizedX, float normalizedY)
{
    // set parameters
    sources_->setParameterNormalized(sourceID, Source::xPosParam, normalizedX);
    sources_->setParameterNormalized(sourceID, Source::yPosParam, normalizedY);
    
    // send only if the value's change is relevant:
    if(relevantChange(sourceID, Source::xPosParam) || relevantChange(sourceID, Source::yPosParam)){
        
        const Source& source_ = sources_->getSource(sourceID);
        
        lastValues_[sourceID][Source::xPosParam] = source_.getXPos();
        lastValues_[sourceID][Source::yPosParam] = source_.getYPos();
        cWonder_->sendSourcePosition(source_.getID(), lastValues_[sourceID][Source::xPosParam],
                                      lastValues_[sourceID][Source::yPosParam]);
    }
}
    
void SourceController::updateSourceName(int sourceID, const std::string& newSourceName)
{
    sources_->setName(sourceID, newSourceName);
    cWonder_->sendSourceName(sourceID, newSourceName);
}
    
void SourceController::updateSourceColour(int sourceID, const Colour colour)
{
    sources_->setColour(sourceID, colour);
    cWonder_->sendSourceColor(sourceID, colour);
}
    
bool SourceController::setCWonderAddress(const std::string &ip, const std::string &port)
{
    if(parsetools::isValidIP(ip) && parsetools::isValidPort(port)){
        cWonder_->setAddress(ip, port);
        return true;
    } else {
        return false;
    }
}
    
std::string SourceController::getCWonderHost() const
{
    return cWonder_->hostname();
}
    
std::string SourceController::getCWonderPort() const
{
    return cWonder_->port();
}
   
ConnectionStates SourceController::connectionStatus() const
{
    return cStatus_;
}
    
std::string SourceController::connectionStatusString() const
{
    switch (cStatus_)
    {
        case inactive:  return "Connection inactive";
        case active:    return "Connection active";
        case error:     return "Connection error!";
    }
    return "Status unknown";
}
    
int SourceController::rcvPort() const
{
    return server_->port();
}
    
const std::string SourceController::getDataDestHostAndPort() const
{
    return cWonder_->hostname() + ":" + cWonder_->port();
}
    
std::shared_ptr<const Room> SourceController::getRoom() const
{
    return room_;
}
    
void SourceController::setRoom(const wonder::Room &room)
{
    *room_ = room;
}
    
void SourceController::connect() const
{
    cWonder_->sendStreamVisualConnect("SPAOP");
}
    
void SourceController::setIncomingParameter(int sourceID,
                                            Source::AutomatedParameters index,
                                            float unnormalizedValue)
{
    const float newValue = Source::normalizeParameter(index, unnormalizedValue);
    
    sources_->setParameterNormalized(sourceID, index, newValue);

    // notify Listener (if one is registered for the sourceID):
    std::lock_guard<std::mutex> lock(listenersMutex_);
    if(listeners_[sourceID] != nullptr){
        listeners_[sourceID]->incomingParameterChange(index, newValue);
    }
    
    
}

bool SourceController::relevantChange(int sourceID, int index)
{
    const Source& source_ = sources_->getSource(sourceID);

    switch (index)
    {
        // for those parameters that have a precision defined, the precision defines if
        // a relevant change has happened:
        case Source::xPosParam:
            return ABS(source_.getXPos() - lastValues_[sourceID][Source::xPosParam]) > COORD_PRECISION;
        case Source::yPosParam:
            return ABS(source_.getYPos() - lastValues_[sourceID][Source::yPosParam]) > COORD_PRECISION;
        case Source::angleParam:
            return ABS(source_.getAngle() - lastValues_[sourceID][Source::angleParam]) > ANGLE_PRECISION;
        
        // for the rest, any change is relevant:
        default:
            if (index >= 0 && index < Source::totalNumParams) {
                return source_.getDenormalizedParameter(index) != lastValues_[sourceID][index];
            } else {
                return false; // index out of bounds
            }
    }
}
    
void SourceController::sendSourceState(int sourceID)
{
    cWonder_->sendSourceActivate(sourceID);
    
    const wonder::Source& src = sources_->getSource(sourceID);
    cWonder_->sendFullSourceInfo(src);
    
    // update lastValues_:
    for(int i=0; i<Source::totalNumParams; i++){
        lastValues_[sourceID][i] = src.getDenormalizedParameter(i);
    }
    
    cWonder_->sendSourceActivate(sourceID); // twice in case one gets lost...
}
    
//==============================================================================
//vvvvv  Implementing PingControl::Listener
    
void SourceController::connectionLost(const int connectionID)
{
    cStatus_ = error;
    for(int i = 0; i < listeners_.size(); i++){
        std::lock_guard<std::mutex> lock(listenersMutex_);
        if (listeners_[i] != nullptr) {
            listeners_[i]->connectionLost();
        }
    }
}
    
//==============================================================================
//vvvvv  OSC-message-handlers implementing wonder::VisualStreamCallInterface
    
int SourceController::onSourceActivate(int sourceID)
{
    sources_->activate(sourceID);
    return 0;
}
    
int SourceController::onSourceDeactivate(int sourceID)
{
    // TODO: check if still active on this side,
    // cWonder_->sendSourceActivate(sourceID_);
    return 0;
}
    
int SourceController::onSourcePosition(int sourceID, float xPos, float yPos)
{
    setIncomingParameter(sourceID, Source::xPosParam, xPos);
    setIncomingParameter(sourceID, Source::yPosParam, yPos);
    return 0;
}

int SourceController::onSourceAngle(int sourceID, float angle)
{
    setIncomingParameter(sourceID, Source::angleParam, angle);
    return 0;
}

int SourceController::onSourceType(int sourceID, int type)
{
    setIncomingParameter(sourceID, Source::typeParam, type);
    return 0;
}
    
int SourceController::onSourceName(int sourceID, const std::string& sourceName)
{
    sources_->setName(sourceID, sourceName);
    return 0;
}
    
int SourceController::onSourceColor(int sourceID, int r, int g, int b)
{
    sources_->setColour(sourceID, Colour(r, g, b));
    return 0;
}
    
int SourceController::onSourceDopplerEffect(int sourceID, int doppler)
{
    setIncomingParameter(sourceID, Source::dopplParam, doppler);
    return 0;
}
    
int SourceController::onGlobalMaxNoSources(int maxSources)
{
    // Not used
    return 0;
}
    
int SourceController::onGlobalRenderpolygon(Room& room)
{
    *room_ = room;
    return 0;
}
    
int SourceController::onProjectXmlDump(int err, const std::string& xmlDump)
{
    xmlParser_->updateSourceCollectionFromCWonderProject(xmlDump, *sources_);
    return 0;
}
    
int SourceController::onStreamVisualPing(int pingCount, OscSender* replyTo)
{
    replyTo->sendStreamVisualPong(pingCount);
    
    // update status:
    cStatus_ = active;
        
    // notify PingControl (on the first Ping, this will also start PingControl):
    pingControl_.onPing();
    
    return 0;
}
    
int SourceController::onStreamVisualConnect(wonder::OscSender *replyTo)
{
    // not implemented (yet?)
    return 0;
}
    
int SourceController::onReply(std::string replyToMsg, int state, std::string msg)
{
    // not implemented (yet?)
    return 0;
}

}
