/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include <exec/id.h>
#include <string>
#include <map>

class PublicPortList {
public:
    PublicPortList() {}
    ID findPort(const std::string name);
    bool addPort(const ID &id, std::string name);
    void remPort(const ID &id);
private:
    std::map<ID, std::string> portList;
};

