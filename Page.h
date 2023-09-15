// Page.h

#include "SVG.h"
#include <assert.h>
#include <string>
#include <vector>
using namespace std;

#define LEN 1024

enum Orientation { portrait = 0, landscape = 1 };

class Page
{
public:
  Orientation orientation;
  vector <LineSegment> curve_part;
  
  Page() { orientation = portrait; }
  float field_width () { return Opt.format[    orientation] - 2 * Opt.margin; }
  float field_height() { return Opt.format[1 - orientation] - 2 * Opt.margin; }
  void copy_data (vector <LineSegment> *src, float from_x, float from_y);
  void write_SVG (const char *prefix, int h_pg, int v_pg);
};

/////////////////////////////////////////////
void Page::copy_data (vector <LineSegment> *src, float from_x, float from_y)
{
  int i, k = src->size();
  for (i = 0; i < k; i++)
    curve_part.push_back ((*src)[i] - Point (from_x, from_y));
}

//////////////////////////////////////////////////////////////////////
void Page::write_SVG (const char *prefix, int h_pg, int v_pg)
{
  int i, k = curve_part.size();
  string marker = prefix;
  char marker_y = 'A' + v_pg, marker_x = '1' + h_pg;
  marker += '_'; marker += marker_y; marker += '-';  marker += marker_x;
  string file_name = marker + (k == 0? "_empty.svg": ".svg");
  
  SVG svg (file_name.c_str(), field_width(), field_height());

  svg.curve_begin (marker.c_str());
  for (i = 0; i < k; i++) svg.add_line_segment (curve_part[i]);
  svg.curve_end();

  svg.curve_begin (" Frame ");
  svg.add_line_segment (LineSegment (Point (0, 0), Point (0, field_height())));
  svg.add_line_segment (LineSegment (Point (0, 0), Point (field_width(), 0)));
  svg.add_line_segment (LineSegment (Point (field_width(), 0), Point (field_width(), field_height())));
  svg.add_line_segment (LineSegment (Point (0, field_height()), Point (field_width(), field_height())));
  svg.curve_end();
  
  if (!Opt.no_legend) svg.add_legend (marker.c_str());
}

