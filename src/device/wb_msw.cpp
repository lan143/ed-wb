#include "wb_msw.h"

using namespace EDWB;

float_t MSW::getTemperature() const
{
    uint16_t val = _client.inputRegisterRead(_address, 0x0004);
    if (val != 0x7FFF && val != 0xFFFF) {
        return (float_t)val * 0.01;
    }

    return -1000.0f;
}

float_t MSW::getHumidity() const
{
    uint16_t val = _client.inputRegisterRead(_address, 0x0005);
    if (val != 0x7FFF && val != 0xFFFF) {
        return (float_t)val * 0.01;
    }

    return -1.0f;
}

float_t MSW::getSoundPressure() const
{
    uint16_t val = _client.inputRegisterRead(_address, 0x0003);
    return (float_t)val * 0.01;
}

int16_t MSW::getAirQuality() const
{
    return _client.inputRegisterRead(_address, 0x000B);
}

uint16_t MSW::getMotion() const
{
    return _client.inputRegisterRead(_address, 0x011B);
}

int16_t MSW::getCO2Value() const
{
    uint16_t value = _client.inputRegisterRead(_address, 0x0008);
    if (value == 0xFFFF) {
        return -1;
    }

    return value;
}

bool MSW::enableCO2Sensor(bool enable) const
{
    return _client.coilWrite(_address, 0x0003, enable ? 1 : 0);
}

float_t MSW::getLightLevel() const
{
    uint32_t part1 = _client.inputRegisterRead(_address, 0x0009);
    uint32_t part2 = _client.inputRegisterRead(_address, 0x000A);
    uint32_t result = (part1 << 16) | part2;
    if (result == 0xFFFFFFFF) {
        return -1.0f;
    }

    return (float_t)result * 0.01;
}
