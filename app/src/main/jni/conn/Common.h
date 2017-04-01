//
// Created by 薛祥清 on 2017/3/31.
//

#ifndef DEX_HOUND_COMMON_H
#define DEX_HOUND_COMMON_H

#include <jni.h>
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
#include <android/log.h>

//日志
#define TAG "GNAIXX-NDK"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

//计算 JNINativeMethod 数组大小
#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

#endif //DEX_HOUND_COMMON_H
