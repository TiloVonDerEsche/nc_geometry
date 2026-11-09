#!/bin/bash
echo "Compiling generate_track_list.exe..."
make -C ./src/interpreter TARGET=WIN64
make -C ./src/interpreter clean

# Check for errors
if [ $? -eq 0 ]; then
  echo "Compilation successful for executable: generate_track_list.exe"
else
  echo "Compilation failed for executable: generate_track_list.exe"
fi
echo ""

make -C ./src/3d_vis/opengl TARGET=WIN64
make -C ./src/3d_vis/opengl clean
