/***************************************************************************
 * file:        ConstSpeedMobility.cc
 *
 * author:      Steffen Sroka
 *
 * copyright:   (C) 2004 Telecommunication Networks Group (TKN) at
 *              Technische Universitaet Berlin, Germany.
 *
 *              This program is free software; you can redistribute it
 *              and/or modify it under the terms of the GNU General Public
 *              License as published by the Free Software Foundation; either
 *              version 2 of the License, or (at your option) any later
 *              version.
 *              For further information see file COPYING
 *              in the top level directory
 ***************************************************************************
 * part of:     framework implementation developed by tkn
 **************************************************************************/


#include "AimsunMobility.h"
#include "AimsunWorldUtility.h"
#include <FWMath.h>
#include <sstream>
#include <math.h>


Define_Module(AimsunMobility);


/**
 * Reads the updateInterval and the velocity
 *
 * If the host is not stationary it calculates a random position and
 * schedules a timer to trigger the first movement
 */
void AimsunMobility::initialize(int stage)
{
    if (stage == 0)
    {
        BaseMobility::initialize(stage);
        aid = par("aid");
        vi = NULL;
        updateInterval = AKIGetSimulationStepTime();
        makeMove();
    }
    else if (stage == 1) 
    {
        coreEV << "initializing BaseMobility stage " << stage << endl;
            // print new host position on the screen and update bb info
        updatePosition();
        coreEV << "Host is moving, speed=" << move.getSpeed() << " updateInterval=" << updateInterval << endl;
        moveMsg = new cMessage("move", MOVE_HOST);
        //host moves the first time after some random delay to avoid synchronized movements
        scheduleAt(AKIGetCurrentSimulationTime() + updateInterval, moveMsg);
    }
}


void AimsunMobility::makeMove()
{
    int S = AKIInfNetNbSectionsANG();
    if (S < 0)
    {
        throw cRuntimeError("AimsunMobility: cannot get sections!");
    }
    
    for (int i = 0; i < S; i++)
    {
        int sid = AKIInfNetGetSectionANGId(i);
        
        int V = AKIVehStateGetNbVehiclesSection(sid, true);
        if (V < 0)
        {
            throw cRuntimeError("AimsunMobility: cannot get vehicles!");
        }
        
        for (int j = 0; j < V; j++)
        {
            InfVeh info = AKIVehStateGetVehicleInfSection(sid, j);
            if (info.report != 0)
            {
                throw cRuntimeError("AimsunMobility: vehicle info error!");
            }
            
            if (info.idVeh == aid)
            {
                vi = &info;
            }
        }
    }
    
    // vehicle in junctions...
    int J = AKIInfNetNbJunctions();
    if (J < 0)
    {
        throw cRuntimeError("AimsunMobility: cannot get junctions!");
    }
    
    for (int i = 0; i < J; i++)
    {
        int jid = AKIInfNetGetJunctionId(i);
        
        int V = AKIVehStateGetNbVehiclesJunction(jid);
        if (V < 0)
        {
            throw cRuntimeError("AimsunMobility: cannot get vehicles!");
        }
        
        for (int j = 0; j < V; j++)
        {
            InfVeh info = AKIVehStateGetVehicleInfJunction(jid, j);
            if (info.report != 0)
            {
                throw cRuntimeError("AimsunMobility: vehicle info error!");
            }
            
            if (info.idVeh == aid)
            {
                vi = &info;
            }
        }
    }
    
    if (vi == NULL)
    {
        throw cRuntimeError("AimsunMobility: no vehicle info!");
    }

    Coord start = Coord(vi->xCurrentPosBack,
                        vi->yCurrentPosBack,
                        vi->zCurrentPosBack > 0.0 ? vi->zCurrentPosBack : 0.0);
    Coord end = Coord(vi->xCurrentPos,
                      vi->yCurrentPos,
                      vi->zCurrentPos > 0.0 ? vi->zCurrentPos : 0.0);
    end = Coord(end.getX()/sqrt(end.squareLength()),
                end.getY()/sqrt(end.squareLength()),
                end.getZ()/sqrt(end.squareLength()));

    move.setStart(start, simTime());
    move.setSpeed(vi->CurrentSpeed * (1000.0 / 3600.0));
    move.setDirectionByVector(end);
    
    //check whether position is within the playground
    if (!move.getStartPos().isInRectangle(Coord(world->use2D()),
        world->getPgs()))
    {
        throw cRuntimeError("Node position exceeds playgroundsize.");
    }
    /*std::ostringstream msg;
    msg << "Move " << aid << " @ " << simTime();
    AimsunPrint(msg.str());*/
}

void AimsunMobility::handleSelfMsg(cMessage * msg)
{
    makeMove();
    updatePosition();
    
    if (!moveMsg->isScheduled())
    {
    	scheduleAt(AKIGetCurrentSimulationTime() + updateInterval, msg);
    } else {
    	delete msg;
    	moveMsg = NULL;
    }
}