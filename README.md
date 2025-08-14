# Guide to using the project
1. **Create a folder** named `data`.
2. Move the NC code (`.mpf`) file into the `data` folder.

3. **Set the desired values in `config.txt`:**

   - **`mpf_lines`** — Number of lines of the `.mpf` file to read and convert.
   - **`precision`** — *(unused at the moment)*.
   - **`max_line_len`** — Must be larger than the longest line in the `.mpf` file.
   - **`mpf_file`** — Path of the `.mpf` file.
   - **`data_tuples_csv`** — Path of the `data_tuples.csv` output file.
   - **`track_list_csv`** — Path of the `track_list.csv` output file.
   - **`horizontal_radius`** — Horizontal radius of a track.
   - **`vertical_radius`** — Vertical radius of a track.

4. Start the `generate_track_list.exe` file.  
   Afterwards, `data_tuples.csv` and `track_list.csv` should appear in the `data` folder.

5. Make sure `3d_track_visualizer.exe` is in the **same directory** as `generate_track_list.exe`.  
   Both should have the `data` folder, with the CSVs and the `.mpf` file in their directory.

6. Start `3d_track_visualizer.exe` — this should open a window with a 3D visualization of the tracks.  
   You can move around in 3D space using **WASD** keys and the **left mouse button**.
