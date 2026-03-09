#pragma once

#include <iarduino_Modbus.h>
#include "device.h"

namespace EDWB
{
    class QDY30A : public Device
    {
        public:
            QDY30A(ModbusClient& client) : Device(client) {}

            std::pair<uint8_t, bool> getUnitOfMeasurement() const;
            std::pair<uint8_t, bool> getDotPosition() const;
            std::pair<uint16_t, bool> getLevel() const;
    };
}
