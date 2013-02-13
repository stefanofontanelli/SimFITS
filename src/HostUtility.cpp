/* -*- mode:c++ -*- ********************************************************
 * file:        HostUtility.cpp
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

#include "HostUtility.h"
#include "AimsunWorldUtility.h"
#include "AimsunMobility.h"
#include "FindModule.h"
#include "BaseConnectionManager.h"
#include <sstream>

Define_Module(HostUtility);


void HostUtility::initialize(int stage)
{
	cCompoundModule::initialize(stage);
    
    if (stage == 0)
    {
        index = 0;
        vehicles = new std::map<int, cModule*>;
        junctions = new std::map<int, cModule*>;
	}
}


void HostUtility::create_junction_host(int aid)
{
    AimsunWorldUtility *world = NULL;
    world = FindModule<AimsunWorldUtility*>::findGlobalModule();
    if(!world)
    {
        throw cRuntimeError("Cannot get Aimsun World Utility!");
    }
    int vector_size = world->get_vehicles() + world->get_junctions();
    
    try
    {
        const unsigned short *attr_name = NULL;
        attr_name = AKIConvertFromAsciiString("GKNode:omnetpp_module");
        void *attr = ANGConnGetAttribute(attr_name);
        const unsigned short *name = NULL;
        name = ANGConnGetAttributeValueString(attr, aid);
        
        bool anyNonAsciiChar = false;
        std::string module_name = AKIConvertToAsciiString(name, false,
                                                          &anyNonAsciiChar);
        
        const unsigned short *attr_x_name = AKIConvertFromAsciiString("GKNode:x");
        void *attr_x = ANGConnGetAttribute(attr_x_name);
        double x = ANGConnGetAttributeValueDouble(attr_x, aid);
        
        const unsigned short *attr_y_name = AKIConvertFromAsciiString("GKNode:y");
        void *attr_y = ANGConnGetAttribute(attr_y_name);
        double y = ANGConnGetAttributeValueDouble(attr_y, aid);
        
        const unsigned short *attr_z_name = AKIConvertFromAsciiString("GKNode:z");
        void *attr_z = ANGConnGetAttribute(attr_z_name);
        double z = ANGConnGetAttributeValueDouble(attr_z, aid);

        if (module_name != "")
        { 
            cModuleType *moduleType = cModuleType::get(module_name.c_str());
            cModule *module = moduleType->create("hosts",
                                                 this->getParentModule(),
                                                 vector_size,
                                                 index);
            index++;
            // set up parameters and gate sizes before we set up itsubmodules
            module->par("aid") = aid;
            module->par("x") = x;
            module->par("y") = y;
            module->par("z") = z;
            module->finalizeParameters();
            // create internals, and schedule it
            module->buildInside();
            module->scheduleStart(simTime());
            module->callInitialize();
            junctions->insert(std::pair<int, cModule*>(aid, module));
        }
    }
    catch (std::exception& e)
    {
        AimsunPrint(std::string(e.what()));
    }
}

void HostUtility::enter_vehicle(int aid, int sid)
{
    AimsunWorldUtility *world = NULL;
    int vector_size;
    const unsigned short *attr_name = NULL;
    const unsigned short *name = NULL;
    bool anyNonAsciiChar = false;
    int obj_id;
    std::string module_name = "";
    
    world = FindModule<AimsunWorldUtility*>::findGlobalModule();
    vector_size = world->get_vehicles() + world->get_junctions();
    
    try
    {
        StaticInfVeh info = AKIVehGetStaticInf(aid);
        name = AKIVehGetVehTypeName(info.type);
        obj_id = ANGConnGetObjectId(name, false);
        attr_name = AKIConvertFromAsciiString("GKVehicle::omnetpp_module");
        name = ANGConnGetAttributeValueString(ANGConnGetAttribute(attr_name),
                                              obj_id);
        if (name == NULL)
        {
            throw cRuntimeError("Cannot get ANG Object Attribute.");
        }

        module_name = AKIConvertToAsciiString(name, false,
                                              &anyNonAsciiChar);
        
        if (module_name != "")
        { 
            cModuleType *moduleType = cModuleType::get(module_name.c_str());
            cModule *module = moduleType->create("hosts",
                                                 this->getParentModule(),
                                                 vector_size,
                                                 index);
            index++;
            // set up parameters and gate sizes before we set up itsubmodules
            module->par("aid") = aid;
            module->par("section") = sid;
            module->finalizeParameters();
            // create internals, and schedule it
            module->buildInside();
            module->scheduleStart(simTime());
            module->callInitialize();
            vehicles->insert(std::pair<int, cModule*>(aid, module));
        }
    }
    catch (std::exception& e)
    {
        AimsunPrint(std::string(e.what()));
    }
}

void HostUtility::exit_vehicle(int aid, int sid)
{
     std::map<int, cModule*>::const_iterator iter;
     for (iter=vehicles->begin(); iter != vehicles->end(); ++iter)
     {
         if (iter->first != aid)
         {
             continue;
         }
         BaseConnectionManager* cm = FindModule<BaseConnectionManager*>::findGlobalModule();
         cModule* nic = iter->second->getSubmodule("nic");
         if (nic != NULL)
         {
             cm->unregisterNic(nic);
         }
         iter->second->callFinish();
         iter->second->deleteModule();
         vehicles->erase(aid);
     }
}