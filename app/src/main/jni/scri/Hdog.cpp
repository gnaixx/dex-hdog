//
// Created by 薛祥清 on 2017/4/1.
//


#include "Hdog.h"

uint32_t  Hdog::getProcessPid(const char * process){
    printf("Start >>> getProcessPid");

    char cmd[256];
    char buff[1024];
    char targetProcess[256];
    uint32_t targetPid = -1;

    //通过ps查找
    sprintf(cmd, "ps | grep %s", process);
    FILE *fp = popen(cmd, "r");
    if(fp == NULL){
        printf("Error: exec popen failed {%d, %s}", errno, strerror(errno));
        return -1;
    }
    while(fgets(buff, sizeof(buff), fp) != NULL){
        printf("Read buff: %s", buff);
        uint32_t  pid = 0;
        sscanf(buff, "%*s\t%d  %*d\t%*d %*d %*x %*x %*c %s", &pid, targetProcess);
        printf("Read pid:%d, process:%s", pid, targetProcess);
        if(strcmp(targetProcess, process) == 0){
            targetPid = pid;
            break;
        }
    }
    fclose(fp);
    fp = NULL;
    if(targetPid != 0){
        return targetPid;
    }

    //通过cmdline查找
    DIR *dirProc;
    if((dirProc = opendir("/proc")) == NULL){
        printf("Error: exec opendir failed {%d, %s}", errno, strerror(errno));
        return -1;
    }
    struct dirent *dirent;
    while((dirent = readdir(dirProc)) != NULL){
        snprintf(buff, sizeof(buff), "proc/%s/cmdline", dirent->d_name);
        fp = fopen(buff, "r");
        if(fp == NULL){
            continue;
        }
        fscanf(fp, "%s", targetProcess);
        fclose(fp);
        fp = NULL;

        if(strcmp(targetProcess, process) == 0){
            targetPid = (uint32_t) atoi(dirent->d_name);
            break;
        }
    }
    closedir(dirProc);
    return targetPid;
}

uint32_t Hdog::getClonePid(uint32_t targetPid){
    printf("Start >>> getClonePid");

    char buff[256];
    sprintf(buff, "proc/%d/task/", targetPid);
    DIR *dirRoot = opendir(buff);
    if(dirRoot == NULL){
        printf("Error: exec opendir failed {%d, %s}", errno, strerror(errno));
        return -1;
    }

    struct dirent* dirent;
    struct dirent* lastDirent;
    while((dirent = readdir(dirRoot)) != NULL){
        lastDirent = dirent;
    }
    if(lastDirent == NULL){
        printf("Error: last dirent is null");
        return -1;
    }
    closedir(dirRoot);
    return (uint32_t) atoi(lastDirent->d_name);
}

int Hdog::attachPid(uint32_t pid){
    char buff[256];
    snprintf(buff, sizeof(buff), "proc/%d/mem", pid);
    int ret = ptrace(PTRACE_ATTACH, pid, NULL, NULL);
    if(ret != 0){
        printf("Ptrace %d failed {%d, %s}", pid, errno, strerror(errno));
        return 0;
    }else{
        int fpMem = open(buff, O_RDONLY);
        if(fpMem == 0){
            printf("Open %s failed: %d, %s", buff, errno, strerror(errno));
        }
        return fpMem;
    }
}


int main(int argc, char *argv[]){
    printf("start >>> hunting");

    uint32_t targetPid = 0;
    uint32_t clonePid = 0;
    Hdog hdog;
    targetPid = hdog.getProcessPid("cc.gnaixx.sample");
    clonePid = hdog.getClonePid(targetPid);
    printf("Target process pid:%d, clone pid:%d", targetPid, clonePid);

    if(targetPid == -1 || targetPid < 1 || clonePid <= 0){
        printf("Can`t find target process");
    }
    hdog.attachPid(clonePid);
}