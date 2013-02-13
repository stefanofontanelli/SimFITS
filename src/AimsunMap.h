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


#ifndef AIMSUNMAP_H_
#define AIMSUNMAP_H_

#include <map>
#include "aimsun.h"
#include "cobject.h"
#include "Junction.h"
#include "Section.h"

class AimsunMap : public cObject
{
	private:
		std::map<int, Section*> sections;
		std::map<int, Junction*> junctions;
		void delete_sections();
		void delete_junctions();
	
	public:
		AimsunMap();
        InfVeh getVehInf(int);
		~AimsunMap();
};

#endif /* AIMSUNMAP_H_ */