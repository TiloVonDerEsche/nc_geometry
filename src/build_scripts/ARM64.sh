#!/bin/bash
echo "Compiling generate_track_list_arm_64.bin..."
make -C ./src/flex TARGET=arm64
make -C ./src/flex clean

# Check for errors
if [ $? -eq 0 ]; then
  echo "Compilation successful for executable: generate_track_list_arm_64.bin"
else
  echo "Compilation failed for executable: generate_track_list_arm_64.bin"
fi
echo ""

make -C ./src/track_visualization/opengl TARGET=arm64
make -C ./src/track_visualization/opengl clean
