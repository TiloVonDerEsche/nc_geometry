#!/bin/bash
clear
echo "Compiling generate_track_list.bin..."
make -C ./src/flex
make -C ./src/flex clean

if [ $? -eq 0 ]; then
  echo "Compilation successful for executable: generate_track_list.bin"
else
  echo "Compilation failed for executable: generate_track_list.bin"
fi
echo ""

./src/track_visualization/opengl/mult_color/linux_compile.sh

echo "Exiting..."
