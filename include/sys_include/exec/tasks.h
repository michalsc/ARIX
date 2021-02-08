#ifndef EXEC_TASKS_H
#define EXEC_TASKS_H

#include <utility/tagitem.h>

#define TASKTAG_Dummy           (TAG_USER + 0x100000)
#define TASKTAG_ERROR           (TASKTAG_Dummy + 0)
#define TASKTAG_CODETYPE        (TASKTAG_Dummy + 1)
#define TASKTAG_PC              (TASKTAG_Dummy + 2)
#define TASKTAG_FINALPC         (TASKTAG_Dummy + 3)
#define TASKTAG_STACKSIZE       (TASKTAG_Dummy + 4)
#define TASKTAG_NAME            (TASKTAG_Dummy + 6)
#define TASKTAG_USERDATA        (TASKTAG_Dummy + 7)
#define TASKTAG_PRI             (TASKTAG_Dummy + 8)
#define TASKTAG_POOLPUDDLE      (TASKTAG_Dummy + 9)
#define TASKTAG_POOLTHRESH      (TASKTAG_Dummy + 10)
#define TASKTAG_ARG1            (TASKTAG_Dummy + 16)
#define TASKTAG_ARG2            (TASKTAG_Dummy + 17)
#define TASKTAG_ARG3            (TASKTAG_Dummy + 18)
#define TASKTAG_ARG4            (TASKTAG_Dummy + 19)
#define TASKTAG_ARG5            (TASKTAG_Dummy + 20)
#define TASKTAG_ARG6            (TASKTAG_Dummy + 21)
#define TASKTAG_ARG7            (TASKTAG_Dummy + 22)
#define TASKTAG_ARG8            (TASKTAG_Dummy + 23)
#define TASKTAG_STARTUPMSG      (TASKTAG_Dummy + 24)
#define TASKTAG_TASKMSGPORT     (TASKTAG_Dummy + 25)
#define TASKTAG_FLAGS           (TASKTAG_Dummy + 26)
#define TASKTAG_TCBEXTRASIZE    (TASKTAG_Dummy + 28)
#define TASKTAG_AFFINITY        (TASKTAG_Dummy + 29)
#define TASKAFFINITY_BOOT 0
#define TASKAFFINITY_ANY ((IPTR)-1)
#define TASKAFFINITY_ALL_BUT_SELF ((IPTR)-2)

#endif /* EXEC_TASKS_H */
