// Poster.h

#include "CrossSection.h"

#define MAX_PAGES_IN_ROW 8
#define MAX_PAGES_IN_COL 8

class Poster
{
  Orientation orientation;
  int h_n[2], v_n[2];
  Page piece[MAX_PAGES_IN_ROW][MAX_PAGES_IN_COL];
  CrossSection *pCS;

  void analyse_h();
  void analyse_v();
public:
  Poster (CrossSection *p_cs) { pCS = p_cs; h_n[0] = h_n[1] = v_n[0] = v_n[1] = 0; }
  void analyse();
  void split_curve ();
  void write_SVG (const char *prefix);
  void write_SVG_preview (const char *prefix);
};

/////////////////////////////////////////
void Poster::analyse_h()
{
  float h;
  int orient, pg;
  for (orient = 0; orient < 2; orient++) {
    h = Opt.format[orient];
    for (pg = 1; pg <= MAX_PAGES_IN_ROW; pg++)
      if ((h - 2 * Opt.margin) * pg >= pCS->hori_size) { h_n[orient] = pg; break; }
  }
}

/////////////////////////////////////////
void Poster::analyse_v()
{
  float v;
  int orient, pg;
  for (orient = portrait; orient < 2; orient++) {
    v = Opt.format[1 - orient];
    for (pg = 1; pg <= MAX_PAGES_IN_COL; pg++)
      if ((v - 2 * Opt.margin) * pg >= pCS->vert_size) { v_n[orient] = pg; break; }
  }
}

/////////////////////////////////////////
void Poster::analyse()
{
  analyse_h();
  analyse_v();
  orientation = (h_n[0] * v_n[0] <= h_n[1] * v_n[1]) ? portrait: landscape;

  fprintf (stderr, "\nOrientation: %s\n",
    orientation == portrait ? "portrait": "landscape");
  fprintf (stderr, "By horisontal %i pages\n", h_n[orientation]);
  fprintf (stderr, "By   vertical %i pages\n", v_n[orientation]);
  
  assert (h_n[orientation] <= MAX_PAGES_IN_ROW);
  assert (v_n[orientation] <= MAX_PAGES_IN_COL);
  
  for (int h_pg = 0; h_pg < h_n[orientation]; h_pg++)
    for (int v_pg = 0; v_pg < v_n[orientation]; v_pg++)
      piece[h_pg][v_pg].orientation = orientation;
}

////////////////////////////////
void Poster::split_curve ()
{
  vector <LineSegment> *src;
  float from_x = 0, to_x = piece[0][0].field_width ();
  float from_y, to_y;

  for (int h_pg = 0; h_pg < h_n[orientation]; h_pg++) {
    from_y = 0, to_y = piece[0][0].field_height();
    for (int v_pg = 0; v_pg < v_n[orientation]; v_pg++) {
      src = pCS->get_curve_part (from_x, to_x, from_y, to_y);
      piece[h_pg][v_pg].copy_data (src, from_x, from_y);
      from_y = to_y;
      to_y += piece[0][0].field_height();
    }
    from_x = to_x;
    to_x += piece[0][0].field_width();
  }
}

/////////////////////////////////////////////
void Poster::write_SVG (const char *prefix)
{
  for (int h_pg = 0; h_pg < h_n[orientation]; h_pg++)
    for (int v_pg = 0; v_pg < v_n[orientation]; v_pg++)
      piece[h_pg][v_pg].write_SVG (prefix, h_pg, v_pg);
}

////////////////////////////////////////////////////////
void Poster::write_SVG_preview (const char *prefix)
{
  if (Opt.no_preview) return;

  int i, k = pCS->curve.size();
  float HS = h_n[orientation] * Opt.format[    orientation] - 2 * Opt.margin * (h_n[orientation] - 1);
  float VS = v_n[orientation] * Opt.format[1 - orientation] - 2 * Opt.margin * (v_n[orientation] - 1);
  fprintf (stderr, "HS = %f, VS = %f\n", HS, VS);

  float scale = max (Opt.format[0] / min (VS, HS), Opt.format[1] / max (VS, HS));
  fprintf (stderr, "Scale = %f\n", scale);

  string file_name = string ("preview_") + string (prefix) + string (".svg");
  SVG svg_preview (file_name.c_str(), HS * scale, VS * scale);

  svg_preview.curve_begin (" Cross section curve ");
  for (i = 0; i < k; i++)
    svg_preview.add_line_segment ((pCS->curve[i] + Point (Opt.margin, Opt.margin)) * scale);
  svg_preview.curve_end();
  
  svg_preview.curve_begin (" Poster\'s edges ");
  svg_preview.add_line_segment (LineSegment (Point (0, 0), Point (0, VS)) * scale);
  svg_preview.add_line_segment (LineSegment (Point (0, 0), Point (HS, 0)) * scale);
  svg_preview.add_line_segment (LineSegment (Point (HS, 0), Point (HS, VS)) * scale);
  svg_preview.add_line_segment (LineSegment (Point (0, VS), Point (HS, VS)) * scale);
  svg_preview.curve_end();

  svg_preview.curve_begin (" Page\'s frames ");
  float hs = piece[0][0].field_width(), vs = piece[0][0].field_height();
  fprintf (stderr, "hs = %f, vs = %f\n", hs, vs);
  for (i = 0; i <= h_n[orientation]; i++)
    svg_preview.add_line_segment (LineSegment (Point (Opt.margin + hs * i, Opt.margin), Point (Opt.margin + hs * i, VS - Opt.margin)) * scale);
  for (i = 0; i <= v_n[orientation]; i++)
    svg_preview.add_line_segment (LineSegment (Point (Opt.margin, Opt.margin + vs * i), Point (HS - Opt.margin, Opt.margin + vs * i)) * scale);
  svg_preview.curve_end();
}

