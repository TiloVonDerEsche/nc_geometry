# Guide to using the project
1. **Create a folder** named `data`.
2. Move the NC code (`.mpf`) file into the `data` folder.

3. **Set the desired values in `config.txt`:**

   - **`debug_prints`** — Enables token ident and jump info debug prints, when set to 1.
   - **`hmhis_to_stdout`** — Enables var hashmap snapshot printing to stdout, for every .mpf line, when set to 1.
   - **`hmhis_to_file`** — Enables var hashmap snapshot printing to hmhis.json file, for every .mpf line, when set to 1
   - **`mpf_file`** — Sets the path of the `.mpf` file.
   - **`track_list_csv`** — Sets the path of the `track_list.csv` output file.
   - **`tracks_to_plot`** — Sets the path of the `track_list.csv`, which should be read by the `3d_track_visualizer.exe`.
4. Start the `generate_track_list.exe` file in a terminal.  
   Afterwards, `track_list.csv` should appear at the path, specified through the `track_list_csv` var.

5. Make sure `3d_track_visualizer.exe` is in the **same directory** as `generate_track_list.exe`.  
   Both should have the `data` folder, with the CSVs and the `.mpf` file in their directory.

6. Start `3d_track_visualizer.exe` — this should open a window with a 3D visualization of the tracks.  
   You can traverse this 3D space using the **WASD** keys and holding down the **left mouse button**.
