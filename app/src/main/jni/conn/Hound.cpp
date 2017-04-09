//
// Created by 薛祥清 on 2017/3/31.
//

#include "Hound.h"

static JNINativeMethod gMethods[] = {
        {"hunting", "(Ljava/lang/String;)Ljava/lang/String;", (void *) hunting},
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

jstring hunting(JNIEnv *env, jclass clazz, jstring) {
    LOGI("%d",getuid());
    if(setuid(0) == -1){
        printf("Get root failed: %d, %s\n", errno, strerror(errno));
    }
    LOGI("%d", getuid());
    return env->NewStringUTF("hunting");
}