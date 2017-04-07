//
// Created by 薛祥清 on 2017/4/1.
//


#include "Hdog.h"

using namespace std;

int Hdog::getProcessPid(const char *process) {
    char cmd[MAX_BUFF];
    char buff[MAX_BUFF];
    char targetProcess[MAX_NAME_LEN];
    int targetPid = 0;

    //通过ps查找
    sprintf(cmd, "ps | grep %s", process);
    FILE *fp = popen(cmd, "r");
    if (fp == NULL) {
        printf("Exec popen failed {%d, %s}\n", errno, strerror(errno));
        return 0;
    }
    while (fgets(buff, sizeof(buff), fp) != NULL) {
        //printf("Read buff: %s", buff);
        uint32_t pid = 0;
        sscanf(buff, "%*s\t%d  %*d\t%*d %*d %*x %*x %*c %s", &pid, targetProcess);
        //printf("Read pid:%d, process:%s\n", pid, targetProcess);
        if (strcmp(targetProcess, process) == 0) {
            targetPid = pid;
            break;
        }
    }
    fclose(fp);
    fp = NULL;
    if (targetPid > 0) {
        return targetPid;
    }

    //通过cmdline查找
    char fileName[MAX_NAME_LEN];
    DIR *dirProc;
    if ((dirProc = opendir("/proc")) == NULL) {
        printf("Error: exec opendir failed {%d, %s}\n", errno, strerror(errno));
        return 0;
    }
    struct dirent *dirent;
    while ((dirent = readdir(dirProc)) != NULL) {
        sprintf(fileName, "/proc/%s/cmdline", dirent->d_name);
        fp = fopen(fileName, "r");
        if (fp == NULL) {
            continue;
        }
        fscanf(fp, "%s", targetProcess);
        fclose(fp);
        fp = NULL;

        if (strcmp(targetProcess, process) == 0) {
            targetPid = (uint32_t) atoi(dirent->d_name);
            break;
        }
    }
    closedir(dirProc);
    return targetPid;
}

int Hdog::getSubPid(int targetPid) {
    char taskDirName[MAX_NAME_LEN];
    sprintf(taskDirName, "/proc/%d/task/", targetPid);
    DIR *rootDir = opendir(taskDirName);
    if (rootDir == NULL) {
        printf("Open dir %s failed {%d, %s}\n", taskDirName, errno, strerror(errno));
        return 0;
    }

    struct dirent *dirent = NULL;
    struct dirent *lastDirent = NULL;
    while ((dirent = readdir(rootDir)) != NULL) {
        lastDirent = dirent;
    }
    if (lastDirent == NULL) {
        printf("Error: last dirent is null\n");
        return 0;
    }
    closedir(rootDir);
    return atoi(lastDirent->d_name);
}

int Hdog::attachPid(int pid) {
    char memName[MAX_NAME_LEN];
    sprintf(memName, "/proc/%d/mem", pid);
    long ret = ptrace(PTRACE_ATTACH, pid, NULL, NULL);
    if (ret != 0) {
        printf("Attach %d failed {%d, %s}\n", pid, errno, strerror(errno));
        return 0;
    } else {
        int memFp = open(memName, O_RDONLY);
        if (memFp == 0) {
            printf("Open %s failed: %d, %s\n", memName, errno, strerror(errno));
        }
        return memFp;
    }
}

int Hdog::dumpMems(int clonePid, int memFp, const char *dumpedPath) {
    printf("Scanning dex\n");
    char mapsName[MAX_NAME_LEN];
    sprintf(mapsName, "/proc/%d/maps", clonePid);

    FILE *mapsFp = fopen(mapsName, "r");
    if (mapsFp == NULL) {
        printf("Open %s failed: %d, %s\n", mapsName, errno, strerror(errno));
        return 0;
    }

    char memLine[MAX_BUFF];
    int dexNum = 1;

    char memName[MAX_NAME_LEN];
    char preMemName[MAX_NAME_LEN];
    MemRegion *memRegion = (MemRegion *) malloc(sizeof(MemRegion));
    uint64_t start, end;
    while (fgets(memLine, sizeof(memLine), mapsFp) != NULL) {
        memName[0] = '\0'; //重置为空
        int rv = sscanf(memLine, "%llx-%llx %*s %*s %*s %*s %s\n", &start, &end, memName);
        if (rv < 2) {
            printf("Scanf failed: %d, %s\n", errno, strerror(errno));
            continue;
        } else {
            if (strcmp(preMemName, memName) == 0 && strcmp(memName, "") != 0) continue;
            strcpy(preMemName, memName);
            //printf("%llx-%llx %s\n", start, end, memName);
            memRegion->start = start;
            memRegion->end = end;
            memRegion->len = end - start;
            strcpy(memRegion->name, memName);

            dexNum = seekDex(memFp, memRegion, dumpedPath, dexNum);
            //memRegion->start = start + 8;
            //dexNum = seekDex(memFp, memRegion, dumpedPath, dexNum);
        }
    }
    free(memRegion);
    fclose(mapsFp);
    printf("Scanning end\n");
}

int Hdog::seekDex(int memFp, MemRegion *memRegion, const char *dumpedPath, int dexNum) {
    char dumpedName[MAX_NAME_LEN];

    off64_t off = lseek64(memFp, memRegion->start, SEEK_SET);
    if (off == -1) {
        printf("Lseek %d failed: %d, %s\n", memFp, errno, strerror(errno));
    } else {
        unsigned char *buffer = (unsigned char *) malloc(memRegion->len);
        ssize_t readLen = read(memFp, buffer, memRegion->len);
        if (strncmp((const char *) buffer, "dex\n035\0", 8) == 0) {
            //printf("MemInfo:%s, memLen:%ld, start:%llx, readLen:%ld\n", memRegion->name, memRegion->len, memRegion->start, readLen);
            DexHeader *dexHeader = (DexHeader *) malloc(sizeof(DexHeader));
            memcpy(dexHeader, buffer, sizeof(DexHeader));
            printf("Find %s, fileSize:%x\n", memRegion->name, dexHeader->fileSize);

            if (lseek64(memFp, memRegion->start, SEEK_SET) != -1) {
                char *dexRaw = (char *) malloc(dexHeader->fileSize);
                ssize_t dexSize = read(memFp, dexRaw, dexHeader->fileSize);
                sprintf(dumpedName, "%s/%s%d.dex", dumpedPath, "class", dexNum);
                //printf("xxxx %s, %x, %s\n", dumpedName, dexSize, dexRaw);
                if (writeMem(dexRaw, dexSize, dumpedName) == 1) {
                    dexNum++;
                    printf("Dump %s success\n", dumpedName);
                } else {
                    printf("Dump %s failed\n", dumpedName);
                }
                free(dexRaw);
                dexRaw = NULL;
            } else {
                printf("Lseek %d failed: %d, %s\n", memFp, errno, strerror(errno));
            }
        } else {
            //printf("%s\n", memRegion->name);
            if (strstr(memRegion->name, ".dex") != NULL && strncmp((const char *) buffer, "dey\n036\0", 8) != 0) {
                printf("Ignore %s, %d %d %d %d,%d %d %d %d\n", memRegion->name, buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7]);
            }
        }
        free(buffer);
        buffer = NULL;
    }
    return dexNum;
}

int Hdog::writeMem(const char *dexRaw, uint64_t size, const char dumpedName[]) {
    int res = -1;
    FILE *fp = fopen(dumpedName, "wb");
    if (fwrite(dexRaw, sizeof(char), size, fp) == size) {
        res = 1;
    } else {
        printf("Write %s failed: %d, %s\n", dumpedName, errno, strerror(errno));
    }
    fclose(fp);
    return res;
}


int main(int argc, char *argv[]) {
    int targetPid = 0;
    int attachPid = 0;
    int memFp;
    char dumpedPath[MAX_NAME_LEN];
    const char *packageName = argv[1]; //获取包名
    Hdog hdog;

    targetPid = hdog.getProcessPid(packageName);
    if (targetPid == 0) {
        printf("Can`t find \"%s\"\n", packageName);
        return 0;
    } else {
        attachPid = hdog.getSubPid(targetPid);
        if (attachPid == 0) {
            printf("Get sub pid failed\n");
            return 0;
        } else {
            printf("Target pid:%d, attach pid:%d\n", targetPid, attachPid);
            memFp = hdog.attachPid(attachPid);
            if (memFp == 0) {
                printf("Attach %d failed\n", attachPid);
                return 0;
            } else {
                printf("Attach %d success\n", attachPid);
                sprintf(dumpedPath, "%s%s", OUTPUT_PATH, packageName);
                hdog.dumpMems(attachPid, memFp, dumpedPath);

            }
        }
    }
}