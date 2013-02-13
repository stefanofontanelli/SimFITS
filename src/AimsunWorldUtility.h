/* -*- mode:c++ -*- ********************************************************
 * file:        AimsunWorldUtility.h
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

#ifndef AIMSUN_WORLD_UTIL_H
#define AIMSUN_WORLD_UTIL_H


#include "aimsun.h"
#undef NONE
#include "BaseWorldUtility.h"


class AimsunWorldUtility : public BaseWorldUtility
{
    enum AimsunWorldUtilityMsgKinds
    {
        UPDATE_SIMULATION_TIME = 65000,
		LAST_AIMSUN_WORLD_UTILITY_KIND,
    };
    
    cMessage *update_msg;
    
    protected:
        int vehicles;
        int junctions;
        double get_OD_matrix_demand();
        double get_traffic_state_demand();
    
	public:
        void initialize(int);
        int get_junctions();
        int get_vehicles();
        void handleMessage(cMessage*) {}
        void updateSimulationTime(double);
};

#endif
