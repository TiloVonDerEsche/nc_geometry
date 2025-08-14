#!/bin/bash

# Path to your MXE installation
MXE_PATH=/home/tilo/res/mxe
TARGET=x86_64-w64-mingw32.static

# Safeguard: Explicitly set PKG_CONFIG_PATH
export PKG_CONFIG_PATH=${MXE_PATH}/usr/${TARGET}/lib/pkgconfig

# Cross-compile with static linking
${MXE_PATH}/usr/bin/${TARGET}-gcc t_vis_color.c -o 3d_track_visualizer.exe \
  `$MXE_PATH/usr/bin/${TARGET}-pkg-config --cflags --libs --static glut glew` \
  -lopengl32 -lglu32 -lm \
  -static \
  -Wl,--subsystem,windows

# Check for errors
if [ $? -eq 0 ]; then
  echo "Compilation successful! Executable: 3d_track_visualizer.exe"
else
  echo "Compilation failed."
fi
