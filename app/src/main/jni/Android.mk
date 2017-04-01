LOCAL_PATH := $(call my-dir)

############# 添加自定义编译脚本 #############
# http://www.cnblogs.com/wi100sh/p/4308594.html
MY_BUILD_EXECUTABLE := $(LOCAL_PATH)/my-build-executable.mk
include $(call all-subdir-makefiles)

############# 动态链接库 #############
include $(CLEAR_VARS)

LOCAL_MODULE := hound-s
LOCAL_SRC_FILES := conn/Hound.cpp
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -llog

include $(BUILD_SHARED_LIBRARY)


############# 可执行库 #############
include $(CLEAR_VARS)

LOCAL_MODULE := hound-e
LOCAL_SRC_FILES := scri/Hdog.cpp
MY_LOCAL_MODULE_FILENAME := hound-e.so

include  $(MY_BUILD_EXECUTABLE)

