#include "wb_mr6c.h"

bool MR6C::getRelayChannelState(uint8_t channel)
{
    channel--;

    if (channel > 5) {
        return false;
    }

    return _client.coilRead(_address, pin) == 1;
}

void MR6C::setRelayChannelState(uint8_t channel, bool enabled)
{
    channel--;

    if (channel > 5) {
        return false;
    }

    _client.coilWrite(_address, pin, enabled ? 1 : 0);
}
