#!/bin/bash
echo "Compiling generate_track_list.bin..."
make -C ./src/interpreter
make -C ./src/interpreter clean

if [ $? -eq 0 ]; then
  echo "Compilation successful for executable: generate_track_list.bin"
else
  echo "Compilation failed for executable: generate_track_list.bin"
fi
echo ""

make -C ./src/3d_vis/opengl
# make -C ./src/3d_vis/opengl clean
