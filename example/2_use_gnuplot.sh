#!/bin/bash

../stl-secator --z=5 snail.ascii.stl > cross_section.dat
./cross_section.gnuplot
