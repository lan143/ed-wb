#pragma once

#include <Arduino.h>
#include <iarduino_Modbus.h>

namespace EDWB
{
    class Device
    {
    public:
        Device(ModbusClient& client) : _client(client) {}
        void init(uint8_t address) { _address = address; };

    protected:
        ModbusClient& _client;
        uint8_t _address;
    };
}
