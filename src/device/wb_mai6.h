#pragma once

#include <iarduino_Modbus.h>
#include "device.h"

namespace EDWB
{
    class MAI6 : public Device
    {
    public:
        MAI6(ModbusClient& client) : Device(client) {}

        std::pair<uint16_t, bool> getSensorTypeP(uint8_t channel);
        bool setSensorTypeP(uint8_t channel, uint16_t type);

        std::pair<uint16_t, bool> getSensorTypeN(uint8_t channel);
        bool setSensorTypeN(uint8_t channel, uint16_t type);

        std::pair<uint32_t, bool> getMeasurementP(uint8_t channel);
        std::pair<uint32_t, bool> getMeasurementN(uint8_t channel);

        std::pair<uint16_t, bool> getCalculatedValueP(uint8_t channel);
        std::pair<uint16_t, bool> getCalculatedValueN(uint8_t channel);

    private:
        bool validateChannel(int8_t channel)
        {
            return channel < 6;
        }
    };
}
