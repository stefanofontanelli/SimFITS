//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "JunctionApp.h"
#include "AimsunApplPkt.h"
#include "NetwControlInfo.h"
#include <SimpleAddress.h>
#include <cassert>
#include <sstream>


Define_Module(JunctionApp);

void JunctionApp::initialize(int stage)
{
	BaseApplLayer::initialize(stage);

	if (stage == 0)
    {
        aid = par("aid");
        x = par("x");
        y = par("y");
        z = par("z");
        tx_period = par("tx_period");
        R2V_tx = 0;
        R2R_tx = 0;
        V2R_rx = 0;
        R2R_rx = 0;
        delayTimer = new cMessage("delay-timer", SEND_PACKET_TIMER);
	}
    else if (stage == 1)
    {
        scheduleAt(simTime() + tx_period, delayTimer);
	}
}

void JunctionApp::handleLowerMsg(cMessage *msg)
{
	AimsunApplPkt *pkt = static_cast<AimsunApplPkt*>(msg);
    switch( pkt->getKind() )
	{   
        case V2R_PACKET:
            V2R_rx++;
            break;

        case R2R_PACKET:
            R2R_rx++;
            break;

        default:
            break;
	}
	delete msg;
	msg = NULL;
}

void JunctionApp::handleSelfMsg(cMessage *msg)
{
	switch( msg->getKind() )
	{
        case SEND_PACKET_TIMER:
            assert(msg == delayTimer);
            sendBroadcast();
			scheduleAt(simTime() + tx_period, msg);
            break;
            
        default:
            delete msg;
	}
}

void JunctionApp::sendBroadcast()
{	
    AimsunApplPkt *pkt = NULL;
    
    pkt = new AimsunApplPkt("BROADCAST_MESSAGE", R2V_PACKET);
    pkt->setAimsunId(aid);
    pkt->setDestAddr(-1);
    // we use the host modules getIndex() as a appl address
    pkt->setSrcAddr( myApplAddr() );
    pkt->setBitLength(headerLength);
    // set the control info to tell the network layer the layer 3 address
    pkt->setControlInfo( new NetwControlInfo(L3BROADCAST) );
    sendDown(pkt);
    R2V_tx++;
        
    pkt = new AimsunApplPkt("BROADCAST_MESSAGE", R2R_PACKET);
    pkt->setAimsunId(aid);
    pkt->setDestAddr(-1);
    // we use the host modules getIndex() as a appl address
    pkt->setSrcAddr( myApplAddr() );
    pkt->setBitLength(headerLength);
    // set the control info to tell the network layer the layer 3 address
    pkt->setControlInfo( new NetwControlInfo(L3BROADCAST) );
    sendDown(pkt);
    R2R_tx++;
}

void JunctionApp::finish()
{
    std::ostringstream msg;
    msg << "Junction: " << aid << ". ";
    msg << "R2R pkts (TX): " << R2R_tx << ". ";
    msg << "R2R pkts (RX): " << R2R_rx << ". ";
    msg << "R2V pkts (TX): " << R2V_tx << ". ";
    msg << "V2R pkts (RX): " << V2R_rx << ". ";
    AimsunPrint(msg.str());
}

JunctionApp::~JunctionApp()
{
	cancelAndDelete(delayTimer);
}