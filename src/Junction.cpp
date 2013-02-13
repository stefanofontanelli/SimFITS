/* -*- mode:c++ -*- ********************************************************
 * file:        AimsunMap.h
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

#include <sstream>

#include "aimsun.h"
#include "Junction.h"
#undef NONE
#include "FindModule.h"
#include "HostUtility.h"

Junction::Junction(int id)
{
    char buf[64];
    snprintf(buf, 64, "Create Junction: %d", id);
    AKIPrintString(buf);
	this->id = id;
    //HostUtility *collection = FindModule<HostUtility*>::findGlobalModule();
    //collection->create_static_node(id);
}
