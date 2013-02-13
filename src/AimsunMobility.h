/* -*- mode:c++ -*- ********************************************************
 * file:        ConstSpeedMobility.h
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


#ifndef AIMSUN_MOBILITY_H
#define AIMSUN_MOBILITY_H

#include "aimsun.h"
#undef NONE
#include <BaseMobility.h>


class AimsunMobility : public BaseMobility
{
    int aid;
    InfVeh *vi;

    public:
        /** @brief Initializes mobility model parameters.*/
        virtual void initialize(int);
    
    protected:
        /** @brief Move the host*/
        virtual void makeMove();
        virtual void handleSelfMsg(cMessage*);
};

#endif
