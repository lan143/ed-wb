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

int32_t MCM8::getShortPressCount(uint8_t pin)
{
    pin--;

    if (pin > 7) {
        return -1;
    }

    return _client.inputRegisterRead(_address, 0x01D0 + pin);
}

int32_t MCM8::getLongPressCount(uint8_t pin)
{
    pin--;

    if (pin > 7) {
        return -1;
    }

    return _client.inputRegisterRead(_address, 0x01E0 + pin);
}

int32_t MCM8::getDoublePressCount(uint8_t pin)
{
    pin--;

    if (pin > 7) {
        return -1;
    }

    return _client.inputRegisterRead(_address, 0x01F0 + pin);
}

int32_t MCM8::getShortAndThenLongPressCount(uint8_t pin)
{
    pin--;

    if (pin > 7) {
        return -1;
    }

    return _client.inputRegisterRead(_address, 0x0200 + pin);
}

void MCM8::setDebouncingTime(uint8_t pin, uint16_t timeMs)
{
    pin--;

    if (pin > 7) {
        return;
    }

    if (timeMs > 100) {
        timeMs = 100;
    } 

    _client.holdingRegisterWrite(_address, 0x0014 + pin, timeMs);
}
