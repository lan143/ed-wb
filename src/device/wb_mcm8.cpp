#include "wb_mcm8.h"

using namespace EDWB;

bool MCM8::getInputState(uint8_t pin)
{
    pin--;

    if (pin > 7) {
        return false;
    }

    return _client.discreteInputRead(_address, pin) == 1;
}

bool MCM8::setInputMode(uint8_t pin, uint16_t mode)
{
    pin--;

    if (pin > 7) {
        return false;
    }

    if (mode > 1) {
        return false;
    }

    return _client.holdingRegisterWrite(_address, 0x0009 + pin, mode);
}
