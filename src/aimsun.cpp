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

#include <string>
#include <sstream>

// Aimsun API includes.
#include "aimsun.h"
#include "AimsunWorldUtility.h"
#include "HostUtility.h"

// Omnet++ includes.
#undef NONE
#include "omnetpp.h"
#include "opp_ctype.h"
#include "startup.h"
#include "args.h"
#include "inifilereader.h"
#include "fileutil.h"

#include "distrib.h"
#include "cconfigoption.h"
#include "cexception.h"
#include "sectionbasedconfig.h"
#include "appreg.h"
#include "cmodule.h"
#include "fsutils.h"
#include "fnamelisttokenizer.h"
#include "stringutil.h"
#include "intxtypes.h"
#include "FindModule.h"


Register_GlobalConfigOption(CFGID_LOAD_LIBS, "load-libs", CFG_FILENAMES, "", "A space-separated list of dynamic libraries to be loaded on startup. The libraries should be given without the `.dll' or `.so' suffix -- that will be automatically appended.");
Register_GlobalConfigOption(CFGID_CONFIGURATION_CLASS, "configuration-class", CFG_STRING, "", "Part of the Envir plugin mechanism: selects the class from which all configuration information will be obtained. This option lets you replace omnetpp.ini with some other implementation, e.g. database input. The simulation program still has to bootstrap from an omnetpp.ini (which contains the configuration-class setting). The class should implement the cConfigurationEx interface.");
Register_GlobalConfigOption(CFGID_USER_INTERFACE, "user-interface", CFG_STRING, "", "Selects the user interface to be started. Possible values are Cmdenv and Tkenv. This option is normally left empty, as it is more convenient to specify the user interface via a command-line option or the IDE's Run and Debug dialogs. New user interfaces can be defined by subclassing cRunnableEnvir.");

#define CREATE_BY_CLASSNAME(var,classname,baseclass,description) \
baseclass *var ## _tmp = (baseclass *) createOne(classname); \
var = dynamic_cast<baseclass *>(var ## _tmp); \
if (!var) \
throw cRuntimeError("Class \"%s\" is not subclassed from " #baseclass, (const char *)classname);




cStaticFlag dummy;
cRunnableEnvir *app;
cSimulation *simulationobject;
cConfigurationEx *configobject;
SectionBasedConfiguration *bootconfig;
bool exitWithError = false;
ArgList *args = NULL;

void AimsunPrint(std::string str)
{
    bool debug = true;
    if (debug)
    {
        char *msg = new char[str.size()];
        strcpy(msg, str.c_str());
        AKIPrintString(msg);
        delete[] msg;
    }
}

void AimsunCancelSimulation(std::string error)
{
	exitWithError = true;
	ANGSetSimulationOrder(1, 0);
	AimsunPrint(error);
}

int AAPILoad()
{
    //freopen("file_out.txt", "a+", stdout);
    //freopen("file_err.txt", "a+", stderr);
	AimsunPrint("Aimsun API loaded!");
	return 0;
}

int AAPIInit()
{	
	try
	{
		AimsunPrint("Aimsun API initializing ...");
		ANGConnEnableVehiclesInBatch(true);
		bool anyNonAsciiChar = false;
		const char *path = AKIConvertToAsciiString(AKIInfNetGetNetworkPath(),
												   true,
												   &anyNonAsciiChar);
		const char *net = AKIConvertToAsciiString(AKIInfNetGetNetworkName(),
												  true,
												  &anyNonAsciiChar);
		std::string basePath = path;
		basePath += "/";
		basePath += net;
		basePath += "_cfg/";
		
		// construct global lists
        ExecuteOnStartup::executeAll();
		
        // verify definitions of int64, int32, etc.
        //verifyIntTypes();
		
		int argc = 0;
		char *c = new char[argc + 1];
		char **argv = &c;
		
        // args
        args = new ArgList();
        args->parse(argc, argv, "h?f:u:l:c:r:p:n:x:agGv");
		
		// First, load the ini file.
		// It might contain the name of the user interface
        // to instantiate.
		
		std::string default_file = basePath + std::string("omnetpp.ini");
		AimsunPrint(std::string("INI file: ") + default_file);
		
		// 1st '-f filename' option
        const char *fname = args->optionValue('f', 0);
		// first argument
        if (!fname) fname = args->argument(0);
		// or default filename
        if (!fname) fname = default_file.c_str();
		
        // when -h or -v is specified,
		// be forgiving about nonexistent omnetpp.ini
        InifileReader *inifile = new InifileReader();
        if ((!args->optionGiven('v') && !args->optionGiven('h')) || fileExists(fname))
            inifile->readFile(fname);
		
        // process additional '-f filename' options or 
		// arguments if there are any
        for (int k=1; (fname=args->optionValue('f', k))!=NULL; k++)
            inifile->readFile(fname);
        for (int k=(args->optionValue('f',0) ? 0 : 1); (fname=args->argument(k))!=NULL; k++)
            inifile->readFile(fname);
		
        // activate [General] section 
		// so that we can read global settings from it
        bootconfig = new SectionBasedConfiguration();
        bootconfig->setConfigurationReader(inifile);
        bootconfig->setCommandLineConfigOptions(args->getLongOptions());
        bootconfig->activateConfig("General", 0);
		
		
        //
        // Load all libraries specified on the command line ('-l' options),
        // and in the configuration [General]/load-libs=.
        // (The user interface library also might be among them.)
        //
        const char *libname;
        for (int k=0; (libname=args->optionValue('l',k))!=NULL; k++)
            loadExtensionLibrary(libname);
        std::vector<std::string> libs = bootconfig->getAsFilenames(CFGID_LOAD_LIBS);
        for (int k=0; k<(int)libs.size(); k++)
            loadExtensionLibrary(libs[k].c_str());
		
		//
        // Create custom configuration object, if needed.
        //
        std::string configclass = bootconfig->getAsString(CFGID_CONFIGURATION_CLASS);
        if (configclass.empty())
        {
            configobject = bootconfig;
        }
        else
        {
            // create custom configuration object
            CREATE_BY_CLASSNAME(configobject, configclass.c_str(), cConfigurationEx, "configuration");
            configobject->initializeFrom(bootconfig);
            delete bootconfig;
            bootconfig = NULL;
			
            // load libs from this config as well
            std::vector<std::string> libs = configobject->getAsFilenames(CFGID_LOAD_LIBS);
            for (int k=0; k<(int)libs.size(); k++)
                loadExtensionLibrary(libs[k].c_str());
        }
		
        // validate the configuration, but make sure we don't report cmdenv-* keys
        // as errors if Cmdenv is absent; same for Tkenv.
        std::string ignorablekeys;
        if (omnetapps.getInstance()->lookup("Cmdenv")==NULL)
            ignorablekeys += " cmdenv-*";
        if (omnetapps.getInstance()->lookup("Tkenv")==NULL)
            ignorablekeys += " tkenv-*";
        configobject->validate(ignorablekeys.c_str());
		
        // Choose and set up user interface (EnvirBase subclass). 
		// Everything else will be done by the user interface class.
		
        // was it specified explicitly which one to use?
		// 1st '-u name' option
        std::string appname = opp_nulltoempty(args->optionValue('u',0));
        if (appname.empty())
            appname = configobject->getAsString(CFGID_USER_INTERFACE);
		
        cOmnetAppRegistration *appreg = NULL;
        if (!appname.empty())
        {
            // look up specified user interface
            appreg = static_cast<cOmnetAppRegistration *>(omnetapps.getInstance()->lookup(appname.c_str()));
            if (!appreg)
            {
				std::string msg = "User interface '" + appname + 
								  "'not found (not linked in or loaded dynamically).";
				AimsunPrint(msg);
				AimsunPrint("Available ones are:");
				cRegistrationList *a = omnetapps.getInstance();
                for (int i=0; i<a->size(); i++)
				{
					msg = std::string(a->get(i)->getName()) + " : ";
					msg += a->get(i)->info();
					AimsunPrint(msg);
				}
				AimsunPrint("Could not start user interface");
				msg = "Could not start user interface: ";
				msg += appname;
                throw AimsunError(msg);
            }
        }
        else
        {
            // user interface not explicitly selected: pick one from what we have
            appreg = cOmnetAppRegistration::chooseBest();
            if (!appreg)
			{
                throw AimsunError("No user interface (Cmdenv, Tkenv, etc.) found");
			}
        }
		
        //
        // Create interface object.
        //
		std::string msg = std::string("Setting up ") + std::string(appreg->getName());
        AimsunPrint(msg);
        app = appreg->createOne();
		
		if (app)
		{
			AimsunPrint("Setting up active simulation object...");
			simulationobject = new cSimulation("simulation", app);
			cSimulation::setActiveSimulation(simulationobject);
			app->run(argc, argv, configobject);
		}
		else
		{
			throw AimsunError("Cannot set active simulation object.");
		}
		
		AimsunPrint("Aimsun API initializing ended.");
	}
	catch (std::exception& e)
	{
		if (app)
        {
            delete app;
            app = NULL;
        }
        else
        {
            // normally, these two get deleted by app
            delete args;
            delete bootconfig;
        }
		AimsunCancelSimulation(std::string(e.what()));
	}
    
    AimsunPrint("Adding junctions hosts...");
    
    int j = AKIInfNetNbJunctions();
    if (j < 0)
    {
        throw cRuntimeError("Cannot retrieve number of junctions: %s", j);
    }
    
    for (int i = 0; i < j; i++)
    {
        int aid = AKIInfNetGetJunctionId(i);
        
        if (aid <= 0)
        {
            throw cRuntimeError("Cannot retrieve junction id: %s", aid);
        }
        
        HostUtility *util = FindModule<HostUtility*>::findGlobalModule();
        util->create_junction_host(aid);
    }
    
    AimsunPrint("Synchronizing simulation clocks...");
    
    try
    {
        AimsunWorldUtility *w = FindModule<AimsunWorldUtility*>::findGlobalModule();
        w->updateSimulationTime(0.0);
    }
    catch (std::exception& e)
    {
        AimsunPrint(std::string(e.what()));
    }
    
	return 0;
}

int AAPIManage(double time, double timeSta, double timTrans, double acicle)
{
    return 0;
}

int AAPIPostManage(double time, double timeSta, double timTrans, double acicle)
{
    
    try
    {
        AimsunWorldUtility *w = FindModule<AimsunWorldUtility*>::findGlobalModule();
        w->updateSimulationTime(time);
    }
    catch (std::exception& e)
    {
        AimsunPrint(std::string(e.what()));
    }
    
    try
	{
		if (simulationobject)
        {
			while (simulationobject->getSimTime() <= time)
			{
				cSimpleModule *mod = simulationobject->selectNextModule();
				if (!mod) { break; }
				simulationobject->doOneEvent(mod);
			}
		}
	}
	catch (cTerminationException& e)
	{
		std::string msg = "Omnet++ finished the simulation: ";
		msg += std::string(e.what());
		AimsunPrint(msg);
	}
	catch (std::exception& e)
	{
		AimsunPrint(e.what());
        //AimsunCancelSimulation(std::string(e.what()));
	}

	return 0;
}

int AAPIFinish()
{
	if (simulationobject)
    {
		if (!exitWithError)
        {
			simulationobject->callFinish();
        }
		simulationobject->endRun();
		simulationobject->deleteNetwork();
	}
	AimsunPrint("Aimsun API simulation completed.");
	return 0;
}

int AAPIUnLoad()
{
	try
	{
		cSimulation::setActiveSimulation(NULL);
		delete simulationobject;  // will delete app as well
		
		componentTypes.clear();
		nedFunctions.clear();
		classes.clear();
		enums.clear();
		classDescriptors.clear();
		configOptions.clear();
		omnetapps.clear();
		cSimulation::clearLoadedNedFiles();
	}
	catch (std::exception& e)
	{
		AimsunPrint(std::string(e.what()));
	}
	AKIPrintString("Aimsun API has been unloaded.");
	return 0;
}

int AAPIEnterVehicle(int idVeh, int idSection)
{
    try
    {
        HostUtility *util = FindModule<HostUtility*>::findGlobalModule();
        util->enter_vehicle(idVeh, idSection);
    }
    catch (std::exception& e)
    {
        AimsunPrint(std::string(e.what()));
    }
    return 0;
}

int AAPIExitVehicle (int idVeh, int idSection)
{
    try
    {
        HostUtility *util = FindModule<HostUtility*>::findGlobalModule();
        util->exit_vehicle(idVeh, idSection);
    }
    catch (std::exception& e)
    {
        AimsunPrint(std::string(e.what()));
    }
    return 0;
}

char* AAPIInternalName()
{
	AKIPrintString("Aimsun API get Internal Name.");
	return "Revans";
}

int AAPIPreRouteChoiceCalculation(double time, double timeSta)
{
	AKIPrintString("Aimsun API Route Choice Calculation.");
	return 0;
}
