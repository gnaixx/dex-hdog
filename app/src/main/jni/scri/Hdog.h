//
// Created by 薛祥清 on 2017/4/1.
//

#ifndef DEX_HOUND_HDOG_H
#define DEX_HOUND_HDOG_H

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

class Hdog {
public:
    uint32_t getProcessPid(const char *);
    uint32_t getClonePid(uint32_t);
    int attachPid(uint32_t);

};

#endif //DEX_HOUND_HDOG_H
