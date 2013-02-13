/* -*- mode:c++ -*- ********************************************************
 * file:        AimsunEnv.cpp
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


#include "AimsunEnv.h"
#include "appreg.h"

#include <sstream>

Register_PerRunConfigOption(CFG_NETWORK, "network", CFG_STRING, "",
							"The name of the network to be simulated. \
							The package name can be omitted if the ini file \
							is in the same directory as the NED file that \
							contains the network.");

Register_OmnetApp("AimsunEnv", AimsunEnv, 30, "Aimsun User Interface");


AimsunEnv::AimsunEnv()
{
	AimsunPrint("AIMSUNENV");
}

int AimsunEnv::run(int argc, char *argv[], cConfiguration *configobject)
{
    args = new ArgList();
    args->parse(argc, argv, "h?f:u:l:c:r:p:n:x:agGv");
    cfg = dynamic_cast<cConfigurationEx *>(configobject);
    if (!cfg)
	{
		std::string msg = "Cannot cast configuration object %s to cConfigurationEx";
		msg += std::string(configobject->getClassName());
        throw AimsunError(msg);
	}
    
	if (simulationRequired()) {
		setup();
		readPerRunOptions();
        
		AimsunPrint("Setting up network object...");
		std::string networkName = configobject->getAsString(CFG_NETWORK);
		AimsunPrint("NETWORK: " + networkName);
		cModuleType *networkType = this->resolveNetwork(networkName.c_str());
		//cModuleType::get(networkName.c_str());
		if (!networkType)
			throw AimsunError("Cannot load network object.");
		simulation.setupNetwork(networkType);
		AimsunPrint("Simulation startRun...");
		simulation.startRun();
	}
    return 0;
}


AimsunEnv::~AimsunEnv()
{
        //delete scheduler;
}


