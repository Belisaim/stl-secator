// CrossSection.h

#include <stdlib.h>
#include "Page.h"

class CrossSection
{
public:
  float minx, miny, maxx, maxy;
  vector <LineSegment> curve;
  float hori_size, vert_size;
  int hori_size_px, vert_size_px;

  CrossSection() { minx = 1e+30; miny = 1e+30; maxx = -1e+30; maxy = -1e+30; }
  void add (LineSegment ls) { curve.push_back (ls); }
  void analyse() { find_minmax(); move_at_corner(); reflect(); }
  vector <LineSegment> * get_curve_part (float from_x, float to_x, float from_y, float to_y);
private:
  void find_minmax();
  void move_at_corner();
  void reflect();
};

//////////////////////////////////////////////
void CrossSection::find_minmax()
{
  Point A, B;
  int i, k = curve.size();
  fprintf (stderr, "\nTotal line segments: %i\n\n", k);
  for (i = 0; i < k; i++) {
    A = curve[i].P1; B = curve[i].P2;
    if (A.x < minx) minx = A.x;
    if (A.y < miny) miny = A.y;
    if (A.x > maxx) maxx = A.x;
    if (A.y > maxy) maxy = A.y;
    if (B.x < minx) minx = B.x;
    if (B.y < miny) miny = B.y;
    if (B.x > maxx) maxx = B.x;
    if (B.y > maxy) maxy = B.y;
    fprintf (stderr, "A: (%f, %f),\tB: (%f, %f)\n", A.x, A.y, B.x, B.y);
  }
  fprintf (stderr, "\nx: [%f, %f],\ty: [%f, %f]\n\n", minx, maxx, miny, maxy);
  hori_size = maxx - minx;
  vert_size = maxy - miny;
  fprintf (stderr, "Horizontal size: %.0fpx (%.0fmm).\nVertical size: %.0fpx (%.0fmm).\n\n",
    hori_size * Opt.PrintDPmm, hori_size, vert_size * Opt.PrintDPmm, vert_size
  );

}

/////// To move so that 0 <= x <= maxx, 0 <= y <= maxy
void CrossSection::move_at_corner()
{
  int i, k = curve.size();

  for (i = 0; i < k; i++) {
    curve[i] = curve[i] - Point (minx, miny);
    fprintf (stderr, "A: (%f, %f),\tB: (%f, %f)\n",
      curve[i].P1.x, curve[i].P1.y, curve[i].P2.x, curve[i].P2.y);
  }
  maxx -= minx;
  maxy -= miny;
  minx = miny = 0;
  fprintf (stderr, "\nx: [%f, %f],\ty: [%f, %f]\n", minx, maxx, miny, maxy);
}

//// Reflect by vertical /////////////////
void CrossSection::reflect()
{
  Point A, B;
  int i, k = curve.size();
  for (i = 0; i < k; i++) {
    A = curve[i].P1; B = curve[i].P2;
    A.y = maxy - A.y;
    B.y = maxy - B.y;
    curve[i] = LineSegment (A, B);
  }
}

//////////////////////////////////////
vector <LineSegment> * CrossSection::get_curve_part (float from_x, float to_x, float from_y, float to_y)
{
  static vector <LineSegment> curve_part;
  Point A, B;
  LineSegment ls;
  int i, k = curve.size();
  
  curve_part.clear();
  for (i = 0; i < k; i++) {
    ls = curve[i]; A = ls.P1; B = ls.P2;
    if (A.belongs (from_x, to_x, from_y, to_y) &&
        B.belongs (from_x, to_x, from_y, to_y))
      curve_part.push_back (ls);
    else if (A.belongs (from_x, to_x, from_y, to_y) &&
           ! B.belongs (from_x, to_x, from_y, to_y)) {
      ls.subregion (from_x, to_x, from_y, to_y);
      curve_part.push_back (ls);
    }
    else if (B.belongs (from_x, to_x, from_y, to_y) &&
           ! A.belongs (from_x, to_x, from_y, to_y)) {
      ls.P1 = B; ls.P2 = A;
      ls.subregion (from_x, to_x, from_y, to_y);
      curve_part.push_back (ls);
    }
  }
  return &curve_part;
}

