#pragma once

#include <iarduino_Modbus.h>
#include "device.h"

namespace EDWB
{
    class M1W2 : public Device
    {
    public:
        M1W2(ModbusClient& client) : Device(client) {}

        float_t getTemperature(uint8_t channel) const;
    };
}
