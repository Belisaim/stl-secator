// geometry.h

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "Options.h"

#define L 128

//**************************************************
class Vertex { public: float x, y, z; };

//**************************************************
class Tri { public: Vertex v[3]; };

//**************************************************
class Point {
public:
  float x, y;
  Point () {}
  Point (float x_, float y_) { x = x_; y = y_; }
  void operator= (Vertex& v) { x = v.x; y = v.y; }
  Point operator- (Point P) { return Point (x - P.x, y - P.y); }
  Point operator+ (Point P) { return Point (x + P.x, y + P.y); }
  Point operator* (float c) { return Point (x * c, y * c); }
  bool belongs (float from_x, float to_x, float from_y, float to_y)
  {
    if (x >= from_x && x <= to_x && y >= from_y && y <= to_y)
      return true;
    else return false;
  }
};

//**************************************************
class LineSegment {
public:
  Point P1, P2;
  LineSegment () {}
  LineSegment (Point A, Point B) { P1 = A; P2 = B; }
  LineSegment operator- (Point P) { return LineSegment (P1 - P, P2 - P); }
  LineSegment operator+ (Point P) { return LineSegment (P1 + P, P2 + P); }
  LineSegment operator* (float c) { return LineSegment (P1 * c, P2 * c); }
  void subregion (float from_x, float to_x, float from_y, float to_y)
  {
    if (P2.x < from_x) {
      P2.y = P1.y + (P2.y - P1.y) * (from_x - P1.x) / (P2.x - P1.x);
      P2.x = from_x;
    }
    if (P2.x > to_x) {
      P2.y = P1.y + (P2.y - P1.y) * (to_x - P1.x) / (P2.x - P1.x);
      P2.x = to_x;
    }
    if (P2.y < from_y) {
      P2.x = P1.x + (P2.x - P1.x) * (from_y - P1.y) / (P2.y - P1.y);
      P2.y = from_y;
    }
    if (P2.y > to_y) {
      P2.x = P1.x + (P2.x - P1.x) * (to_y - P1.y) / (P2.y - P1.y);
      P2.y = to_y;
    }
  }
};

//------------------------------------------
Point getPointAtZ0 (Vertex &A, Vertex &B) {
  Point res;
  float a = A.z / (A.z - B.z);
  res.x = A.x + a * (B.x - A.x);
  res.y = A.y + a * (B.y - A.y);
  return res;
}

//---------------------------------------------------
bool get2pointsAtZ0 (Tri &t, Point &A, Point &B) {
  static Point P[6];
  int i = 0;
  
  if (signbit (t.v[0].z) != signbit (t.v[1].z))
    P[i++] = getPointAtZ0 (t.v[0], t.v[1]);
  if (t.v[2].z == 0.0) P[i++] = t.v[2];

  if (signbit (t.v[1].z) != signbit (t.v[2].z))
    P[i++] = getPointAtZ0 (t.v[1], t.v[2]);
  if (t.v[0].z == 0.0) P[i++] = t.v[0];

  if (signbit (t.v[2].z) != signbit (t.v[0].z))
    P[i++] = getPointAtZ0 (t.v[2], t.v[0]);
  if (t.v[1].z == 0.0) P[i++] = t.v[1];

  if (i >= 2) { A = P[0]; B = P[1]; return true; }
  else return false;
}

//**************************
class Solid {
  char *p, buf[L];
  FILE *f;
  
  void readNextLine();
  float readFloat ();
public:
  Tri t;

  Solid (char *file);
 ~Solid() { fclose (f); }
  bool getNextTri();
};

////////////////////////////////
void Solid::readNextLine() {
  *buf = '\0'; fgets (buf, L, f);
  p = buf; while (*p) *p++ = tolower(*p);
}

////////////////////////////////
float Solid::readFloat () {
  float r = atof (p);
  return (r == -0.0? 0.0: r);
}

////////////////////////////////
Solid::Solid (char *file) {
  f = fopen (file, "r");
  readNextLine();
  p = strstr (buf, "solid");
  if (!p) { perror ("not ASCII file"); exit (-1); }
  fprintf (stderr, "Title of the STL: %s", p + 6);
}

////////////////////////////
bool Solid::getNextTri() {
  readNextLine();
  if ((p =  strstr (buf, "facet")) && (p = strstr (p + 6, "normal"))) {
    // TODO a something with normal...
    readNextLine();
    if ((p = strstr (buf, "outer")) && (p = strstr (p + 6, "loop"))) {
      for (int i = 0; i < 3; i++) {
        readNextLine();
        if (p = strstr (buf, "vertex")) {
          p = strtok (p += 7, " \t"); t.v[i].x = readFloat();
          p = strtok (NULL  , " \t"); t.v[i].y = readFloat();
          p = strtok (NULL  , " \t"); t.v[i].z = readFloat() - Opt.z;
        }
      }
      readNextLine();
      if (!strstr (buf, "endloop")) { perror ("no endloop"); exit (-2); }
    }
    else { perror ("no outer loop"); exit (-3); }

    readNextLine();
    if (!strstr (buf, "endfacet")) { perror ("no endfacet"); exit (-4); }
  }
  else {
    if (strstr (buf, "endsolid")) return false;
    else { perror ("no facet normal"); exit (5); }
  }
  return true;
}

