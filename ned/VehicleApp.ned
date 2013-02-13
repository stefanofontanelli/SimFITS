//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package org.revans;

import org.mixim.base.modules.IBaseApplLayer;

//
// Application layer which generates a certain almost constant
// amount of traffic.
// See class description for details.
//
simple VehicleApp like IBaseApplLayer
{
    parameters:
        int aid;
        int section;
        bool debug; // debug switch
        int headerLength @unit(bit);
        double tx_period @unit(s) = default(5s);
        
    gates:
    	input lowerGateIn; // from lower
        output lowerGateOut; // to lower
        input lowerControlIn; // control from lower
        output lowerControlOut; // control to lower
}
