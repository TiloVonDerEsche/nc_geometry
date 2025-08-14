#!/bin/bash
gcc t_vis_color.c -o 3d_track_visualizer.bin \
-I/home/tilo/res/freeglut/include \
-L/home/tilo/res/freeglut/build/lib \
-I/home/tilo/res/glew/include \
-L/home/tilo/res/glew/lib \
-lglut -lGLEW -lGL -lGLU -lm
