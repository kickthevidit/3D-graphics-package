#pragma once

#include <cmath>
#include <iostream>

#include "../material.h"
#include "../pixel.h"

class Shape {
public:
  Shape() = default;
  Shape(Pixel &p, double s, Vec3 &al) : M(p, s, al) {}
  Shape(Pixel &p) : M(p) {}
  Shape(Material &m) : M(m) {}

  virtual bool RayIntersect(const Vec3 &orig, const Vec3 &dir, double &t0) const {
    return false;
  }

  Pixel GetColor() const { return M.diffuse_color; }
  Material GetMaterial() const { return M; }

  virtual Vec3 GetNormal(const Vec3 &point) const { return Vec3(0, 0, 0); }

private:
  Material M = Material();
};