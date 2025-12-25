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
    echo "  Debug"
    echo "  Release"
    echo ""
    exit 1
fi

if test \( \( -n "$1" \) -a \( "$1" = "Debug" \) \); then 
    XCODE_BUILD_CONFIGURATION="Debug"
elif test \( \( -n "$1" \) -a \( "$1" = "Release" \) \); then
    XCODE_BUILD_CONFIGURATION="Release"
else
    echo "The configuration \"$1\" is not supported!"
    echo ""
    echo "configuration:"
    echo "  Debug"
    echo "  Release"
    echo ""
    exit 1
fi

PROJECT_DIRECTORY="$(cd "$(dirname "$0")" 1>/dev/null 2>/dev/null && pwd)"  

XCODE_BUILD_ARGUMENT_BEFORE_SCHEME=(
  "-workspace"
  "${PROJECT_DIRECTORY}/BRX-Puppet.xcworkspace"
)

XCODE_BUILD_ARGUMENT_AFTER_SCHEME=(
  "-destination"
  "generic/platform=macOS"
  "-configuration"
  "${XCODE_BUILD_CONFIGURATION}"
  "-derivedDataPath"
  "${PROJECT_DIRECTORY}/Xcode/DerivedData"
  "build"
)

if xcodebuild "${XCODE_BUILD_ARGUMENT_BEFORE_SCHEME[@]}" -scheme BRX-ANARI-Build-GLSL "${XCODE_BUILD_ARGUMENT_AFTER_SCHEME[@]}"; then
    echo "build Brioche-Analytic-Rendering-Interface GLSL passed"
else
    echo "build Brioche-Analytic-Rendering-Interface GLSL failed"
    exit 1
fi

if xcodebuild "${XCODE_BUILD_ARGUMENT_BEFORE_SCHEME[@]}" -scheme BRX-ImGui-Build-Fonts "${XCODE_BUILD_ARGUMENT_AFTER_SCHEME[@]}"; then
    echo "xcode build Brioche-ImGui fonts passed"
else
    echo "bxcode uild Brioche-ImGui fonts failed"
    exit 1
fi

if xcodebuild "${XCODE_BUILD_ARGUMENT_BEFORE_SCHEME[@]}" -scheme BRX-ImGui-Build-GLSL "${XCODE_BUILD_ARGUMENT_AFTER_SCHEME[@]}"; then
    echo "xcode build Brioche-ImGui GLSL passed"
else
    echo "xcode build Brioche-ImGui GLSL failed"
    exit 1
fi

if xcodebuild "${XCODE_BUILD_ARGUMENT_BEFORE_SCHEME[@]}" -scheme BRX-Motion-Build-MediaPipe-Model "${XCODE_BUILD_ARGUMENT_AFTER_SCHEME[@]}"; then
    echo "xcode build mediapipe-model passed"
else
    echo "xcode build mediapipe-model failed"
    exit 1
fi

if xcodebuild "${XCODE_BUILD_ARGUMENT_BEFORE_SCHEME[@]}" -scheme BRX-Puppet "${XCODE_BUILD_ARGUMENT_AFTER_SCHEME[@]}"; then
    echo "xcode build Brioche-Puppet passed"
else
    echo "xcode build Brioche-Puppet failed"
    exit 1
fi
