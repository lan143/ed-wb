#pragma once

#include <iarduino_Modbus.h>
#include "device.h"

namespace EDWB
{
    class MSW : public Device
    {
    public:
        MSW(ModbusClient& client) : Device(client) {}

        float_t getTemperature() const;
        float_t getHumidity() const;
        float_t getSoundPressure() const;
        int16_t getAirQuality() const;
        uint16_t getMotion() const;

        int16_t getCO2Value() const;
        bool enableCO2Sensor(bool enable) const;

        float_t getLightLevel() const;
    };
}
