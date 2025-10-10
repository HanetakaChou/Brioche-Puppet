#
# Copyright (C) YuqiaoZhang(HanetakaChou)
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

# https://developer.android.com/ndk/guides/android_mk

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := BRX-Puppet

LOCAL_SRC_FILES := \
	$(LOCAL_PATH)/../source/main.cpp \
	$(LOCAL_PATH)/../source/user_input_controller.cpp \
	$(LOCAL_PATH)/../source/user_input_model.cpp \
	$(LOCAL_PATH)/../thirdparty/DXUT/Optional/DXUTcamera.cpp

LOCAL_CFLAGS :=

ifeq (armeabi-v7a,$(TARGET_ARCH_ABI))
LOCAL_ARM_MODE := arm
LOCAL_ARM_NEON := true
else ifeq (arm64-v8a,$(TARGET_ARCH_ABI))
LOCAL_CFLAGS +=
else ifeq (x86,$(TARGET_ARCH_ABI))
LOCAL_CFLAGS += -mf16c
LOCAL_CFLAGS += -mfma
LOCAL_CFLAGS += -mavx2
else ifeq (x86_64,$(TARGET_ARCH_ABI))
LOCAL_CFLAGS += -mf16c
LOCAL_CFLAGS += -mfma
LOCAL_CFLAGS += -mavx2
else
LOCAL_CFLAGS +=
endif

LOCAL_CFLAGS += -Wall
LOCAL_CFLAGS += -Werror=return-type

LOCAL_CFLAGS += -DPAL_STDCPP_COMPAT=1
LOCAL_CFLAGS += -DIMGUI_API=
LOCAL_CFLAGS += -DIMGUI_IMPL_API=

LOCAL_CFLAGS += -finput-charset=UTF-8 -fexec-charset=UTF-8

LOCAL_CPPFLAGS := 
LOCAL_CPPFLAGS += -std=c++17

LOCAL_C_INCLUDES :=
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../thirdparty/CoreRT/src/Native/inc/unix
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../thirdparty/DirectXMath/Inc

LOCAL_LDFLAGS :=
LOCAL_LDFLAGS += -Wl,--enable-new-dtags
LOCAL_LDFLAGS += -Wl,-rpath,\$$ORIGIN
LOCAL_LDFLAGS += -Wl,--version-script,$(LOCAL_PATH)/BRX-Puppet.map

LOCAL_LDFLAGS += -lm -lxcb

LOCAL_STATIC_LIBRARIES :=

LOCAL_SHARED_LIBRARIES :=
LOCAL_SHARED_LIBRARIES += BRX-Asset-Import
LOCAL_SHARED_LIBRARIES += BRX-ANARI
LOCAL_SHARED_LIBRARIES += BRX-ImGui
LOCAL_SHARED_LIBRARIES += BRX-PAL
LOCAL_SHARED_LIBRARIES += BRX-Motion
LOCAL_SHARED_LIBRARIES += BRX-WSI
LOCAL_SHARED_LIBRARIES += McRT-Malloc

include $(BUILD_EXECUTABLE)
