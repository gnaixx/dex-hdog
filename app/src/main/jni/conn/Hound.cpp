//
// Created by 薛祥清 on 2017/3/31.
//

#include "Hound.h"

static JNINativeMethod gMethods[] = {
        {"isRunning", "(Ljava/lang/String;)Z", (void *) isRunning},
};

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGI("Start >>> JNI_OnLoad");

    JNIEnv *env = NULL;
    jint result = -1;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return result;
    }
    const char *CLASS_NAME = "cc/gnaixx/hdog/util/JniUtil";
    jclass clazz = env->FindClass(CLASS_NAME);
    if (clazz == NULL) {
        LOGE("Find %s failed !!!", CLASS_NAME);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, NELEM(gMethods)) != JNI_OK) {
        LOGE("Register natives failed !!!");
    }
    return JNI_VERSION_1_6;
}

jboolean isRunning(JNIEnv *env, jclass clazz, jstring jpackageName) {
    LOGI("%d",getuid());
    if(setuid(0) == -1){
        printf("Get root failed: %d, %s\n", errno, strerror(errno));
    }
    LOGI("%d", getuid());

    const char *packageName = env->GetStringUTFChars(jpackageName, NULL);
    int isRunning  = JNI_FALSE;
    char cmd[256];
    char process[256];
    char buff[1024];

    sprintf(cmd, "ps | grep %s", packageName);
    FILE *fp = popen(cmd, "r");
    if (fp == NULL) {
        printf("Exec popen failed {%d, %s}\n", errno, strerror(errno));
    }else {
        while (fgets(buff, sizeof(buff), fp) != NULL) {
            uint32_t pid = 0;
            sscanf(buff, "%*s\t%d  %*d\t%*d %*d %*x %*x %*c %s", &pid, process);
            //printf("Read pid:%d, process:%s\n", pid, targetProcess);
            if (strcmp(process, packageName) == 0) {
                isRunning = JNI_TRUE;
                break;
            }
        }
        fclose(fp);
        fp = NULL;
    }
    return isRunning;
}