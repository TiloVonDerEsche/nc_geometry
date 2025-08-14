To use the project:
Create a folder named "data".
Move the nc code (.mpf) file into the data folder

Set the desired values in config.txt:
    mpf_lines sets the amount of lines of the .mpf file that should be read and converted
    (precision unused at the moment)
    max_line_len must be larger than the longest line in the .mpf file
    mpf_file specifies the path of the .mpf file
    data_tuples_csv specifies the path of the data_tuples.csv out file
    track_list_csv specifies the path of the track_list.csv out file

    horizontal_radius specifies the horizontal_radius of a track
    vertical_radius specifies the vertical_radius of a track
     

Then start the generate_track_list.exe file.


Afterwards a data_tuples.csv and track_list.csv should appear in the data folder.

