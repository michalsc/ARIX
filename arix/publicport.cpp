#include <stdint.h>
#include "publicport.h"



bool PublicPortList::addPort(const uuid_t &uuid, std::string name)
{
    bool can_add = true;

    if (portList.find(uuid) != portList.end()) {
        can_add = false;
    }
    else {
        for (auto v: this->portList) {
            if (v.first == uuid && v.second == name) {
                can_add = false;
                break;
            }
        }
    }

    if (can_add) {
        portList[uuid] = name;
    }

    return can_add;
}

void PublicPortList::remPort(const uuid_t &uuid)
{
    portList.erase(uuid);
}

uuid_t PublicPortList::findPort(const std::string name)
{
    uuid_t uuid = MAKE_UUID(0, 0, 0, 0, 0);
    for (auto it: portList) {
        if (it.second == name) {
            uuid = it.first;
            break;
        }
    }
    return uuid;
}