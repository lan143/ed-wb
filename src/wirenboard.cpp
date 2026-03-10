#include "wirenboard.h"

using namespace EDWB;

void WirenBoard::init(uint32_t timeout)
{
    _client.begin();
    _client.setTypeMB(MODBUS_RTU);
    _client.setTimeout(timeout);
}

MCM8* WirenBoard::addMCM8(uint8_t address)
{
    MCM8* obj = new MCM8(_client);
    obj->init(address);
    _devices.push_back(obj);

    return obj;
}

MR6C* WirenBoard::addMR6C(uint8_t address)
{
    MR6C* obj = new MR6C(_client);
    obj->init(address);
    _devices.push_back(obj);

    return obj;
}

MSW* WirenBoard::addMSW(uint8_t address)
{
    MSW* obj = new MSW(_client);
    obj->init(address);
    _devices.push_back(obj);

    return obj;
}

LED* WirenBoard::addLED(uint8_t address)
{
    LED* obj = new LED(_client);
    obj->init(address);
    _devices.push_back(obj);

    return obj;
}

M1W2* WirenBoard::addM1W2(uint8_t address)
{
    M1W2* obj = new M1W2(_client);
    obj->init(address);
    _devices.push_back(obj);

    return obj;
}

MS* WirenBoard::addMS(uint8_t address)
{
    MS* obj = new MS(_client);
    obj->init(address);
    _devices.push_back(obj);

    return obj;
}

MTD262MB* WirenBoard::addMTD262MB(uint8_t address)
{
    MTD262MB* obj = new MTD262MB(_client);
    obj->init(address);
    _devices.push_back(obj);

    return obj;
}

QDY30A* WirenBoard::addQDY30A(uint8_t address)
{
    QDY30A* obj = new QDY30A(_client);
    obj->init(address);
    _devices.push_back(obj);

    return obj;
}

MAI6* WirenBoard::addMAI6(uint8_t address)
{
    MAI6* obj = new MAI6(_client);
    obj->init(address);
    _devices.push_back(obj);

    return obj;
}

bool WirenBoard::changeSpeed(uint32_t newSpeed)
{
    return _client.holdingRegisterWrite(0, 0x006E, newSpeed / 100);
}
