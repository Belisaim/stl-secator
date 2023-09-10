// SVG.h

#include "Options.h"

const float PrintDPmm = Opt.printer_dpi / 25.4;

class SVG
{
  FILE *f;
  float scale;
public:
  SVG (const char *file_name, float maxx, float maxy);
  void curve_begin (const char *title);
  void add_line_segment (LineSegment ls);
  void curve_end();
 ~SVG() { fputs ("</svg>\n", f); fclose (f); }
};

///////////////////////////////////////////////////////////////////////////
SVG::SVG (const char *file_name, float maxx, float maxy)
{
  f = fopen (file_name, "w");
  fputs ("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n", f);
  fputs ("<svg version=\"1.1\"\n", f);
  fputs ("\tbaseProfile=\"full\"\n", f);
  fputs ("\txmlns=\"http://www.w3.org/2000/svg\"\n", f);
  fputs ("\txmlns:xlink=\"http://www.w3.org/1999/xlink\"\n", f);
  fputs ("\txmlns:ev=\"http://www.w3.org/2001/xml-events\"\n", f);
  fprintf (f, "\twidth=\"%.0fpx\" height=\"%.0fpx\">\n\n",
    maxx * PrintDPmm + 2,
    maxy * PrintDPmm + 2
  );
}

//////////////////////////////////////////
void SVG::curve_begin (const char *title)
{
  fprintf (f, "\t<!--%s-->\n", title);
  fputs ("\t<path fill=\"none\" stroke=\"black\" d=\"\n", f);
}

///////////////////////////////////////////////
void SVG::add_line_segment (LineSegment ls)
{
  ls = ls * PrintDPmm;
  fprintf (f, "\t\tM %.0f\t%.0f", ls.P1.x, ls.P1.y);
  fprintf (f, "\tL %.0f\t%.0f\n", ls.P2.x, ls.P2.y);
}

/////////////////////////////
void SVG::curve_end()
{
  fputs ("\t\" />\n\n", f);
}

