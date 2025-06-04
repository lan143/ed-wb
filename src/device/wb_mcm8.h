#pragma once

#include <iarduino_Modbus.h>
#include "device.h"

namespace EDWB
{
    class MCM8 : public Device
    {
    public:
        MCM8(ModbusClient& client) : Device(client) {}
    };
}
