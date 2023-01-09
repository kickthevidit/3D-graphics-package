#include "line.h"

void Line::TestArgs(Vec3 &point, Vec3 &dir) {
  if (dir == Vec3(0, 0, 0))
    throw std::invalid_argument(
        "Line::TestArgs() -> Dir Vector cannot be 0,0,0");
}

Vec3 Line::GetNormal(const Vec3 &point) const {
  return Vec3(1, -(dir_.I + dir_.K), 1 + (0.f * point.I));
}

bool Line::LightingAware() const { return lighting_aware; }

bool Line::RayIntersect(const Vec3 &orig, const Vec3 &dir, double &t0) const {

  // TODO: Fix bug where connects is nan. Recreate usig orig_ = (0,0,1), dir_ =
  // (0,1,0). Probably a divide by 0 error .

  bool connects = doubleEquals(
      (abs((dir % dir_) * 2 * (orig - orig_)) / (dir % dir_).M()), 0.f, 0.001);

  if (connects) {
    Vec3 intersection =
        orig_ + ((double)((1.f / (1.f - (dir_.I * dir.J) / (dir_.J * dir.I))) *
                          ((((1.f / dir_.J) * (orig_.J - orig.J)) +
                            (orig.I - orig_.I)) /
                           dir_.I)) *
                 dir_);
    t0 = (dir - intersection).M();
    // std::cout << "Distance: " << t0 << '\n';

    // std::cout << "Running Line Intersect between: ";
    // std::cout << orig_ << '\t' << dir_ << '\n';
    // std::cout << (connects ? "Conects: " : "No Connection: ") << t0 << '\n';
  }

  return connects;
}
