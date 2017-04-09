//
// Created by 薛祥清 on 2017/4/1.
//

#ifndef DEX_HOUND_HDOG_H
#define DEX_HOUND_HDOG_H

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <iostream>

#define OUTPUT_PATH "/sdcard/Hdog/"

#define MAX_BUFF 1024
#define MAX_NAME_LEN 512

#ifdef HAVE_STDINT_H
#include <stdint.h>    /* C99 */
    typedef uint8_t             u1;
    typedef uint16_t            u2;
    typedef uint32_t            u4;
    typedef uint64_t            u8;
    typedef int8_t              s1;
    typedef int16_t             s2;
    typedef int32_t             s4;
    typedef int64_t             s8;
#else
    typedef unsigned char       u1;
    typedef unsigned short      u2;
    typedef unsigned int        u4;
    typedef unsigned long long  u8;
    typedef signed char         s1;
    typedef signed short        s2;
    typedef signed int          s4;
    typedef signed long long    s8;
#endif

/*
 * define DexHeader
 */
typedef struct DexHeader {
    u1 magic[8];
    u4 checksum;
    u1 signature[20];
    u4 fileSize;
    u4 headerSize;
    u4 endianTag;
    u4 linkSize;
    u4 linkOff;
    u4 mapOff;
    u4 stringIdsSize;
    u4 stringIdsOff;
    u4 typeIdsSize;
    u4 typeIdsOff;
    u4 protoIdsSize;
    u4 protoIdsOff;
    u4 fieldIdsSize;
    u4 fieldIdsOff;
    u4 methodIdsSize;
    u4 methodIdsOff;
    u4 classDefsSize;
    u4 classDefsOff;
    u4 dataSize;
    u4 dataOff;
};

struct DexOptHeader {
    u1  magic[8];
    u4  dexOffset;
    u4  dexLength;
    u4  depsOffset;
    u4  depsLength;
    u4  optOffset;
    u4  optLength;
    u4  flags;
    u4  checksum;
};

typedef struct MemRegion {
    uint64_t start;
    uint64_t end;
    uint64_t len;
    char     name[MAX_NAME_LEN];
};


class Hdog {
private:
    int seekDex(int, MemRegion*, const char*, int);
    int readMem(int, uint64_t, uint32_t, const char *, int );
    int writeMem(const char*, uint64_t, const char*);

public:
    int getProcessPid(const char *);

    int getSubPid(int);

    int attachPid(int);

    int dumpMems(int, int, const char *);
};

#endif //DEX_HOUND_HDOG_H
