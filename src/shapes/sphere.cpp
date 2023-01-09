#include "sphere.h"

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

  Vec3 normal = point - center;
  normal.N();

  return normal;
}