// stl-secator.cpp

#include "Poster.h"

int main (int argc, char *argv[])
{
  if (argc < 2) { Opt.print_usage(); return -1; }
  Opt.init (argv, argc);

  size_t i, k = 0;
  string stl_file = Opt.stl_file, prefix;

  i = stl_file.find (".stl");
  if (i != string::npos)
    prefix = stl_file.substr (0, i);
  else
    prefix = stl_file;

  Solid solid (Opt.stl_file);
  CrossSection cross_section;
  Point A, B;

  while (solid.getNextTri()) {
    if (get2pointsAtZ0 (solid.t, A, B)) {
      cross_section.add ({A, B});
      printf ("%f %f\n",   A.x, A.y);
      printf ("%f %f\n\n", B.x, B.y);
    }
//  for (i = 0; i < 3; i++)
//    fprintf (stderr, "x = %f, y = %f, z = %f\n", s.t.v[i].x, s.t.v[i].y, s.t.v[i].z);
  }

  cross_section.analyse();
  
  Poster pl (&cross_section);
  pl.analyse();
  pl.split_curve();
  pl.write_SVG (prefix.c_str());
  pl.write_SVG_preview (prefix.c_str());
  
  return 0;
}

