# Info
This Repository contains GNU: `Bison`, `Flex` and `C` Code, which defines a NC Interpreter: `generate_track_list.exe`, that reads a NC code file and outputs a `track_list.csv` file. <br>
# The **`track_list.csv` contains**:
- `track_index`
-  start `point A`, end `point B` of a welding track
- laser_power (PUIS_LASER)
- machine_speed (VIT_TIR)
- overlap_vec (overlap / intersect direction of previously drawn track -> Work in Progress)
<br>
A track is traditionally detected through "LASER_ON" and "LASER_OFF" commands. The 3D points between those LASER commands form a track. <br>
<br>
The NC Interpreter is designed around the dialect of the AddUp M400 machine. <br>
The following Sinumerik manual seems to describe that dialect: <br>
https://cache.industry.siemens.com/dl/files/354/109779354/att_1019648/v1/MC_ncprogramming_progr_man_0220_de-DE.pdf <br>

# Guide to using the project
1. Put `config.txt` into the same folder as `generate_track_list.exe` and `3d_track_visualizer.exe`.
2. **Set the desired values** for the vars in **`config.txt`**:
   - **`debug_prints`** — Enables token identification prints and jump-info prints, when set to 1.
   - **`print_strs`** — Puts strings of the NC Code, which are denoted by these quotation marks: "A valid String!", when set to 1, on stdout.
   - **`hmhis_to_stdout`** — Enables hashmap (memory of variables) snapshot printing to stdout, for every NC code line, when set to 1.
   - **`hmhis_to_file`** — Enables hashmap (memory of variables) snapshot printing to hmhis.json file, for every NC code line, when set to 1
   - **`tracks_def_by_laser`** — Enables experimental track defined by track_mid_len and coordinate-modification-lines (coord-lines), when set to 0.
   - **`track_mid_len`** — Defines how many coord-lines to ignore, between track start- A, and end-point B, when using `tracks_def_by_laser=0`.
   - **`nc_file`** — Sets the path of the NC code file. 'mpf_file' is the deprecated name for 'nc_file'.
   - **`track_list_csv`** — Sets the path of the `track_list.csv` output file.
   - **`hmhis_json`** — When hmhis_to_file=1, then it sets the path of the strfloat Hashmap Snapshot History (Trace) writing target.
   - **`tracks_to_plot`** — Sets the path of the `track_list.csv`, which then will be read by the `3d_track_visualizer.exe`.
   - **`horizontal_radius`** — The floating point value of the horizontal radius spanning the elliptical cylinder, track abstraction.
   - **`vertical_radius`** — The floating point value of the horizontal radius spanning the elliptical cylinder, track abstraction.
   - **`track_accel_margin`** — Section-length from the end and start of a track, where the CNC machine (de-/)accelerates. WIP!
   
3. **Start the `generate_track_list.exe`** in a terminal.
   Afterwards, `track_list.csv` should appear at the path, specified through the `track_list_csv` var.
   
4. **Start `3d_track_visualizer.exe`** — This should open a window, with a 3D model of the tracks, which are drawn as cylinders, with an elliptical base.  
   **Exploring the 3D space:** **WASD** keys for XZ-Plane traversal, **Space** and **C** for Y-Axis as-/descend, hold down **left mouse button**, to pan around.
   
# Version-Features of **`generate_track_list.exe`**:
- **`0.1`** Saving X,Y,Z and (R) variables to Hashmap. Boolean expressions & arithmetic expressions, can used in assignments of variables. 
- **`0.2`** IF COND THEN BODY ENDIF Block. GOTOB jump command.
- **`0.3`** A,B,C work-plain rotation & GOTOF, GOTO and REPEAT.
- **`0.4`** Binary benchmarking tool.
- **`0.5`** GLR Parsing -> Expand Look-Ahead-buffer (currently size 1)
- **`0.6`** Complex user-variable types and functions.

# Version-Features of **`3d_track_visualizer.exe`**:
- **`0.1`** Seperate GitHub Repo.
- **`0.3.7`** FPS-like camera control, enriched with Y-Axis as-/descend

# Building **`generate_track_list.exe`** yourself:
Requirements:
- **`bison`** (https://gnuwin32.sourceforge.net/packages/bison.htm) or (https://www.gnu.org/software/bison/)
- **`flex`** (https://gnuwin32.sourceforge.net/packages/flex.htm)
- **`gcc`** (https://sourceforge.net/projects/gcc-win64/) or (https://gcc.gnu.org/)
- **`make`** (https://gnuwin32.sourceforge.net/packages/make.htm) (optional, see Makefile commands point 5)

Navigate to **`./src/flex`** and executing **`make`** in a terminal.
Alternativly you can just **execute the commands**, which are specified in the Makefile:
- **`flex -l lex.l`**
- **`bison -dv grammar.y`**
- **`gcc -o ./../../generate_track_list main.c`**
# Download
Under the Releases tab you can download the latest binaries / executables, and the corresponding source code, in a ZIP File, if interested.
[Download from Releases](releases), or click here: <br>
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/TiloVonDerEsche/nc_geometry)](https://github.com/TiloVonDerEsche/nc_geometry/releases/latest)

