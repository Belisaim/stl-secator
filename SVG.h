// SVG.h

#include "Region.h"
#include "Options.h"

// Edit this if you want:
const float LEGEND_SHIFT_X = 1; // mm
const float LEGEND_SHIFT_Y = 1; // mm
const float FontSize_mm = 4;

class SVG
{
  FILE *f;
  float scale;
  Region legend_place;
public:
  SVG (const char *file_name, float maxx, float maxy);
  void curve_begin (const char *title);
  void add_line_segment (LineSegment ls);
  void add_legend (const char *legend);
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
    maxx * Opt.PrintDPmm + 2,
    maxy * Opt.PrintDPmm + 2
  );
  legend_place.init (0, 0, maxx, maxy);
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
  legend_place.reduce (ls);
  ls = ls * Opt.PrintDPmm;
  fprintf (f, "\t\tM %.0f\t%.0f", ls.P1.x, ls.P1.y);
  fprintf (f, "\tL %.0f\t%.0f\n", ls.P2.x, ls.P2.y);
}

/////////////////////////////////////////
void SVG::add_legend (const char *legend)
{
  const int font_size = FontSize_mm / 25.4 * Opt.printer_dpi;
  int n = strlen (legend), len_in_dots = n * font_size;
  float len_in_mm = n * FontSize_mm;
/*
  if (len_in_mm > legend_place.maxx - legend_place.minx)
    fprintf (stderr, "Warning: legend region for \"%s\" is too short: %.0f mm\n", legend, legend_place.maxx - legend_place.minx);
  if (FontSize_mm > legend_place.maxy - legend_place.miny)
    fprintf (stderr, "Warning: legend region for \"%s\" is too narrow: %.0f mm\n", legend, legend_place.maxy - legend_place.miny);
*/
  fprintf (f, "<text x = \"%i\" y=\"%i\" font-size=\"%i\" font-family=\"Courier\">\n",
    (int)(Opt.PrintDPmm * (legend_place.minx + LEGEND_SHIFT_X)),
    (int)(Opt.PrintDPmm * (legend_place.miny + LEGEND_SHIFT_Y + FontSize_mm)),
    font_size
  );
  fprintf (f, "%s\n", legend);
  fprintf (f, "</text>\n\n");

}

/////////////////////////////
void SVG::curve_end()
{
  fputs ("\t\" />\n\n", f);
}

