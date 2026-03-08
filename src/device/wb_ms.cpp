#include "wb_ms.h"

using namespace EDWB;

std::pair<float_t, bool> MS::getTemperature() const
{
    auto val = _client.inputRegisterRead(_address, 0x0004);
    if (val == -1 ) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair((float_t)val * 0.01, true);
}

std::pair<float_t, bool> MS::getHumidity() const
{
    auto val = _client.inputRegisterRead(_address, 0x0005);
    if (val == -1 ) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair((float_t)val * 0.01, true);
}

std::pair<float_t, bool> MS::getOneWireTemperature(uint8_t channel) const
{
    if (channel < 1 || channel > 2) {
        return std::make_pair(0.0f, false);
    }

    auto val = _client.inputRegisterRead(_address, 0x0006 + channel - 1);
    if (val == -1 ) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair((float_t)val * 0.0625, true);
}

std::pair<uint16_t, bool> MS::getAirQuality() const
{
    auto val = _client.inputRegisterRead(_address, 0x000B);
    if (val == -1 ) {
        return std::make_pair(0.0f, false);
    }

    return std::make_pair((float_t)val * 0.01, (uint16_t)val);
}

std::pair<uint16_t, bool> MS::getLightLevel() const
{
    auto val = _client.inputRegisterRead(_address, 0x0002);
    if (val == -1) {
        return std::make_pair(0, false);
    }

    return std::make_pair(val, true);
}
