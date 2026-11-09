#!/bin/bash
echo "Compiling for Windows XP (32-bit)..."

# Call make with the XP target
make -C ./src/interpreter TARGET=XP
make -C ./src/interpreter clean

if [ $? -eq 0 ]; then
  echo "Success! generate_track_list.exe is ready for XP."
else
  echo "Compilation failed."
fi

make -C ./src/3d_vis/opengl TARGET=XP
make -C ./src/3d_vis/opengl clean
