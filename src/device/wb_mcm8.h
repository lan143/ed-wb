#pragma once

#include <iarduino_Modbus.h>
#include "device.h"

namespace EDWB
{
    class MCM8 : public Device
    {
    public:
        MCM8(ModbusClient& client) : Device(client) {}

        bool getInputState(uint8_t pin);
        uint16_t getShortPressCount(uint8_t pin);
        uint16_t getLongPressCount(uint8_t pin);
        uint16_t getDoublePressCount(uint8_t pin);
        uint16_t getShortAndThenLongPressCount(uint8_t pin);

        bool setInputMode(uint8_t pin, uint16_t mode);
    };
}
