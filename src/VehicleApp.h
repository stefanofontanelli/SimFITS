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

#ifndef VEHICLE_APP_H_
#define VEHICLE_APP_H_

#include "aimsun.h"
#undef NONE
#include "BaseApplLayer.h"


class VehicleApp : public BaseApplLayer
{
    public:

        /** @brief The message kinds used by this layer.*/
        enum VehicleAppMessageKinds {
            /** @brief Schedules sending of a new message.*/
            SEND_PACKET_TIMER = LAST_BASE_APPL_MESSAGE_KIND,
            /** @brief The kind for a packet send by this layer.*/
            V2V_PACKET = LAST_BASE_APPL_MESSAGE_KIND + 1,
            V2R_PACKET = LAST_BASE_APPL_MESSAGE_KIND + 2,
            R2R_PACKET = LAST_BASE_APPL_MESSAGE_KIND + 3,
            R2V_PACKET = LAST_BASE_APPL_MESSAGE_KIND + 4,
            /** @brief Sub classing layers shoudl begin their own kinds at this value.*/
            LAST_VEHICLE_APP_MESSAGE_KIND
        };

    protected:
    
        int aid;
        int start_section;
        int end_section;
        double tx_period;
        int V2V_tx;
        int V2V_rx;
        int V2R_tx;
        int R2V_rx;
        /** @brief Timer message to schedule next packet send.*/
        cMessage *delayTimer;

    public:
        virtual ~VehicleApp();
        /** @brief Omnet++ Initialisation.*/
        virtual void initialize(int stage);
        /** @brief Called at the end of the simulation to record statistics.*/
        virtual void finish();

    protected:
        /** @brief Handle self messages such as timer... */
        virtual void handleSelfMsg(cMessage *msg);
        /** @brief Handle messages from lower layer */
        virtual void handleLowerMsg(cMessage *msg);
        /** @brief Send a broadcast message to lower layer. */
        virtual void sendBroadcast();
        /** @brief send a reply to a broadcast message */
        void sendReply(ApplPkt *msg);
};

#endif
