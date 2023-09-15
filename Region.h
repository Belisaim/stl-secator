#include "geometry.h"

//////////////////////
class Region
{
public:
  float minx, miny, maxx, maxy;
  
  Region() { minx = 1e+30; miny = 1e+30; maxx = -1e+30; maxy = -1e+30; }
  void init (float _minx, float _miny, float _maxx, float _maxy);
  bool in (Point p);
  void reduce (Point p);        // The point must be out of the region. Make it.
  void reduce (LineSegment ls); // Also
};

//----------------------------------------------------------------------
void Region::init (float _minx, float _miny, float _maxx, float _maxy)
{
  minx = _minx; miny = _miny; maxx = _maxx; maxy = _maxy;
}

//-----------------------------------------------------
bool Region::in (Point p)
{
  if (p.x >= minx && p.x <= maxx &&
      p.y >= miny && p.y <= maxy) return true; else return false;
}

//-------------------------------------------------
void Region::reduce (Point p)
{
  if (in (p)) {
    if (maxx - p.x > p.x - minx) minx = p.x; else maxx = p.x;
    if (maxy - p.y > p.y - miny) miny = p.y; else maxy = p.y;
  }
}

//-------------------------------------------------
void Region::reduce (LineSegment ls)
{
  reduce (ls.P1);
  reduce (ls.P2);
}

