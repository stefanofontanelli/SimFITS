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

#include "VehicleApp.h"
#include "AimsunApplPkt.h"
#include "NetwControlInfo.h"
#include <SimpleAddress.h>
#include <cassert>
#include <sstream>


Define_Module(VehicleApp);

void VehicleApp::initialize(int stage)
{
	BaseApplLayer::initialize(stage);

	if (stage == 0)
    {
        aid = par("aid");
        start_section = par("section");
        end_section = -1;
        tx_period = par("tx_period");
        V2V_tx = 0;
        V2R_tx = 0;
        V2V_rx = 0;
        R2V_rx = 0;
        delayTimer = new cMessage("delay-timer", SEND_PACKET_TIMER);
	}
    else if (stage == 1)
    {
        scheduleAt(simTime() + tx_period, delayTimer);
	}
}

void VehicleApp::handleLowerMsg(cMessage *msg)
{
	AimsunApplPkt *pkt = static_cast<AimsunApplPkt*>(msg);
    switch( pkt->getKind() )
	{
        case V2V_PACKET:
            V2V_rx++;
            break;

        case R2V_PACKET:
            R2V_rx++;
            break;

        default:
            break;
	}
	delete msg;
	msg = NULL;
}

void VehicleApp::handleSelfMsg(cMessage *msg)
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

void VehicleApp::sendBroadcast()
{
    AimsunApplPkt *pkt = NULL;
    
    pkt = new AimsunApplPkt("BROADCAST_MESSAGE", V2V_PACKET);
    pkt->setAimsunId(aid);
    pkt->setDestAddr(-1);
    // we use the host modules getIndex() as a appl address
    pkt->setSrcAddr( myApplAddr() );
    pkt->setBitLength(headerLength);
    // set the control info to tell the network layer the layer 3 address
    pkt->setControlInfo( new NetwControlInfo(L3BROADCAST) );
    sendDown(pkt);
    V2V_tx++;
        
    pkt = new AimsunApplPkt("BROADCAST_MESSAGE", V2R_PACKET);
    pkt->setAimsunId(aid);
    pkt->setDestAddr(-1);
    // we use the host modules getIndex() as a appl address
    pkt->setSrcAddr( myApplAddr() );
    pkt->setBitLength(headerLength);
    // set the control info to tell the network layer the layer 3 address
    pkt->setControlInfo( new NetwControlInfo(L3BROADCAST) );
    sendDown(pkt);
    V2R_tx++;
}

void VehicleApp::finish()
{
    std::ostringstream msg;
    msg << "Vehicle: " << aid << ". ";
    msg << "V2V pkts (TX): " << V2V_tx << ". ";
    msg << "V2V pkts (RX): " << V2V_rx << ". ";
    msg << "V2R pkts (TX): " << V2R_tx << ". ";
    msg << "R2V pkts (RX): " << R2V_rx << ". ";
    AimsunPrint(msg.str());
}

VehicleApp::~VehicleApp()
{
	cancelAndDelete(delayTimer);
}