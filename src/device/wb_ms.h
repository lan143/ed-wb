#pragma once

#include <iarduino_Modbus.h>
#include "device.h"

namespace EDWB
{
    class MS : public Device
    {
    public:
        MS(ModbusClient& client) : Device(client) {}

        std::pair<float_t, bool> getTemperature() const;
        std::pair<float_t, bool> getHumidity() const;
        std::pair<float_t, bool> getOneWireTemperature(uint8_t channel) const;
        std::pair<uint16_t, bool> getAirQuality() const;

        std::pair<uint16_t, bool> getLightLevel() const;
    };
}
