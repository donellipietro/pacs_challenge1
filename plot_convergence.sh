#!/bin/bash

gnuplot<<EOF

set term jpeg
set output "results/convergence.jpeg"
set logscale xy
set key left top
set format xy "10^{%L}"
set ylabel "log(error)"
set xlabel "log(h)"
set grid
set title "Convergence order"
plot "results/convergence.dat" u 1:2 w lp lw 2 title "error", \
     "results/convergence.dat" u 1:3 w l lw 2 title "h", \
     "results/convergence.dat" u 1:4 w l lw 2 title "h^2"

EOF