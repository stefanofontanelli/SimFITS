/* -*- mode:c++ -*- ********************************************************
 * file:        AAPI.cpp
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

#ifndef _aimsun_h_
#define _aimsun_h_

#include <string>
#include <exception>

// Aimsun API includes.
#include "AKIProxie.h"
#include "CIProxie.h"
#include "ANGConProxie.h"
#include "AAPI.h"

void AimsunPrint(std::string str);

class AimsunError : public std::exception {
	
	private:
		std::string msg;
	
	public:
		AimsunError(std::string	msg) { this->msg = msg; }
		virtual const char *what() const throw() { return msg.c_str(); }
		virtual ~AimsunError() throw() {}
};

#endif
