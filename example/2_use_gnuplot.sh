#!/bin/bash

../stl-secator --no-preview ulitka.ascii.stl > cross_section.dat
./cross_section.gnuplot
