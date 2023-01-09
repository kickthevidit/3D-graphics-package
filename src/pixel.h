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

  Pixel operator*(const double &d);
  Pixel operator+(const Pixel &p);

  unsigned X = 0;
  unsigned Y = 0;
  unsigned Z = 0;
};
