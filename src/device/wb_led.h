#pragma once

#include <iarduino_Modbus.h>
#include "device.h"

namespace EDWB
{
    enum LEDMode : uint16_t
    {
        LED_MODE_WWWW = 0,
        LED_MODE_2WWW = 1,
        LED_MODE_CCTWW = 2,
        LED_MODE_WW2W = 16,
        LED_MODE_2W2W = 17,
        LED_MODE_CCT2W = 18,
        LED_MODE_WWCCT = 32,
        LED_MODE_2WCCT = 33,
        LED_MODE_CCTCCT = 34,
        LED_MODE_RGBW = 256,
        LED_MODE_4W = 512
    };

    enum SafeMode : uint16_t
    {
        SAFE_MODE_DONT_BLOCK_INPUT = 0,
        SAFE_MODE_INPUT_BLOCK_IN = 1,
        SAFE_MODE_DONT_BLOCK_IN = 2
    };

    class LED : public Device
    {
    public:
        LED(ModbusClient& client) : Device(client) {}

        bool isChannelEnabled(uint8_t channel) const;
        void enableChannel(uint8_t channel, bool enabled);

        bool isPairChannelsEnabled(uint8_t pair) const;
        void enablePairChannels(uint8_t pair, bool enabled);

        bool isAllChannelsEnabled() const;
        void enableAllChannels(bool enabled);

        bool isEnabledCCT1() const;
        void enableCCT1(bool enabled);

        bool isEnabledCCT2() const;
        void enableCCT2(bool enabled);

        bool isEnabledRGB() const;
        void enableRGB(bool enabled);

        bool isContinuousRGBChangeEnabled() const;
        void enableContinuousRGBChange(bool enabled);

        uint8_t getChannelBrightness(uint8_t channel) const;
        void setChannelBrightness(uint8_t channel, uint8_t brightness);

        uint8_t getPairChannelsBrightness(uint8_t pair) const;
        void setPairChannelsBrightness(uint8_t pair, uint8_t brightness);

        uint8_t getAllChannelsBrightness() const;
        void setAllChannelsBrightness(uint8_t brightness);

        uint8_t getTemperatureCCT1() const;
        void setTemperatureCCT1(uint8_t temperature);

        uint8_t getBrightnessCCT1() const;
        void setBrightnessCCT1(uint8_t brightness);

        uint8_t getTemperatureCCT2() const;
        void setTemperatureCCT2(uint8_t temperature);

        uint8_t getBrightnessCCT2() const;
        void setBrightnessCCT2(uint8_t brightness);

        uint32_t getRGBColor() const;
        void setRGBColor(uint32_t color);

        void setMode(LEDMode mode);

        bool setInputMode(uint8_t channel, bool isButton) const;

        bool setSafeMode(uint8_t channel, SafeMode mode) const;
    };
}
