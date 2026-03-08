#include "mtd262mb.h"

std::pair<bool, bool> EDWB::MTD262MB::isHumanDetected() const
{
    auto val = _client.inputRegisterRead(_address, 0x0);
    if (val == -1) {
        return std::make_pair(false, false);
    }

    return std::make_pair(val, true);
}
