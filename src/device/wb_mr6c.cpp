#include <log/log.h>
#include "wb_mr6c.h"

using namespace EDWB;

std::pair<bool, bool> MR6C::getInputChannelState(uint8_t channel) const
{
    if (channel > 6) {
        return std::make_pair(false, false);
    }

    uint16_t reg = 0x0000 + channel - 1;
    if (channel == 0) {
        reg = 0x0007;
    }

    auto val = _client.discreteInputRead(_address, reg);
    if (val == -1) {
        return std::make_pair(false, false);
    }

    return std::make_pair(val != 0, true);
}

bool MR6C::setInputMode(uint8_t channel, MR6CInputMode mode)
{
    if (!_inited) {
        return false;
    }

    if (channel > 6) {
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

std::pair<bool, bool> MR6C::getRelayChannelState(uint8_t channel)
{
    if (!_inited) {
        return std::make_pair(false, false);
    }

    if (channel > 6) {
        return std::make_pair(false, false);
    }

    int8_t val;
    if (_fwVersion < Version{1, 24, 0}) {
        uint16_t reg = 0x0000 + channel - 1;
        val = _client.coilRead(_address, reg);
    } else {
        uint16_t reg = 0x0060 + channel - 1;
        val = _client.discreteInputRead(_address, reg);
    }

    if (val == -1) {
        return std::make_pair(false, false);
    }

    return std::make_pair(val == 1, true);
}

bool MR6C::setRelayChannelState(uint8_t channel, bool enabled)
{
    if (!_inited) {
        return false;
    }

    if (channel > 6) {
        return false;
    }

    auto reg = 0x0000 + channel - 1;

    return _client.coilWrite(_address, reg, enabled ? 1 : 0) > 0;
}

std::pair<uint16_t, bool> MR6C::getInputCounter(uint8_t channel)
{
    if (!_inited) {
        return std::make_pair(0, false);
    }

    if (channel > 6) {
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
