/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef _EXEC_RANDOM_H
#define _EXEC_RANDOM_H

#include <clib/exec_protos.h>
#include <stdint.h>

class RandomNumberGenerator {
    static uint32_t _seed;
    friend void __attribute__((constructor)) ExecInit();
    friend void Spawn(struct Hook * spawnHook);
    static void seed();
public:
    template<class T=uint8_t>
    static T get() {
        _seed = (_seed * 1103515245) + 12345;
        switch(sizeof(T)) {
            case 1:
                return (T)(_seed & 0xff);
                break;
            case 2:
                return (T)(_seed & 0xffff);
                break;
            case 4:
                return (T)(_seed);
                break;
            case 8:
                return (T)(((uint64_t)(get<uint32_t>()) << 32) | get<uint32_t>());
                break;
        }
    }
};

#endif // _EXEC_RANDOM_H