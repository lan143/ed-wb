#include "wb_mr6c.h"

using namespace EDWB;

std::pair<bool, bool> MR6C::getRelayChannelState(uint8_t channel) const
{
    channel--;

    if (channel > 5) {
        return std::make_pair(false, false);
    }

    auto val = _client.coilRead(_address, channel);
    if (val == -1) {
        return std::make_pair(false, false);
    }

    return std::make_pair(val != 0, true);
}

bool MR6C::setRelayChannelState(uint8_t channel, bool enabled)
{
    channel--;

    if (channel > 5) {
        return false;
    }

    return _client.coilWrite(_address, channel, enabled ? 1 : 0) > 0;
}

std::pair<uint16_t, bool> MR6C::getDigitalInput0Counter()
{
    auto val = _client.inputRegisterRead(_address, 0x0027);
    if (val == -1) {
        return std::make_pair(0, false);
    }

    return std::make_pair(val, true);
}

std::pair<uint16_t, bool> MR6C::getInputCounter(uint8_t channel)
{
    if (channel < 1 || channel > 6) {
        return std::make_pair(0, false);
    }

    auto val = _client.inputRegisterRead(_address, 0x0020 + channel - 1);
    if (val == -1) {
        return std::make_pair(0, false);
    }

    return std::make_pair(val, true);
}
