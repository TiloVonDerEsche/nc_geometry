#!/bin/bash
echo "Compiling generate_track_list.bin..."
make -C ./src/flex
make -C ./src/flex clean

if [ $? -eq 0 ]; then
  echo "Compilation successful for executable: generate_track_list.bin"
else
  echo "Compilation failed for executable: generate_track_list.bin"
fi
echo ""

make -C ./src/track_visualization/opengl
# make -C ./src/track_visualization/opengl clean
