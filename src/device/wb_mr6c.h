#pragma once

#include <iarduino_Modbus.h>
#include "device.h"

namespace EDWB
{
    class MR6C : public Device
    {
    public:
        MR6C(ModbusClient& client) : Device(client) {}

        std::pair<bool, bool> getInputChannelState(uint8_t channel) const;
        bool setRelayChannelState(uint8_t channel, bool enabled);

        std::pair<uint16_t, bool> getInputCounter(uint8_t channel);
    };
}
