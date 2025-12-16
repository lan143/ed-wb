#include "wb_msw.h"

using namespace EDWB;

Result<float_t> MSW::getTemperature() const
{
    auto val = _client.inputRegisterRead(_address, 0x0004);
    if (val == -1 ) {
        return Result<float_t>(false, 0.0f);
    }

    return Result<float_t>(true, (float_t)val * 0.01);
}

Result<float_t> MSW::getHumidity() const
{
    auto val = _client.inputRegisterRead(_address, 0x0005);
    if (val == -1 ) {
        return Result<float_t>(false, 0.0f);
    }

    return Result<float_t>(true, (float_t)val * 0.01);
}

Result<float_t> MSW::getSoundPressure() const
{
    auto val = _client.inputRegisterRead(_address, 0x0003);
    if (val == -1 ) {
        return Result<float_t>(false, 0.0f);
    }

    return Result<float_t>(true, (float_t)val * 0.01);
}

Result<uint16_t> MSW::getAirQuality() const
{
    auto val = _client.inputRegisterRead(_address, 0x000B);
    if (val == -1 ) {
        return Result<uint16_t>(false, 0);
    }

    return Result<uint16_t>(true, (uint16_t)val);
}

Result<uint16_t> MSW::getMotion() const
{
    auto val = _client.inputRegisterRead(_address, 0x011B);
    if (val == -1 ) {
        return Result<uint16_t>(false, 0);
    }

    return Result<uint16_t>(true, (uint16_t)val);
}

Result<uint16_t> MSW::getCO2Value() const
{
    auto val = _client.inputRegisterRead(_address, 0x0008);
    if (val == -1 ) {
        return Result<uint16_t>(false, 0);
    }

    if (val == 0xFFFF) {
        return Result<uint16_t>(false, 0);
    }

    return Result<uint16_t>(true, (uint16_t)val);
}

bool MSW::enableCO2Sensor(bool enable) const
{
    return _client.coilWrite(_address, 0x0003, enable ? 1 : 0);
}

Result<float_t> MSW::getLightLevel() const
{
    auto part1 = _client.inputRegisterRead(_address, 0x0009);
    if (part1 == -1) {
        return Result<float_t>(false, 0.0f);
    }

    if (part1 == 0xffff) {
        return Result<float_t>(false, 0.0f);
    }

    auto part2 = _client.inputRegisterRead(_address, 0x000A);
    if (part2 == -1) {
        return Result<float_t>(false, 0.0f);
    }

    if (part2 == 0xffff) {
        return Result<float_t>(false, 0.0f);
    }

    uint32_t result = (part1 << 16) | part2;

    return Result<float_t>(false, (float_t)result * 0.01);
}
