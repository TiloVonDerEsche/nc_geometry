#!/bin/bash
x86_64-w64-mingw32-gcc ./src/flex/main.c -o ./generate_track_list.exe -ltre -lintl -lm

# Check for errors
if [ $? -eq 0 ]; then
  echo "Compilation successful for executable: generate_track_list.exe"
else
  echo "Compilation failed for executable: generate_track_list.exe"
fi


./src/track_visualization/opengl/mult_color/crosscompile_to_win.sh
