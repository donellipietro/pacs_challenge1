#!/bin/bash

gnuplot<<EOF

set term jpeg
set output "solutions/solution.jpeg"
plot "solutions/solution.dat" u 1:2 w lp lw 2 title "uh", "solutions/solution.dat" u 1:3 w l lw 2 title "uex"
set output "solutions/convergence.jpeg"
set logscale xy
set key left top
plot "solutions/convergence.dat" u 1:2 w lp lw 2 title "error", \
     "solutions/convergence.dat" u 1:3 w l lw 2 title "h", \
     "solutions/convergence.dat" u 1:4 w l lw 2 title "h^2"

EOF