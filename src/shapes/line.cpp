#include "line.h"

void Line::TestArgs(Vec3 &point, Vec3 &dir, double &radius) {
  if (dir == Vec3(0, 0, 0))
    throw std::invalid_argument(
        "Line::TestArgs() -> Dir Vector cannot be 0,0,0");
  if (radius < 0)
    throw std::invalid_argument(
        "Line::TestArgs() -> thickness has to be non-negative");
}

Vec3 Line::GetNormal(const Vec3 &point) const {
  return Vec3(1, -(dir_.I + dir_.K), 1 + (0.f * point.I));
}

bool Line::LightingAware() const { return lighting_aware; }

bool Line::RayIntersect(const Vec3 &orig, const Vec3 &dir, double &t0) const {

  // TODO: Fix bug where connects is nan. Recreate usig orig_ = (0,0,1), dir_ =
  // (0,1,0). Probably a divide by 0 error when dir_.I and dir_.J are 0.

  if (orig_ == Vec3(0, 0, 0))
    return false;

  double a = (abs((dir % dir_) * 2 * (orig - orig_)) / (dir % dir_).M());

  // std::cout << "a: " << a << '\n';

  bool connects = doubleEquals(a, 0.f, radius_);

  if (connects) {
    Vec3 intersection =
        orig_ + ((double)((1.f / (1.f - (dir_.I * dir.J) / (dir_.J * dir.I))) *
                          ((((1.f / dir_.J) * (orig_.J - orig.J)) +
                            (orig.I - orig_.I)) /
                           dir_.I)) *
                 dir_);
    t0 = (dir - intersection).M();
  }

  return connects;
}
