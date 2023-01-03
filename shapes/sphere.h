#pragma once

#include "shape.h"

class Sphere : public Shape {
  Vec3 center = Vec3(0, 0, 0);
  double radius = 1;

public:
  Sphere() = default;
  Sphere(const Vec3 &c, double r) : Shape(), center(c), radius(r) {
    if (radius < 0)
      radius *= -1;
  }
  Sphere(const Vec3 &c, double r, Pixel &p, double d, Vec3 al)
      : Shape(p, d, al), center(c), radius(r) {
    if (radius < 0)
      radius *= -1;
  }
  Sphere(const Vec3 &c, double r, Pixel &p) : Shape(p), center(c), radius(r) {
    if (radius < 0)
      radius *= -1;
  }
  Sphere(const Vec3 &c, double r, Material &m)
      : Shape(m), center(c), radius(r) {
    if (radius < 0)
      radius *= -1;
  }

  Vec3 GetCenter() const { return center; }
  virtual Vec3 GetNormal(const Vec3 &point) const;

  virtual bool RayIntersect(const Vec3 &orig, const Vec3 &dir,
                            double &t0) const;
};

bool Sphere::RayIntersect(const Vec3 &orig, const Vec3 &dir, double &t0) const {
  Vec3 L = center - orig;
  double tca = L * dir;
  double d2 = L * L - tca * tca;
  if (d2 > radius * radius)
    return false;
  double thc = sqrtf(radius * radius - d2);
  t0 = tca - thc;
  double t1 = tca + thc;
  if (t0 < 0)
    t0 = t1;
  if (t0 < 0)
    return false;
  return true;
}

Vec3 Sphere::GetNormal(const Vec3 &point) const {
  // check if the given point is on the sphere
  // if (pow((point.I - center.I), 2) + pow((point.J - center.J), 2) +
  // pow((point.K - center.K), 2) != pow(radius, 2)) throw
  // std::invalid_argument("Sphere::GetNormal -> point not present on the
  // sphere");

  Vec3 normal = point - center;
  normal.N();

  return normal;
}