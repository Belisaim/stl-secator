# stl-secator
Utility stl-secator is intended to make cross section of 3D object stored in ASCII STL file by the plane z = 0.
It is assumed to be used together with admesh utility.
Admesh cans convert binary STL to ASCII STL, move and turn 3D object in order to make cross section in any place of object.
Bash script geves possibility to automate this process.
The result of stl-secator is SVG files.
They are parts of one big poster (in general case) to be printed on small printer and pasted together.
Moreover, stl-secator outputs in stdout a data of cross section to display its by gnuplot for example.

Tested on Debian GNU/Linux 11 (bullseye)

Usage:
  stl-secator [options] <ascii_stl_file>

Options:
  --margin=n    Width of empty fields in edges on page for pasting together of a few pages.
                n is float, millimeters.
                By default 10.
  --format=x:y  Format of the paper.
                x, y are float, millimeters.
                By default format A4: x = 210, y = 297.
  --no-legend   Do not print markers in SVG pictures.
                By default false.
  --no-preview  Do not create preview SVG file.

Example:
  ./stl-secator --margin=15 --format=297:420 boat.stl &>/dev/null

