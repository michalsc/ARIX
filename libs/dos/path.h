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
    void setVolume(const std::string & vol) { __volume = vol; }
    void setPath(const std::string & path) { __path = path; }
    const std::string & volume() const { return __volume; }
    const std::string & path() const { return __path; }

    static Path PathFromDOS(const std::string &);
    static Path PathFromUNIX(const std::string &);
};

#endif /* _PATH_H */
