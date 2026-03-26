#!/bin/bash
echo "Compiling generate_track_list.exe..."
make -C ./src/flex TARGET=WIN64
make -C ./src/flex clean

# Check for errors
if [ $? -eq 0 ]; then
  echo "Compilation successful for executable: generate_track_list.exe"
else
  echo "Compilation failed for executable: generate_track_list.exe"
fi
echo ""

make -C ./src/track_visualization/opengl TARGET=WIN64
make -C ./src/track_visualization/opengl clean
