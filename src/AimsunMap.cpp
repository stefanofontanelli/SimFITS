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

#include "AimsunMap.h"
#undef NONE
#include "omnetpp.h"


AimsunMap::AimsunMap()
{
    AKIPrintString("AIMSUN MAP!");
    
	int n = AKIInfNetNbSectionsANG();
	if (n < 0)
	{
		throw cRuntimeError("Cannot retrieve the number of sections %s", n);
	}

	for (int i = 0; i < n; i++)
	{
		int id = AKIInfNetGetSectionANGId(i);
		if (id <= 0)
		{
			throw cRuntimeError("Cannot retrieve section id: %s", id);
		}
		sections[id] = new Section(id);
	}

	int j = AKIInfNetNbJunctions();
	if (j < 0)
	{
		throw cRuntimeError("Cannot retrieve number of junctions: %s", j);
	}
	
	for (int i = 0; i < j; i++)
	{
		int id = AKIInfNetGetJunctionId(i);
		if (id <= 0)
		{
			throw cRuntimeError("Cannot retrieve junction id: %s", id);
		}
		junctions[id] = new Junction(id);
	}
	
	/*
	for (int i = 0; i < information.nbTurnings; i++)
	{
		int dst_id = AKIInfNetGetIdSectionANGDestinationofTurning(id, i);
		if (dst_id <= 0)
		{
			throw Exception("Cannot retrieve destination section id",
							dst_id);
		}
	}
	*/
}

InfVeh AimsunMap::getVehInf(int idVeh)
{
    std::map<int, Section*>::iterator sec_iter;
	for (sec_iter = sections.begin(); sec_iter != sections.end(); ++sec_iter)
	{ 
		int nVeh = AKIVehStateGetNbVehiclesSection((*sec_iter).first, true);
        if (nVeh < 0)
        {
            throw cRuntimeError("Cannot get vehicles number inside section %d!",
                                (*sec_iter).first);
        }
        for (int i = 0; i < nVeh; i++)
        {
            InfVeh info = AKIVehStateGetVehicleInfSection((*sec_iter).first, i);
            if (info.report != 0)
            {
                throw cRuntimeError("Cannot get vehicle %d information inside section %d!", 
                                    i, (*sec_iter).first);
            }
            if (info.idVeh == idVeh)
            {
                return info;
            }
        }
	}
    
    std::map<int, Junction*>::iterator jun_iter;
	for (jun_iter = junctions.begin(); jun_iter != junctions.end(); ++jun_iter)
	{
		int nVeh = AKIVehStateGetNbVehiclesJunction((*jun_iter).first);
        if (nVeh < 0)
        {
            throw cRuntimeError("Cannot get vehicles number inside junction %d!",
                                (*jun_iter).first);
        }
        for (int i = 0; i < nVeh; i++)
        {
            InfVeh info = AKIVehStateGetVehicleInfJunction((*jun_iter).first, i);
            if (info.report != 0)
            {
                throw cRuntimeError("Cannot get vehicle %d information inside junction %d!",
                                    i, (*jun_iter).first);
            }
            if (info.idVeh == idVeh)
            {
                return info;
            }
        }
	}
    
    throw cRuntimeError("Cannot find vehicle %d!", idVeh);
}

AimsunMap::~AimsunMap()
{

    //delete_sections();
    //delete_junctions();
	
}

void AimsunMap::delete_sections()
{
	std::map<int, Section*>::iterator iter;
	for (iter = sections.begin(); iter != sections.end(); ++iter)
	{
		delete (*iter).second;
	}
}

void AimsunMap::delete_junctions()
{
	std::map<int, Junction*>::iterator iter;
	for (iter = junctions.begin(); iter != junctions.end(); ++iter)
	{
		delete (*iter).second;
	}
}



