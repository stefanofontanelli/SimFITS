package org.revans;

import org.mixim.base.modules.BaseArp;
import org.mixim.base.modules.BaseMobility;
import org.mixim.base.modules.BaseNetwLayer;
import org.mixim.base.modules.BaseUtility;
import org.mixim.modules.application.BurstApplLayer;
import org.mixim.modules.nic.Nic80211;


module Junction
{
    parameters:
        int aid;
        double x;
        double y;
        double z;
        @display("p=10,10;b=250,250,rect;o=white");

    gates:
        input radioIn; // gate for sendDirect

    submodules:
        blackboard: BaseUtility {
            @display("p=200,75;b=30,25;i=block/blackboard");
        }

        mobility: BaseMobility {
            @display("p=200,150;b=30,25;i=block/cogwheel");
        }
        
        appl: JunctionApp {
            aid = aid;
            x = x;
            y = y;
            z = z;
            @display("p=91,75;b=22,20,rect;i=block/app");
        }

        arp: BaseArp {
            @display("p=200,225;b=30,25;i=block/process");
        }

        net: BaseNetwLayer {
            @display("p=91,150;b=25,20,rect;i=block/layer");
        }

        nic: Nic80211 {
            @display("p=91,225;b=22,20,rect;i=block/ifcard");
        }

    connections:
        nic.upperGateOut --> net.lowerGateIn;
        nic.upperGateIn <-- net.lowerGateOut;
        nic.upperControlOut --> net.lowerControlIn;
        nic.upperControlIn <-- net.lowerControlOut;

        net.upperGateOut --> appl.lowerGateIn;
        net.upperGateIn <-- appl.lowerGateOut;
        net.upperControlOut --> appl.lowerControlIn;
        net.upperControlIn <-- appl.lowerControlOut;

        radioIn --> nic.radioIn;

}