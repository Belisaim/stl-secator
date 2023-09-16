#!/bin/gnuplot -persist

set border 0
plot 'cross_section.dat' using 1:2 title 'Cross section' with lines
