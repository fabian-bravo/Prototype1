LOCAL_PATH := $(call my-dir)/..
include $(CLEAR_VARS)
LOCAL_MODULE := Prototype1

LOCAL_SRC_FILES := /src/Engine/drawHandler.cpp /src/Engine/engine.cpp /jni/main.cpp
LOCAL_LDLIBS := -landroid -llog -lEGL -lGLESv2
LOCAL_STATIC_LIBRARIES := android_native_app_glue
LOCAL_CFLAGS += -fpermissive
include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)