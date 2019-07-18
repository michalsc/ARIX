#!/usr/bin/python
#
# Copyright (C) 2018 Michal Schulz <michal.schulz@gmx.de>
# https://github.com/michalsc
#
# This Source Code Form is subject to the terms of the
# Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
# with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

import sys
import os
import json
import datetime
import string

def read_config(config_file):
    with open(config_file) as f:
        config = json.load(f)
    return config

def generate_basefile(cfg):
    print("""// Automatically generated file.
#include <exec/types.h>
#include <exec/libraries.h>
#include <clib/{0}_protos.h>""".format(cfg["configuration"]["name"]))
    for cdef in cfg["cdef"]:
        print(cdef)
    print("")
    print("static const char __attribute__((used, section(\".text\"))) __idString[] = \"\\0$VER: {0}.{1} {2}.{3} \" VERSION_STRING_DATE \"\\0\";".format(
        cfg["configuration"]["name"], cfg["configuration"]["type"],
        cfg["configuration"]["version.major"], cfg["configuration"]["version.minor"]
    ))
    print("")
    for func in cfg["functions"]:
        print(func[0] + " " + func[1] + "(" + string.join(func[2:], ", ")
        + ");")
    print("")
    print("static const struct " + cfg["configuration"]["base.name"] + "LVO __lvo = {")
    for func in cfg["functions"]:
        print("    " + func[1] + ",")
    print("};")
    print("static const char __attribute__((used)) __versionString[] = \"{0} {2}.{3} \" VERSION_STRING_DATE;".format(
        cfg["configuration"]["name"], cfg["configuration"]["type"],
        cfg["configuration"]["version.major"], cfg["configuration"]["version.minor"]
    ))
    print("""
static const struct Library _{0} = {{
    (const APTR)&__lvo, 
    {1},
    {2},
    (const APTR)__versionString,
}};

const struct Library * LibBase __attribute__((weak)) = &_{0};
const struct Library * {0} = &_{0};
""".format(cfg["configuration"]["base.name"], str(cfg["configuration"]["version.major"]),
str(cfg["configuration"]["version.minor"])))

def generate_proto(cfg):
    print("""#ifndef PROTO_{0}_H
#define PROTO_{0}_H

// Automatically generated file.
#include <exec/types.h>
#include <clib/{1}_protos.h>

#if !defined(__NOLIBBASE__) && !defined(__{0}_NOLIBBASE__)
  #if !defined({2})
    extern struct {3} * {2};
  #endif
#endif

#if !defined(NOLIBINLINE) && !defined({0}_NOLIBINLINE)
#   include <inline/{1}.h>
#elif !defined(NOLIBDEFINES) && !defined({0}_NOLIBDEFINES)
#   include <defines/{1}.h>
#endif

#endif /* PROTO_{0}_H */
""".format(
    cfg["configuration"]["name"].upper(),
    cfg["configuration"]["name"],
    cfg["configuration"]["base.name"],
    cfg["configuration"]["base.type"]
))

def generate_clib(cfg):
    print("""#ifndef CLIB_{0}_PROTOS_H
#define CLIB_{0}_PROTOS_H

/*
    Automatically generated file. do not edit!
*/

""".format(cfg["configuration"]["name"].upper()))
    for cdef in cfg["cdef"]:
        print(cdef)
    print("""
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
""")
    print("struct {0}LVO {{".format(cfg["configuration"]["base.name"]))
    for func in cfg["functions"]:
        print("    {0} (*{1})({2});".format(func[0], func[1], string.join(func[2:], ", ")))
    print("};")
    print("")
    start_idx = { "resource": 0, "library": 4, "device": 7 }
    for func in cfg["functions"][start_idx.get(cfg["configuration"]["type"], lambda: 0):]:
        print("{0} {1}({2});".format(func[0], func[1], string.join(func[2:], ", ")))

    print("""
#ifdef __cplusplus
}}
#endif /* __cplusplus */

#endif /* CLIB_{0}_PROTOS_H */
""".format(cfg["configuration"]["name"].upper()))

def generate_inline(cfg):
    print("""#ifndef INLINE_{0}_H
#define INLINE_{0}_H

/*
    Automatically generated file. do not edit!
*/

#include <exec/libraries.h>
""".format(cfg["configuration"]["name"].upper()))
    for cdef in cfg["cdef"]:
        print(cdef)
    print("#include <clib/{0}_protos.h>".format(cfg["configuration"]["name"]))
    print("""
#if !defined(__{0}_LIBBASE)
#   define __{0}_LIBBASE {1}
#endif
""".format(cfg["configuration"]["name"].upper(), cfg["configuration"]["base.name"]))

    start_idx = { "resource": 0, "library": 4, "device": 7 }
    for func in cfg["functions"][start_idx.get(cfg["configuration"]["type"], lambda: 0):]:
        if func[0] == "void":
            ret = ""
        else:
            ret = "return "
        params = func[2:]
        params[:] = [s[s.rfind(" ")+1:] for s in params]
        func_lvo = func[2:]
        func_lvo.append("APTR Base")
        print("""static inline __attribute__((always_inline)) {0} __inline_{1}({2}) {{
    struct Library * lib = (struct Library *)Base;
    {3} ((struct {4}LVO *)lib->lib_LVOTable)->{1}({5});
}}""".format(
    func[0], 
    func[1], 
    string.join(func_lvo, ", "), 
    ret,
    cfg["configuration"]["base.name"],
    string.join(params, ", ")
))
        params_p = ["({0})".format(s) for s in params]
        params_p.append("__{0}_LIBBASE".format(cfg["configuration"]["name"].upper()))
        print("""
#define {0}({1}) \\
    __inline_{0}({2})
""".format(func[1], string.join(params, ", "), string.join(params_p, ", ")
))
    print("""
#endif /* INLINE_{0}_H */
""".format(cfg["configuration"]["name"].upper()))

def generate_defines(cfg):
    print("""#ifndef DEFINES_{0}_H
#define DEFINES_{0}_H

/*
    Automatically generated file. do not edit!
*/

#include <exec/libraries.h>
""".format(cfg["configuration"]["name"].upper()))
    for cdef in cfg["cdef"]:
        print(cdef)
    print("#include <clib/{0}_protos.h>".format(cfg["configuration"]["name"]))
    print("""
#if !defined(__{0}_LIBBASE)
#   define __{0}_LIBBASE {1}
#endif
""".format(cfg["configuration"]["name"].upper(), cfg["configuration"]["base.name"]))
    start_idx = { "resource": 0, "library": 4, "device": 7 }
    for func in cfg["functions"][start_idx.get(cfg["configuration"]["type"], lambda: 0):]:
        if func[0] == "void":
            ret = ""
        else:
            ret = "return "
        params = func[2:]
        params[:] = [s[s.rfind(" ")+1:] for s in params]
        params_p = ["({0})".format(s) for s in params]
        print("""#define __define_{1}_WB({2}, Base) ({{ \\
    struct Library * lib = (struct Library *)Base; \\
    ((struct {4}LVO *)lib->lib_LVOTable)->{1}({5});  \\
}})""".format(
    func[0], 
    func[1], 
    string.join(params, ", "), 
    ret,
    cfg["configuration"]["base.name"],
    string.join(params_p, ", ")
))
        print("""
#define {0}({1}) \\
    __define_{0}_WB({2}, __{3}_LIBBASE)
""".format(func[1], string.join(params, ", "), string.join(params_p, ", "), cfg["configuration"]["name"].upper()
))
    print("""
#endif /* DEFINES_{0}_H */
""".format(cfg["configuration"]["name"].upper()))


if __name__ == "__main__":
    cfg = read_config(sys.argv[2])
    if sys.argv[1] == "basefile":
        generate_basefile(cfg)
    elif sys.argv[1] == "header_clib":
        generate_clib(cfg)
    elif sys.argv[1] == "header_proto":
        generate_proto(cfg)
    elif sys.argv[1] == "header_defines":
        generate_defines(cfg)
    elif sys.argv[1] == "header_inline":
        generate_inline(cfg)
