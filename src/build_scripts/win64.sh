#!/bin/bash
clear
echo "Compiling generate_track_list.exe..."
make -C ./src/flex windows

# Check for errors
if [ $? -eq 0 ]; then
  echo "Compilation successful for executable: generate_track_list.exe"
else
  echo "Compilation failed for executable: generate_track_list.exe"
fi
echo ""

./src/track_visualization/opengl/mult_color/crosscompile_to_win.sh
