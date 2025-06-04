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

MSW* WirenBoard::addMSW(uint8_t address)
{
    MSW* obj = new MSW(_client);
    obj->init(address);
    _devices.push_back(obj);

    return obj;
}
