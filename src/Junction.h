/* -*- mode:c++ -*- ********************************************************
 * file:        Junction.h
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


#ifndef JUNCTION_H_
#define JUNCTION_H_

#include "cobject.h"

class Junction : public cObject
{
	private:
		int id;
	public:
		Junction(int);
};

#endif /* JUNCTION_H_ */
