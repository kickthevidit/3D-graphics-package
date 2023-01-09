#pragma once

#include <cmath>
#include <iostream>


#include "../material.h"
#include "../pixel.h"

class Shape {
public:
  Shape() = default;
  Shape(Pixel &p, double specularity, Vec3 &al) : M(p, specularity, al) {}
  Shape(Pixel &p) : M(p) {}
  Shape(Material &m) : M(m) {}

  /**
   * Description:
   * Input: 
   * Output: 
   */
  virtual bool RayIntersect(const Vec3 &orig, const Vec3 &dir,
                            double &t0) const {
    return false;
  }

  // Return Color of Line
  Pixel GetColor() const { return M.diffuse_color; }

  // Return Material of Line
  Material GetMaterial() const { return M; }

  // Return Normal at the point on the shape
  virtual Vec3 GetNormal(const Vec3 &point) const { return Vec3(0, 0, 0); }

  /**
   * Description: Return whether lighting should effect the shape
   * Input: None
   * Output: bool representing whether lighting processes should work
   */
  virtual bool LightingAware() const { return true; }

private:
  Material M = Material();
};