#include "qdy30a.h"

using namespace EDWB;

std::pair<uint8_t, bool> QDY30A::getUnitOfMeasurement() const
{
    auto val = _client.holdingRegisterRead(_address, 0x0002);
    if (val == -1) {
        return std::make_pair(0, false);
    }

    return std::make_pair(val, true);
}

std::pair<uint8_t, bool> QDY30A::getDotPosition() const
{
    auto val = _client.holdingRegisterRead(_address, 0x0003);
    if (val == -1) {
        return std::make_pair(0, false);
    }

    return std::make_pair(val, true);
}

std::pair<uint16_t, bool> QDY30A::getLevel() const
{
    auto val = _client.holdingRegisterRead(_address, 0x0004);
    if (val == -1) {
        return std::make_pair(0, false);
    }

    return std::make_pair(val, true);
}
