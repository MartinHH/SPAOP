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
                                   Listener* listener,
                                   ConnectionTimer::Factory* timerFactory,
                                   XmlParser* xmlParser,
                                   int maxSources,
                                   const std::string clientName):
    server_(vsFactory->createVisualStreamMulticastReceiver(VISUAL_MC_GROUP_STR,
                                                           VISUAL_MC_PORT_STR,
                                                           VISUAL_MC_IFACE_IP_STR)),
    cWonder_(server_->createSenderThread(CWONDER_DEFAULT_IP_STR, CWONDER_DEFAULT_PORT_STR)),
    mCaster_(server_->createSenderThread("localhost", MULTICASTER_PORT_STR)),
    peerGroup_(server_->createSenderThread(VISUAL_MC_GROUP_STR, VISUAL_MC_PORT_STR)),
    xmlParser_(xmlParser),
    sources_(new SourceCollection(maxSources)),
    sourceID_(0),
    room_(),
    listener_(listener),
    pingControl_(timerFactory, this, 0),
    clientName_(clientName),
    linkedToWonder_(false),
    isLocked_(false),
    cStatus_(inactive),
    sentLogger_("sent_"),
    rcvdLogger_("received_")
{    
    for(int i=0; i<Source::totalNumParams; i++){
        lastValues_[i] = Source::denormalizeParameter(i, Source::getParameterDefaultValue(i));
    }
    
    peerGroup_->setTtl(VISUAL_MC_TTL);
    peerGroup_->setIfaceByIp(VISUAL_MC_IFACE_IP_STR);
    
    server_->setListener(this);
    server_->setPingHandler(this);
    server_->start();
    mCaster_->start();
    cWonder_->start();
    peerGroup_->start();
}
    
SourceController::~SourceController()
{
    mCaster_->stop();
    cWonder_->stop();
    server_->stop();
    peerGroup_->stop();

    streamSource().sendStreamVisualDisconnect();
    
    mCaster_->join();
    cWonder_->join();
    server_->join();
    peerGroup_->join();
}
    
const Source& SourceController::getSource() const
{
    return sources_->getSource(sourceID_);
}
    
std::shared_ptr<const SourceCollection> SourceController::getSources() const
{
    return sources_;
}
    
bool SourceController::setSource(const wonder::Source &source)
{
    if(isLocked_){
        return false;
    } else if (sources_->setSource(source)){
        sourceID_ = source.getID();
        sentLogger_.setSourceID(sourceID_);
        rcvdLogger_.setSourceID(sourceID_);
        return true;
    } else {
        return false;
    }
}
    
bool SourceController::setID(int sourceID)
{
    if(isLocked_){
        return false;
    } else {
        sourceID_ = sourceID;
        sentLogger_.setSourceID(sourceID);
        rcvdLogger_.setSourceID(sourceID);
        return true;
    }
}

void SourceController::setParameterAndSendChange(int paramIndex, float normalizedValue)
{
    sources_->setParameterNormalized(sourceID_, paramIndex, normalizedValue);
    
    if(isLocked_ && relevantChange(paramIndex))
    {
        const Source& source_ = sources_->getSource(sourceID_);
        
        // store the new value as last sent value before sending:
        lastValues_[paramIndex] = source_.getDenormalizedParameter(paramIndex);
        
        switch (paramIndex)
        {
            case Source::typeParam:
                dataDest().sendSourceType(source_.getID(), source_.getType());
                break;
            case Source::xPosParam:
            case Source::yPosParam:
            {
                // since we send out both x and y, both msut be stored as last sent:
                lastValues_[Source::xPosParam] = source_.getXPos();
                lastValues_[Source::yPosParam] = source_.getYPos();
                const int id = source_.getID();
                const float x = lastValues_[Source::xPosParam];
                const float y = lastValues_[Source::yPosParam];
                sentLogger_.logPosMessage(id, x, y);
                dataDest().sendSourcePosition(id, x, y);
                break;
            }
            case Source::angleParam:
                dataDest().sendSourceAngle(source_.getID(), lastValues_[Source::angleParam]);
                break;
            case Source::dopplParam:
                dataDest().sendSourceDopplerEffect(source_.getID(),
                                                     (int) source_.dopplerIsEnabled());
                break;
            default:
                break;
        }
    }
}
    
void SourceController::updateSourceName(const std::string& newSourceName)
{
    sources_->setName(sourceID_, newSourceName);
    if(isLocked_){
        dataDest().sendSourceName(sourceID_, newSourceName);
    }
}
    
void SourceController::updateSourceColour(const Colour colour)
{
    sources_->setColour(sourceID_, colour);
    if(isLocked_){
        dataDest().sendSourceColor(sourceID_, colour);
    }
}
    
void SourceController::setLinkedToWonder(bool linked, bool notifyPeers)
{
    if (linked && !linkedToWonder_) {
        linkedToWonder_ = true;
        streamSource().sendStreamVisualConnect("SPAOP");
        sendOwnState();
        if(notifyPeers){
            peerGroup_->sendPluginStandalone(false);
        }
    } else if(!linked && linkedToWonder_) {
        linkedToWonder_ = false;
        streamSource().sendStreamVisualConnect("SPAOP");
        sendOwnState(); // just in case the peers were not linked to cWONDER
        if(notifyPeers){
            peerGroup_->sendPluginStandalone(true);
        }
        
        // no ping control in standalone mode:
        pingControl_.stop();
        cStatus_ = inactive;
    }
}
    
bool SourceController::isLinkedToWonder() const
{
    return linkedToWonder_;
}
    
void SourceController::setIdIsLocked(bool isLocked)
{
    if (isLocked && !isLocked_) {
        isLocked_ = true;
        streamSource().sendStreamVisualConnect("SPAOP");
    } else if(!isLocked && isLocked_){
        isLocked_ = false;
        pingControl_.stop();
        cStatus_ = inactive;
    }
}
    
bool SourceController::idIsLocked() const
{
    return isLocked_;
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
    return dataDest().hostname() + ":" + dataDest().port();
}
    
const Room* SourceController::getRoom() const
{
    return &room_;
}
    
void SourceController::setRoom(wonder::Room &room)
{
    room_ = room;
}
    
void SourceController::setIncomingParameter(int sourceID,
                                             Source::AutomatedParameters index,
                                             float unnormalizedValue)
{
    const float newValue = Source::normalizeParameter(index, unnormalizedValue);
    if(sourceID == sourceID_){
        listener_->incomingParameterChange(index, newValue);
    }
    sources_->setParameterNormalized(sourceID, index, newValue);
}

bool SourceController::relevantChange(int index)
{
    const Source& source_ = sources_->getSource(sourceID_);

    switch (index)
    {
        // for those parameters that have a precision defined, the precision defines if
        // a relevant change has happened:
        case Source::xPosParam:
            return ABS(source_.getXPos() - lastValues_[Source::xPosParam]) > COORD_PRECISION;
        case Source::yPosParam:
            return ABS(source_.getYPos() - lastValues_[Source::yPosParam]) > COORD_PRECISION;
        case Source::angleParam:
            return ABS(source_.getAngle() - lastValues_[Source::angleParam]) > ANGLE_PRECISION;
        
        // for the rest, any change is relevant:
        default:
            if (index >= 0 && index < Source::totalNumParams) {
                return source_.getDenormalizedParameter(index) != lastValues_[index];
            } else {
                return false; // index out of bounds
            }
    }
}
    
OscSenderThread& SourceController::dataDest() const
{
    return linkedToWonder_ ? *cWonder_ : *peerGroup_;
}

OscSenderThread& SourceController::streamSource() const
{
    return linkedToWonder_ ? *mCaster_ : *peerGroup_;
}
    
void SourceController::sendOwnState()
{
    dataDest().sendSourceActivate(sourceID_);
    
    const wonder::Source& src = sources_->getSource(sourceID_);
    dataDest().sendFullSourceInfo(src);
    
    // update lastValues_:
    for(int i=0; i<Source::totalNumParams; i++){
        lastValues_[i] = src.getDenormalizedParameter(i);
    }
    
    dataDest().sendSourceActivate(sourceID_); // twice in case one gets lost...
}
    
//==============================================================================
//vvvvv  Implementing PingControl::Listener
    
void SourceController::connectionLost(const int connectionID)
{
    cStatus_ = error;
    listener_->connectionLost();
}
    
//==============================================================================
//vvvvv  OSC-message-handlers implementing wonder::VisualStreamCallInterface
    
int SourceController::onSourceActivate(int sourceID)
{
    if(isLocked_){
        sources_->activate(sourceID);
        // TODO: reconsider the (sourceID_ == sourceID) case
    }
    return 0;
}
    
int SourceController::onSourceDeactivate(int sourceID)
{
    if(isLocked_){
        if (sourceID == sourceID_) {
            // someone else has deactivated "our" source -> reactivate
            dataDest().sendSourceActivate(sourceID_);
        } else {
            sources_->deactivate(sourceID);
        }
    }
    return 0;
}
    
int SourceController::onSourcePosition(int sourceID, float xPos, float yPos)
{
    if(isLocked_){
        rcvdLogger_.logPosMessage(sourceID, xPos, yPos);

        setIncomingParameter(sourceID, Source::xPosParam, xPos);
        setIncomingParameter(sourceID, Source::yPosParam, yPos);
    }
    return 0;
}

int SourceController::onSourceAngle(int sourceID, float angle)
{
    if(isLocked_){
        setIncomingParameter(sourceID, Source::angleParam, angle);
    }
    return 0;
}

int SourceController::onSourceType(int sourceID, int type)
{
    if(isLocked_){
        setIncomingParameter(sourceID, Source::typeParam, type);
    }
    return 0;
}
    
int SourceController::onSourceName(int sourceID, const std::string& sourceName)
{
    if(isLocked_){
        sources_->setName(sourceID, sourceName);
        // TODO: consider stack overflow...
    }
    return 0;
}
    
int SourceController::onSourceColor(int sourceID, int r, int g, int b)
{
    if(isLocked_){
        sources_->setColour(sourceID, Colour(r, g, b));
    }
    return 0;
}
    
int SourceController::onSourceGroupId(int sourceID, int groupID)
{
    /*TODO*/
    return 0;
}
    
int SourceController::onSourceRotatingDirection(int sourceID, int rotDir)
{
    // not supported (yet?)
    return 0;
}
    
int SourceController::onSourceScalingDirection(int sourceID, int scalDir)
{
    // not supported (yet?)
    return 0;
}
    
int SourceController::onSourceDopplerEffect(int sourceID, int doppler)
{
    if(isLocked_){
        setIncomingParameter(sourceID, Source::dopplParam, doppler);
    }
    return 0;
}
    
int SourceController::onGlobalMaxNoSources(int maxSources)
{
    /*TODO*/
    return 0;
}
    
int SourceController::onGlobalRenderpolygon(Room room)
{
    room_ = room;
    
    return 0;
}
    
int SourceController::onProjectXmlDump(int err, const std::string& xmlDump)
{
    if(isLocked_){
        xmlParser_->updateSourceCollectionFromCWonderProject(xmlDump, *(sources_.get()));
    }
    return 0;
}
    
int SourceController::onStreamVisualPing(int pingCount, OscSender* replyTo)
{
    if(isLocked_){
        cStatus_ = active;
        replyTo->sendStreamVisualPong(pingCount);
        pingControl_.onPing(); // first Ping will start PingControl
    }
    return 0;
}
    
int SourceController::onStreamVisualConnect(wonder::OscSender *replyTo)
{
    // when /WONDER/stream/visual/connect is received, the sender hopes
    // to get all informtion about the current state of the system.
    // We send back the part we are responsible for. (Since all plugins
    // are sending from the same port and listening to the multicast group
    // only, we cannot use the replyTo address here.)
    
    if(isLocked_){
        peerGroup_->sendSourceActivate(sourceID_);
        peerGroup_->sendFullSourceInfo(sources_->getSource(sourceID_));
        peerGroup_->sendSourceActivate(sourceID_);
    }
    return 0;
}
    
int SourceController::onReply(std::string replyToMsg, int state, std::string msg)
{
    /*TODO*/
    return 0;
}
    
int SourceController::onPluginStandalone(bool standAloneOn)
{
    setLinkedToWonder(!standAloneOn, false);
    return 0;
}
    
}
