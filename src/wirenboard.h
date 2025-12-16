#pragma once

#include <Arduino.h>
#include <iarduino_Modbus.h>
#include <list>

#include "device/device.h"
#include "device/wb_mcm8.h"
#include "device/wb_mr6c.h"
#include "device/wb_msw.h"
#include "device/wb_led.h"
#include "device/wb_m1w2.h"

namespace EDWB
{
    class WirenBoard
    {
    public:
        WirenBoard(Stream& serial) : _client(serial) { }
        void init(uint32_t timeout);

        MCM8* addMCM8(uint8_t address);
        MR6C* addMR6C(uint8_t address);
        MSW* addMSW(uint8_t address);
        LED* addLED(uint8_t address);
        M1W2* addM1W2(uint8_t address);

        bool changeSpeed(uint32_t newSpeed);
        ModbusClient* getClient() { return &_client; }

    private:
        ModbusClient _client;
        std::list<Device*> _devices;
    };
}
