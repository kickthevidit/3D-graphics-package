#include "pixel.h"

std::ostream &operator<<(std::ostream &os, const Pixel &p) {
  os << p.X << ' ' << p.Y << ' ' << p.Z;
  return os;
}

// multiplication with a ceiling of 255
Pixel Pixel::operator*(const double &d) {
  Pixel a;
  a.X = (X * d > 255) ? 255 : X * d;
  a.X = (X * d < 0) ? 0 : X * d;
  a.Y = (Y * d > 255) ? 255 : Y * d;
  a.Y = (Y * d < 0) ? 0 : Y * d;
  a.Z = (Z * d > 255) ? 255 : Z * d;
  a.Z = (Z * d < 0) ? 0 : Z * d;
  return a;
}

Pixel Pixel::operator+(const Pixel &p) {
  return Pixel(min((unsigned)255, X + p.X), min((unsigned)255, Y + p.Z),
               min((unsigned)255, Z + p.Z));
}