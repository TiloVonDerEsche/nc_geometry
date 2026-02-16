gcc ./track_visualization/opengl/mult_color/t_vis_color.c -o ./3d_track_visualizer ^
-I"C:\Program Files (x86)\freeglut\include" -L"C:\Program Files (x86)\freeglut\lib" ^
-I"C:\Program Files (x86)\glew\include" -L"C:\Program Files (x86)\glew\lib" ^
-lfreeglut -lopengl32 -lgdi32 -lglew32 -lglu32
