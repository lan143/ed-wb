#include "wb_msw.h"

using namespace EDWB;

float_t MSW::getTemperature() const
{
    uint16_t val = _client.inputRegisterRead(_address, 0x0004);
    if (val != 0x7FFF) {
        return (float_t)val * 0.01;
    }

    return -1000.0f;
}

float_t MSW::getHumidity() const
{
    uint16_t val = _client.inputRegisterRead(_address, 0x0005);
    if (val != 0x7FFF) {
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