set datafile separator ","
set title "3D Points A and B from track_list.csv"
set xlabel "X"
set ylabel "Y"
set zlabel "Z"
set view 60, 30
set style data points
splot "track_list.csv" using 2:3:4 with points pointtype 7 pointsize 1 lc rgb "red" title "Point A", \
      "track_list.csv" using 5:6:7 with points pointtype 7 pointsize 1 lc rgb "blue" title "Point B"
pause -1
