#ifndef MODULETYPE_H
#define MODULETYPE_H

// Possible types. The _END value is there to automatically
// detect the last element and work accordingly.
enum ModuleType
{
    COMMANDER = 0,
    MACROMGR,
    MICROMGR,
    LEARNING,
    _END
};

#endif
