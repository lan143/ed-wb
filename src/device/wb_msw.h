#pragma once

#include <iarduino_Modbus.h>
#include "device.h"

namespace EDWB
{
    class MSW : public Device
    {
    public:
        MSW(ModbusClient& client) : Device(client) {}

        Result<float_t> getTemperature() const;
        Result<float_t> getHumidity() const;
        Result<float_t> getSoundPressure() const;
        Result<uint16_t> getAirQuality() const;
        Result<uint16_t> getMotion() const;

        Result<uint16_t> getCO2Value() const;
        bool enableCO2Sensor(bool enable) const;

        Result<float_t> getLightLevel() const;
    };
}
