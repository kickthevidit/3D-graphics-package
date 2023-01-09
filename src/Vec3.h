#pragma once

#include <cmath>
#include <iostream>

struct Vec3 {
  Vec3() = default;
  Vec3(double i, double j, double k) : I(i), J(j), K(k) {}
  Vec3(const Vec3 &copy) : I(copy.I), J(copy.J), K(copy.K) {}
  Vec3(double x) : I(x), J(x), K(x) {}
  ~Vec3() = default;

  Vec3 &operator=(const Vec3 &copy);
  bool operator==(const Vec3 &vec) const;
  Vec3 operator+(const Vec3 &v) const;
  Vec3 operator-(const Vec3 &v) const;
  double operator*(const Vec3 &v) const; // dot product
  Vec3 operator%(const Vec3 &v) const;   // cross product
  Vec3 operator*(const double &d) const;

  double M() const;
  void N();

  friend Vec3 operator*(double i, const Vec3 &v);
  friend Vec3 operator/(const Vec3 &v, double d);
  friend std::ostream &operator<<(std::ostream &o, const Vec3 &v);

  bool VecDoubleEquals(const Vec3 &v, const double diff) const;

  double I = 0.f;
  double J = 0.f;
  double K = 0.f;
};

bool doubleEquals(const double d1, const double d2, const double diff);