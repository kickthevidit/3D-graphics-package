#include "Vec3.h"

Vec3 &Vec3::operator=(const Vec3 &copy) {
  if (&copy == this)
    return *this;

  I = copy.I;
  J = copy.J;
  K = copy.K;
  return *this;
}

bool Vec3::operator==(const Vec3 &vec) const {
  return (I == vec.I && J == vec.J && K == vec.K);
}

Vec3 Vec3::operator+(const Vec3 &v) const {
  Vec3 r;
  r.I = this->I + v.I;
  r.J = this->J + v.J;
  r.K = this->K + v.K;
  return r;
}

Vec3 Vec3::operator-(const Vec3 &v) const {
  Vec3 r;
  r.I = this->I - v.I;
  r.J = this->J - v.J;
  r.K = this->K - v.K;
  return r;
}

// dot product
double Vec3::operator*(const Vec3 &v) const {
  return (this->I * v.I) + (this->J * v.J) + (this->K * v.K);
}

Vec3 Vec3::operator*(const double &d) const {
  Vec3 ret;
  ret.I = I * d;
  ret.J = J * d;
  ret.K = K * d;
  return ret;
}

double Vec3::M() const { return sqrt(I * I + J * J + K * K); }

void Vec3::N() {
  I = I / M();
  J = J / M();
  K = K / M();
}

Vec3 operator*(double i, const Vec3 &v) {
  Vec3 r;
  r.I = v.I * i;
  r.J = v.J * i;
  r.K = v.K * i;
  return r;
}

Vec3 operator/(const Vec3 &v, double d) {
  Vec3 r;
  r.I = v.I / d;
  r.J = v.J / d;
  r.K = v.K / d;
  return r;
}

bool doubleEquals(const double d1, const double d2, double diff) {
  return (abs(d1 - d2) < diff);
}

bool Vec3::VecDoubleEquals(const Vec3 &V, const double diff) const {
  return (doubleEquals(V.I, I, diff) && doubleEquals(V.J, J, diff) &&
          doubleEquals(V.K, K, diff));
}

Vec3 Vec3::operator%(const Vec3 &v) const {
  return Vec3((J * v.K) - (v.J * K), (K * v.I) - (I * v.K),
              (I * v.J) - (J * v.I));
}

std::ostream &operator<<(std::ostream &o, const Vec3 &v) {
  o << v.I << ',' << v.J << ',' << v.K;
  return o;
}