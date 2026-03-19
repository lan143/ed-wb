#pragma once

#include <iarduino_Modbus.h>
#include "wb_device.h"

namespace EDWB
{
    enum MR6CInputMode : uint8_t
    {
        MR6C_INPUT_MODE_BUTTON_WITHOUT_LOCKING,
        MR6C_INPUT_MODE_SWITCH,
        MR6C_INPUT_MODE_DISABLE_ALL_OUTPUTS,
        MR6C_INPUT_MODE_FREQUENCY,
        MR6C_INPUT_MODE_MAPPING,
        MR6C_INPUT_MODE_DONT_USE,
        MR6C_INPUT_MODE_MAPPING_BUTTON
    };

    class MR6C : public WBDevice
    {
    public:
        MR6C(ModbusClient& client) : WBDevice(client) {}

        std::pair<bool, bool> getInputChannelState(uint8_t channel) const;
        bool setInputMode(uint8_t channel, MR6CInputMode mode);
        std::pair<bool, bool> getRelayChannelState(uint8_t channel);
        bool setRelayChannelState(uint8_t channel, bool enabled);

        std::pair<uint16_t, bool> getInputCounter(uint8_t channel);
    };
}
