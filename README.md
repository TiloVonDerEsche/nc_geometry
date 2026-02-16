# Requirements to build **`generate_track_list.exe`**:
   - **`bison`** (https://gnuwin32.sourceforge.net/packages/bison.htm) or (https://www.gnu.org/software/bison/)
   - **`flex`** (https://gnuwin32.sourceforge.net/packages/flex.htm)
   - **`gcc`** (https://sourceforge.net/projects/gcc-win64/) or (https://gcc.gnu.org/)
   - `make` (https://gnuwin32.sourceforge.net/packages/make.htm) (optional, see manual commands point 5) 


# Guide to using the project
1. **Setup a root folder** for the project.
2. In the root folder **create another folder** named `data`.
3. **Populate the `data` folder** with NC code (`.mpf`) files.
4. **Set the desired values** for the vars in **`config.txt`**:
   - **`debug_prints`** — Enables token ident and jump info debug prints, when set to 1.
   - **`hmhis_to_stdout`** — Enables var hashmap snapshot printing to stdout, for every .mpf line, when set to 1.
   - **`hmhis_to_file`** — Enables var hashmap snapshot printing to hmhis.json file, for every .mpf line, when set to 1
   - **`mpf_file`** — Sets the path of the `.mpf` file.
   - **`track_list_csv`** — Sets the path of the `track_list.csv` output file.
   - **`tracks_to_plot`** — Sets the path of the `track_list.csv`, which should be read by the `3d_track_visualizer.exe`.
5. Build **`generate_track_list.exe`** by navigating to **`./src/flex`** and executing **`make`**.
   Alternativly you can just **execute the commands**, which are specified in the Makefile:
   - **`flex -l lex.l`**
	- **`bison -dv grammar.y`**
	- **`gcc -o ./../../generate_track_list main.c`**
   
7. **Start the `generate_track_list.exe`** in a terminal.
   Afterwards, `track_list.csv` should appear at the path, specified through the `track_list_csv` var.

8. Make sure `3d_track_visualizer.exe` is in the **same directory** as `generate_track_list.exe`.  
   Both should have the `data` folder, with the CSVs and the `.mpf` file in their directory.

9. Start `3d_track_visualizer.exe` — this should open a window with a 3D visualization of the tracks.  
   You can traverse this 3D space using the **WASD** keys and holding down the **left mouse button**.
