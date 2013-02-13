/* -*- mode:c++ -*- ********************************************************
 * file:        HostUtility.cpp
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
 * description: 
 *              
 **************************************************************************/


#ifndef HOST_UTILITY_H
#define HOST_UTILITY_H

#include <map>
#include "aimsun.h"
#undef NONE
#include "omnetpp.h"


class HostUtility : public cCompoundModule
{
    protected:
        int index;
        std::map<int, cModule*> *vehicles;
        std::map<int, cModule*> *junctions;
    
    public:
        void initialize(int stage);
        void create_junction_host(int);
        void enter_vehicle(int, int);
        void exit_vehicle(int, int);
};

#endif

