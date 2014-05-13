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

#include "PosMsgLogger.h"

namespace wonder {
PosMsgLogger::PosMsgLogger(const std::string& filename, int sourceID):
    logCount_(0),
    startTime_(std::chrono::high_resolution_clock::now()),
    filePath_(LOG_FILE_DIR + filename),
    sourceID_(sourceID)
{
}
    
PosMsgLogger::~PosMsgLogger()
{
    int nMsgs = logCount_;
    nMsgs = nMsgs <= MAX_LOGGED_MESSAGES ? nMsgs : MAX_LOGGED_MESSAGES;
    
    if(nMsgs){
        // appending the sourceID of the first logged message to the filename:
        const std::string fp = filePath_ + std::to_string(sourceID_) + ".log";
        std::ofstream logfile(fp);
        
        logfile << "Logged " << nMsgs << " of " << logCount_  << " messages." << std::endl;
        logfile << "high_resolution_clock is " << (std::chrono::high_resolution_clock::is_steady ?
            "" : "not ") << "steady." << std::endl;
        
        for(int i=0; i<nMsgs; i++){
            std::chrono::microseconds timestamp =
                std::chrono::duration_cast<std::chrono::microseconds>(log_[i].time.time_since_epoch());
            
            logfile << timestamp.count() << " , " << log_[i].sourceID << " , "
                << log_[i].x << " , " << log_[i].y << std::endl;
        }
    }
}

void PosMsgLogger::logPosMessage(int sourceID, float x, float y)
{
    int msgNo = logCount_.fetch_add(1);
    if (msgNo < MAX_LOGGED_MESSAGES) {
        log_[msgNo].sourceID = sourceID;
        log_[msgNo].x = x;
        log_[msgNo].y = y;
        
        log_[msgNo].time = std::chrono::high_resolution_clock::now();
    }
}
    
void PosMsgLogger::setSourceID(int sourceID)
{
    sourceID_ = sourceID;
}
    
}