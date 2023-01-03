#pragma once

#include <cmath>
#include <iostream>

struct Vec3 {
  Vec3() = default;
  Vec3(double i, double j, double k) : I(i), J(j), K(k) {}
  Vec3(const Vec3 &copy) : I(copy.I), J(copy.J), K(copy.K) {}
  Vec3(double x) : I(x), J(x), K(x) {}
  ~Vec3() = default;

  Vec3 &operator=(const Vec3 &copy) {
    if (&copy == this)
      return *this;

    I = copy.I;
    J = copy.J;
    K = copy.K;
    return *this;
  }

  bool operator==(const Vec3 &vec) const {
    return (I == vec.I && J == vec.J && K == vec.K);
  }

  Vec3 operator+(const Vec3 &v) const {
    Vec3 r;
    r.I = this->I + v.I;
    r.J = this->J + v.J;
    r.K = this->K + v.K;
    return r;
  }

  Vec3 operator-(const Vec3 &v) const {
    Vec3 r;
    r.I = this->I - v.I;
    r.J = this->J - v.J;
    r.K = this->K - v.K;
    return r;
  }

  // dot product
  double operator*(const Vec3 &v) const {
    return (this->I * v.I) + (this->J * v.J) + (this->K * v.K);
  }

  Vec3 operator*(const double &d) const {
    Vec3 ret;
    ret.I = I * d;
    ret.J = J * d;
    ret.K = K * d;
    return ret;
  }

  double M() const { return sqrt(I * I + J * J + K * K); }

  void N() {
    I = I / M();
    J = J / M();
    K = K / M();
  }

  friend Vec3 operator*(double i, const Vec3 &v) {
    Vec3 r;
    r.I = v.I * i;
    r.J = v.J * i;
    r.K = v.K * i;
    return r;
  }

  friend Vec3 operator/(const Vec3 &v, double d) {
    Vec3 r;
    r.I = v.I / d;
    r.J = v.J / d;
    r.K = v.K / d;
    return r;
  }

  friend std::ostream &operator<<(std::ostream &o, const Vec3 &v);

  double I = 0.f;
  double J = 0.f;
  double K = 0.f;
};