#ifndef _ARIXFILE_H
#define _ARIXFILE_H

#include <uuid/uuid.h>
#include <unordered_map>

enum EntryType {
    Volume, Assign
};

struct ARIXEntry {
    std::string     ae_Name;
    EntryType       ae_Type;
};

struct ARIXAssign : public ARIXEntry {
    std::string     aa_UnixPath;
};

class ARIXFile {
    std::string __volume;
    std::string __path;
    uuid_t  __id;
    int     __fd;
public:
    ARIXFile(const char * path, int mode);
    static std::string arix2unix(std::string path);

    int     fd() { return __fd; }
    uuid_t  id() { return __id; }
};

extern std::unordered_map<uuid_t, ARIXFile *> *__files;

#endif /* _ARIXFILE_H */
