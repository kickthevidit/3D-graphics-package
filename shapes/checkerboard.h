#pragma once

#include "shape.h"

class Checkerboard : public Shape {
  Pixel color_2;
  Vec3 normal;
  double d;  // distance of origin to plane
  Vec3 side; // dir vec showing board orientation, going from one corner to another
  Vec3 centre;

public:
  Checkerboard(Vec3 n, double d_, Pixel p, Vec3 cen, Vec3 s, Material &m)
      : Shape(m), color_2(p), normal(n), d(d_), side(s), centre(cen) {}

  bool RayIntersect(const Vec3 &orig, const Vec3 &dir, double &t0) const;
  virtual Vec3 GetNormal(const Vec3 &point) const { return normal; }
};

bool Checkerboard::RayIntersect(const Vec3 &orig, const Vec3 &dir,
                                double &t0) const {
  // halfway done
  double Vd = normal * dir;
  if (Vd == 0)
    return false; // ray is parallel to plane}
  double v0 = (-(normal * orig + d));
  t0 = v0 / Vd;
  if (t0 < 0)
    return false; // intersects behind camera
  Vec3 cont_pt = orig + dir * t0;
  if ((((centre - cont_pt) * side) / pow(side.M(), 2)) > (side / 2).M())
    return false;
  return true;
}