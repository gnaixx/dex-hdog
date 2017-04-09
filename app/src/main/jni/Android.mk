LOCAL_PATH := $(call my-dir)

############# 动态链接库 #############
include $(CLEAR_VARS)

LOCAL_MODULE := hound-c
LOCAL_SRC_FILES := conn/Hound.cpp
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog

include $(BUILD_SHARED_LIBRARY)


############# 可执行库 #############
include $(CLEAR_VARS)

## 支持可执行文件打包到apk http://www.cnblogs.com/wi100sh/p/4308594.html ##
MY_BUILD_EXECUTABLE := $(LOCAL_PATH)/my-build-executable.mk
include $(call all-subdir-makefiles)

## 支持PIE ##
LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE

LOCAL_MODULE := hound-e
LOCAL_SRC_FILES := exec/Hdog.cpp
MY_LOCAL_MODULE_FILENAME := hound-e.so

include  $(MY_BUILD_EXECUTABLE)

