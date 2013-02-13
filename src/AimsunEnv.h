/* -*- mode:c++ -*- ********************************************************
 * file:        AimsunEnv.h
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

#ifndef AIMSUN_ENV_H
#define AIMSUN_ENV_H


#include "aimsun.h"
#undef NONE
#include "omnetpp.h"
#include "cpar.h"
#include "inifilereader.h"
#include "sectionbasedconfig.h"
#include "startup.h"
#include "envirbase.h"
#include <unistd.h>


class AimsunEnv : public EnvirBase
{
	private:
		std::string basePath;
		InifileReader *inifile;
	public:
		cSimulation *sim;

	public:
		AimsunEnv();
		virtual int run(int argc, char *argv[], cConfiguration *configobject);
		~AimsunEnv();
        virtual void putsmsg(const char* a) {
                //std::string msg = std::string(a);
                //AimsunPrint(msg);
        }
        virtual void sputn(const char *s, int n) {
                //std::string msg = std::string(s);
                //AimsunPrint(msg);
        }
		virtual bool askyesno(const char*) {}
		virtual void messageSent_OBSOLETE(cMessage*, cGate*) {}
		virtual unsigned int getExtraStackForEnvir() const {}
		virtual bool isGUI() const { return false; }
		virtual cEnvir& flush() {}
		virtual std::string gets(const char* a, const char* b) {}
		virtual void run() {}
		virtual void printUISpecificHelp() {}
		virtual void askParameter(cPar*, bool) {}
        cEnvir& operator<<(const std::string& t) {}
        template<typename T>
        cEnvir& operator<<(const T &t) {}
};

#endif
