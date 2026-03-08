#pragma once

#include <iarduino_Modbus.h>
#include "device.h"

namespace EDWB
{
    class MTD262MB : public Device
    {
        public:
            MTD262MB(ModbusClient& client) : Device(client) {}

            std::pair<bool, bool> isHumanDetected() const;
    };
}
