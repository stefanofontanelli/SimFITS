//***************************************************************************
// * file:        Demo.ned
// *
// * author:      Stefano Fontanelli
// *
// * copyright:   (C) 2010 Real-Time Systems Laboratory at
// *              Scuola Superiore Sant'Anna, Pisa, Italy.
// *
// *              This program is free software; you can redistribute it 
// *              and/or modify it under the terms of the GNU General Public 
// *              License as published by the Free Software Foundation;
// *              either version 2 of the License, or (at your option) any 
// *              later version.
// *              For further information see file COPYING 
// *              in the top level directory.
// **************************************************************************
// * part of:     ReVANS
// * description: 
// *              
// **************************************************************************

package org.revans;

import org.mixim.base.connectionManager.ConnectionManager;


network Demo
{
    submodules:
        world: AimsunWorldUtility;
        connectionManager: ConnectionManager;
        hostUtility: HostUtility;

    connections allowunconnected:
    // all connections and gates are to be generated dynamically

}
