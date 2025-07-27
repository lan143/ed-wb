#include "wb_mr6c.h"

using namespace EDWB;

bool MR6C::getRelayChannelState(uint8_t channel) const
{
    channel--;

    if (channel > 5) {
        return false;
    }

    return _client.coilRead(_address, channel) == 1;
}

void MR6C::setRelayChannelState(uint8_t channel, bool enabled)
{
    channel--;

    if (channel > 5) {
        return;
    }

    _client.coilWrite(_address, channel, enabled ? 1 : 0);
}
