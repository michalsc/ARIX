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

static inline bool operator==(uuid_t first, uuid_t second) {
    union {
        uint64_t a[2];
        uuid_t u;
    } u1, u2;

    u1.u = first;
    u2.u = second;

    return (u1.a[0] == u2.a[0] && u1.a[1] == u2.a[1]);
}

static inline bool operator<(uuid_t first, uuid_t second) {
    union {
        uint64_t a[2];
        uuid_t u;
    } u1, u2;

    u1.u = first;
    u2.u = second;

    return (u1.a[0] < u2.a[0] || ((u1.a[0] == u2.a[0]) && (u1.a[1] < u2.a[1])));
}
