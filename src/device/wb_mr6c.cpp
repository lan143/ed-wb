#include "wb_mr6c.h"

using namespace EDWB;

std::pair<bool, bool> MR6C::getInputChannelState(uint8_t channel) const
{
    if (channel < 0 || channel > 6) {
        return std::make_pair(false, false);
    }

    uint16_t reg = 0x0000 + channel - 1;
    if (channel == 0) {
        reg = 0x0007;
    }

    auto val = _client.inputRegisterRead(_address, reg);
    if (val == -1) {
        return std::make_pair(false, false);
    }

    return std::make_pair(val != 0, true);
}

bool MR6C::setInputMode(uint8_t channel, MR6CInputMode mode)
{
    if (channel < 0 || channel > 6) {
        return false;
    }

    if (mode > MR6C_INPUT_MODE_MAPPING_BUTTON) {
        return false;
    }

    if (channel == 0 && (mode == MR6C_INPUT_MODE_BUTTON_WITHOUT_LOCKING || mode == MR6C_INPUT_MODE_SWITCH)) {
        return false;
    }

    uint16_t reg = 0x0009 + channel - 1;
    if (channel == 0) {
        reg = 0x0010;
    }

    return _client.holdingRegisterWrite(_address, reg, mode);
}

bool MR6C::setRelayChannelState(uint8_t channel, bool enabled)
{
    channel--;

    if (channel > 5) {
        return false;
    }

    return _client.coilWrite(_address, channel, enabled ? 1 : 0) > 0;
}

std::pair<uint16_t, bool> MR6C::getInputCounter(uint8_t channel)
{
    if (channel < 0 || channel > 6) {
        return std::make_pair(0, false);
    }

    uint16_t reg = 0x0020 + channel - 1;
    if (channel == 0) {
        reg = 0x0027;
    }

    auto val = _client.inputRegisterRead(_address, reg);
    if (val == -1) {
        return std::make_pair(0, false);
    }

    return std::make_pair(val, true);
}
