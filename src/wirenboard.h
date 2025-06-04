#pragma once

#include <Arduino.h>
#include <iarduino_Modbus.h>
#include <list>

#include "device/device.h"
#include "device/wb_mcm8.h"
#include "device/wb_msw.h"

namespace EDWB
{
    class WirenBoard
    {
    public:
        WirenBoard(Stream& serial) : _client(serial) { }
        void init(uint32_t timeout);

        MCM8* addMCM8(uint8_t address);
        MSW* addMSW(uint8_t address);

    private:
        ModbusClient _client;
        std::list<Device*> _devices;
    };
}
