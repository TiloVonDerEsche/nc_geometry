#To use the project:
Create a folder named "data".
Move the nc code (.mpf) file into the data folder

Set the desired values in config.txt:
    -mpf_lines sets the amount of lines of the .mpf file that should be read and converted
    -(precision unused at the moment)
    -max_line_len must be larger than the longest line in the .mpf file
    -mpf_file specifies the path of the .mpf file
    -data_tuples_csv specifies the path of the data_tuples.csv out file
    -track_list_csv specifies the path of the track_list.csv out file
    -horizontal_radius specifies the horizontal_radius of a track
    -vertical_radius specifies the vertical_radius of a track
     

Then start the generate_track_list.exe file.
Afterwards a data_tuples.csv and track_list.csv should appear in the data folder.

Make sure that 3d_track_visualizer.exe is in the same directory as generate_track_list.exe.
Both should have the data folder, with the csv's and the mpf in their directory.

Then starting 3d_track_visualizer.exe should result in a window with a 3D visalization of the tracks.
You can traverse that 3D space with WASD and the left mouse button.

