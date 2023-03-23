#!/bin/bash

gnuplot<<EOF

set term jpeg
set output "results/solution.jpeg"
set title "Solution"
set xlabel "t"
set grid
plot "results/solution.dat" u 1:2 w lp lw 2 title "uh", "results/solution.dat" u 1:3 w l lw 2 title "uex"

EOF