#!/bin/bash
echo "Compiling generate_track_list_arm_64.bin..."
make -C ./src/interpreter TARGET=arm64
make -C ./src/interpreter clean

# Check for errors
if [ $? -eq 0 ]; then
  echo "Compilation successful for executable: generate_track_list_arm_64.bin"
else
  echo "Compilation failed for executable: generate_track_list_arm_64.bin"
fi
echo ""

make -C ./src/3d_vis/opengl TARGET=arm64
make -C ./src/3d_vis/opengl clean
