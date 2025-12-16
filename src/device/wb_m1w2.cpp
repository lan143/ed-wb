#include "wb_m1w2.h"

using namespace EDWB;

float_t M1W2::getTemperature(uint8_t channel) const
{
    channel--;
    if (channel > 1) {
        return -1000.0f;
    }

    int32_t val = _client.inputRegisterRead(_address, 0x0007 + channel);
    if (val == 0x7FFF) {
        return -1000.0f;
    }

    return (float_t)val * 0.0625;
}
