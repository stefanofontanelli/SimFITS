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
#undef NONE
#include "omnetpp.h"
#include "Section.h"

Section::Section(int id)
{

	information = AKIInfNetGetSectionANGInf(id);
	
	if (information.report != 0)
	{
		throw cRuntimeError("Cannot retrieve section information: %s",
							information.report);
	}
	
	/*
	std::stringstream msg;
	msg << "Section ANG ID: " << information.angId;
	std::string str = msg.str();
	char *cstr = new char [str.size() + 1];
	strcpy(cstr, str.c_str());
	AKIPrintString(cstr);
	 */
}
