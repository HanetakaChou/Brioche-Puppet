#!/bin/bash

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

set -euo pipefail

# configure
if test \( $# -ne 1 \); then
    echo "usage: build.sh configuration"
    echo ""
    echo "configuration:"
    echo "  debug"
    echo "  release"
    echo ""
    exit 1
fi

if test \( \( -n "$1" \) -a \( "$1" = "debug" \) \); then 
    CONFIGURATION_NDK_DEBUG="NDK_DEBUG:=1"
    CONFIGURATION_APP_DEBUG="APP_DEBUG:=true"
    CONFIGURATION_DIRECTORY="debug"
elif test \( \( -n "$1" \) -a \( "$1" = "release" \) \); then
    CONFIGURATION_NDK_DEBUG="NDK_DEBUG:=0"
    CONFIGURATION_APP_DEBUG="APP_DEBUG:=false"
    CONFIGURATION_DIRECTORY="release"
else
    echo "The configuration \"$1\" is not supported!"
    echo ""
    echo "configuration:"
    echo "  debug"
    echo "  release"
    echo ""
    exit 1
fi

PROJECT_DIRECTORY="$(cd "$(dirname "$0")" 1>/dev/null 2>/dev/null && pwd)"  

OBJECT_DIRECTORY="${PROJECT_DIRECTORY}/obj/${CONFIGURATION_DIRECTORY}"
BINARY_DIRECTORY="${PROJECT_DIRECTORY}/bin/${CONFIGURATION_DIRECTORY}"

NDK_BUILD_PATH="${PROJECT_DIRECTORY}/../thirdparty/NDK-Build-Linux/ndk-build"

NDK_BUILD_ARGUMENT=(
  "-j$(nproc)"
  "NDK_MODULE_PATH:=${PROJECT_DIRECTORY}/.."
  "NDK_PROJECT_PATH:=null"
  "NDK_OUT:=${OBJECT_DIRECTORY}"
  "NDK_LIBS_OUT:=${BINARY_DIRECTORY}"
  "${CONFIGURATION_NDK_DEBUG}"
  "NDK_APPLICATION_MK:=Application.mk"
  "APP_BUILD_SCRIPT:=Linux.mk"
  "APP_ABI:=x86_64"
  "APP_PLATFORM:=android-28"
)

MAKE_ARGUMENT=(
  "-j$(nproc)"
  "APP_PROJECT_PATH:=${PROJECT_DIRECTORY}"
  "APP_ABI:=x86_64"
  "${CONFIGURATION_APP_DEBUG}"
)

cd ${PROJECT_DIRECTORY}

if make -C "${PROJECT_DIRECTORY}/../thirdparty/Brioche-Analytic-Rendering-Interface/build-GLSL" -f "GLSL.mk" "${MAKE_ARGUMENT[@]}"; then
    echo "build Brioche-Analytic-Rendering-Interface GLSL passed"
else
    echo "build Brioche-Analytic-Rendering-Interface GLSL failed"
    exit 1
fi

if make -C "${PROJECT_DIRECTORY}/../thirdparty/Brioche-ImGui/build-fonts" -f "fonts.mk" "${MAKE_ARGUMENT[@]}"; then
    echo "build Brioche-ImGui fonts passed"
else
    echo "build Brioche-ImGui fonts failed"
    exit 1
fi

if make -C "${PROJECT_DIRECTORY}/../thirdparty/Brioche-ImGui/build-GLSL" -f "GLSL.mk" "${MAKE_ARGUMENT[@]}"; then
    echo "build Brioche-ImGui GLSL passed"
else
    echo "build Brioche-ImGui GLSL failed"
    exit 1
fi

if make -C "${PROJECT_DIRECTORY}/../thirdparty/Brioche-Motion/build-mediapipe-model" -f "mediapipe-model.mk" "${MAKE_ARGUMENT[@]}"; then
    echo "build mediapipe-model passed"
else
    echo "build mediapipe-model failed"
    exit 1
fi

if "${NDK_BUILD_PATH}" -C "${PROJECT_DIRECTORY}" "APP_MODULES:=BRX-OpenFileDialog BRX-Puppet" "${NDK_BUILD_ARGUMENT[@]}" ; then
    echo "ndk-build Brioche-Puppet passed"
else
    echo "ndk-build Brioche-Puppet failed"
    exit 1
fi

if test \( \( -n "$1" \) -a \( "$1" = "debug" \) \); then 
    cp -f "${PROJECT_DIRECTORY}/../thirdparty/Vulkan-ValidationLayers/bin/linux/x86_64/VkLayer_khronos_validation.json" "${PROJECT_DIRECTORY}/obj/debug/local/x86_64"
    cp -f "${PROJECT_DIRECTORY}/../thirdparty/Vulkan-ValidationLayers/bin/linux/x86_64/libVkLayer_khronos_validation.so" "${PROJECT_DIRECTORY}/obj/debug/local/x86_64"
fi
