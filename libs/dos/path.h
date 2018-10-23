/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef _PATH_H
#define _PATH_H

#include <vector>
#include <string>

class Path {
    std::string __volume;
    std::string __path;

public:
    static Path PathFromDOS(std::string);
    static Path PathFromUNIX(std::string);
};

#endif /* _PATH_H */
