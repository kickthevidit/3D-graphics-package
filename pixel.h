#pragma once

#include <cmath>
#include <iostream>

#include "Vec3.h"

using std::min;

struct Pixel {
  Pixel() = default;
  Pixel(unsigned x, unsigned y, unsigned z) {
    if (x < 0 || x > 255 || y < 0 || y > 255 || z < 0 || z > 255)
      throw std::invalid_argument(
          "Pixel::Pixel() -> pixel constructor parameters out of bounds");
    X = x;
    Y = y;
    Z = z;
  }
  Pixel(const Vec3 &v)
      : X(min((unsigned)255, (unsigned)v.I)),
        Y(min((unsigned)255, (unsigned)v.J)),
        Z(min((unsigned)255, (unsigned)v.K)) {}

  friend std::ostream &operator<<(std::ostream &os, const Pixel &p);

  // multiplication with a ceiling of 255
  Pixel operator*(const double &d) {
    Pixel a;
    a.X = (X * d > 255) ? 255 : X * d;
    a.X = (X * d < 0) ? 0 : X * d;
    a.Y = (Y * d > 255) ? 255 : Y * d;
    a.Y = (Y * d < 0) ? 0 : Y * d;
    a.Z = (Z * d > 255) ? 255 : Z * d;
    a.Z = (Z * d < 0) ? 0 : Z * d;
    return a;
  }

  Pixel operator+(const Pixel &p) {
    return Pixel(min((unsigned)255, X + p.X), min((unsigned)255, Y + p.Z),
                 min((unsigned)255, Z + p.Z));
  }

  unsigned X = 0;
  unsigned Y = 0;
  unsigned Z = 0;
};

std::ostream &operator<<(std::ostream &os, const Pixel &p) {
  os << p.X << ' ' << p.Y << ' ' << p.Z;
  return os;
}