#include <stdint.h>
#include "publicport.h"
#include <iostream>


bool PublicPortList::addPort(const uuid_t &uuid, std::string name)
{
    bool can_add = true;

std::cout << "[ARIX] PublicPortList::addPort(" << name << ")" << std::endl;

    if (portList.find(uuid) != portList.end()) {
        can_add = false;
    }
    else {
        for (auto v: this->portList) {
            if (v.second == name) {
                can_add = false;
                break;
            }
        }
    }

std::cout << "[ARIX] can_add=" << can_add << std::endl;

    if (can_add) {
        portList[uuid] = name;
    }

    return can_add;
}

void PublicPortList::remPort(const uuid_t &uuid)
{
std::cout << "[ARIX] PublicPortList::remPort()" << std::endl;

    portList.erase(uuid);
}

uuid_t PublicPortList::findPort(const std::string name)
{
    uuid_t uuid = MAKE_UUID(0, 0, 0, 0, 0);

    std::cout << "[ARIX] PublicPortList::findPort(" << name << ")" << std::endl;

    for (auto it: portList) {
        if (it.second == name) {
            uuid = it.first;
            break;
        }
    }
    return uuid;
}