#pragma once

#include <iarduino_Modbus.h>
#include "device.h"

namespace EDWB
{
    class MR6C : public Device
    {
    public:
        MR6C(ModbusClient& client) : Device(client) {}

        bool getRelayChannelState(uint8_t channel) const;
        void setRelayChannelState(uint8_t channel, bool enabled);
    };
}
