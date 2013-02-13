/* -*- mode:c++ -*- ********************************************************
 * file:        AimsunWorldUtility.cpp
 *
 * author:      Stefano Fontanelli
 *
 * copyright:   (C) 2010 Real-Time System Laboratory (Retis Lab) at
 *              Scuola Superiore Sant'Anna, Italy.
 *
 *              This program is free software; you can redistribute it
 *              and/or modify it under the terms of the GNU General Public
 *              License as published by the Free Software Foundation; either
 *              version 2 of the License, or (at your option) any later
 *              version.
 *              For further information see file COPYING
 *              in the top level directory
 ***************************************************************************
 * description: basic world utility class
 *              provides world-required values
 **************************************************************************/


#include "AimsunWorldUtility.h"
#include <sstream>
#include <math.h>
#include "cMessage.h"


Define_Module(AimsunWorldUtility);


void AimsunWorldUtility::initialize(int stage)
{
    if (stage == 0)
    {
        // Set BaseWorldUtility parameters using Aimsun information.
        int units = AKIInfNetGetUnits();
        if (units < 0)
        {
            throw cRuntimeError("Unable to get units of the network.");
        }
        else if (units == 0)
        {
            throw cRuntimeError("Wrong units: you must set metric units.");
        }

        double min_x;
        double min_y;
        double max_x;
        double max_y;
        if (AKIInfNetGetWorldCoordinates(&min_x, &min_y, &max_x, &max_y) < 0)
        {
            throw cRuntimeError("Cannot retrieve AIMSUN World Coordinates!");
        }

        if (min_x < 0 || min_y < 0)
        {
            throw cRuntimeError("Check world coordinates: min_x and min_y cannot be negative.");
        }

        std::ostringstream world;
        world << "Min (x, y): (" << min_x << ", " << min_y << ")\n";
        world << "Max (x, y): (" << max_x << ", " << max_y << ")\n";

        par("playgroundSizeX") = max_x;
        par("playgroundSizeY") = max_y;
        par("playgroundSizeZ") = 1000.0;
        par("use2D") = false;
        par("useTorus") = false;

        junctions = AKIInfNetNbJunctions();
        
        int demand_type = AKIInfNetGetTrafficDemandType();
        
        if (demand_type == 1)
        {
            vehicles = get_OD_matrix_demand();
        }
        else if (demand_type == 2)
        {
            vehicles = get_traffic_state_demand();
        }
        else
        {
            throw cRuntimeError("Cannot get Traffic Demand Type!");
        }

        world << "Junctions: " << junctions << "\n";
        world << "Vehicles: " << vehicles << "\n";
        
        AimsunPrint(world.str());
        
        update_msg = new cMessage("updateSimulationTime",
                                  UPDATE_SIMULATION_TIME);
    }
    
	BaseWorldUtility::initialize(stage);
}

double AimsunWorldUtility::get_OD_matrix_demand()
{
    int centroids = AKIInfNetNbCentroids();
    if (centroids <= 0)
    {
        throw cRuntimeError("Cannot get the total number of centroids!");
    }
    
    int slices = AKIODDemandGetNumSlicesOD();
    if (slices <= 0)
    {
        throw cRuntimeError("Cannot get slices of OD matrix demand!");
    }
    
    double demand = 0.0;
    
    for (int i = 0; i < centroids; ++i)
    for (int j = 0; j < centroids; ++j)
    {
        int src = AKIInfNetGetCentroidId(i);
        int dst = AKIInfNetGetCentroidId(j);
        
        for (int n = 0; n < slices; ++n)
        {
            demand += AKIODDemandGetDemandODPair(src, dst, NULL, n);
        }
    }
    
    return demand;
}

double AimsunWorldUtility::get_traffic_state_demand()
{
    int sections = AKIInfNetNbSectionsANG();
    if (sections < 0)
    {
        throw cRuntimeError("Cannot get sections number!");
    }
    
    int slices = AKIStateDemandGetNumSlices();
    if (slices <= 0)
    {
        throw cRuntimeError("Cannot get slices of Traffic State demand!");
    }
    
    double demand = 0.0;
    
    for (int i = 0; i < sections; ++i)
    {
        int sid = AKIInfNetGetSectionANGId(i);
        for (int n = 0; n < slices; ++n)
        {
            demand += AKIStateDemandGetDemandSection(sid, NULL, n);
        }
    }
    
    double time = (AKIGetEndSimTime() - AKIGetIniSimTime()) / 3600;
    return ceil(demand * time);
}

int AimsunWorldUtility::get_junctions()
{
    return junctions;
}

int AimsunWorldUtility::get_vehicles()
{
    return vehicles;
}

void AimsunWorldUtility::updateSimulationTime(double time)
{
    if( !update_msg->isScheduled() )
    {
        scheduleAt(time, update_msg);
    }
    else
    {
        std::ostringstream msg;
        msg << "MESSAGE @" << time << " is already scheduled. ";
        AimsunPrint(msg.str().c_str());
    }
}