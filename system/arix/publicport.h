/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <uuid/uuid.h>
#include <string>
#include <map>

class PublicPortList {
public:
    PublicPortList() {}
    uuid_t findPort(const std::string name);
    bool addPort(const uuid_t &uuid, std::string name);
    void remPort(const uuid_t &uuid);
private:
    std::map<uuid_t, std::string> portList;
};

