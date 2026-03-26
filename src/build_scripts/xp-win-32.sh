#!/bin/bash
echo "Compiling for Windows XP (32-bit)..."

# Call make with the XP target
make -C ./src/flex TARGET=XP

if [ $? -eq 0 ]; then
  echo "Success! generate_track_list.exe is ready for XP."
else
  echo "Compilation failed."
fi

make -C ./src/track_visualization/opengl TARGET=XP
make -C ./src/track_visualization/opengl clean
