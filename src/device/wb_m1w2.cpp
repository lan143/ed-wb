#include "wb_m1w2.h"

using namespace EDWB;

Result<float_t> M1W2::getTemperature(uint8_t channel) const
{
    channel--;
    if (channel > 1) {
        return Result<float_t>(false, 0.0f);
    }

    auto val = _client.inputRegisterRead(_address, 0x0007 + channel);
    if (val == -1) {
        return Result<float_t>(false, 0.0f);
    }

    return Result<float_t>(true, (float_t)val * 0.0625f);
}
