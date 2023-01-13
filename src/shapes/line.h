#pragma once

#include "shape.h"

class Line : public Shape {
  Vec3 orig_ = Vec3(0, 0, 0);
  Vec3 dir_ = Vec3(1, 1, 1); // non zero
  double radius_ = 0.05;     // unsigned
  bool lighting_aware = false;

  /**
   * * Description: Checks if args for Class constructors are valid values.
   * Else, throws errors.
   *
   * * Input: input orig, input dir, input radius
   *
   * * Output: None
   *
   */
  void TestArgs(Vec3 &point, Vec3 &dir, double &radius);

public:
  Line() = default;
  Line(Vec3 &pt, Vec3 &dir, double radius)
      : Shape(), orig_(pt), dir_(dir), radius_(radius) {
    TestArgs(pt, dir, radius);
  }
  Line(Pixel &p, double specularity, Vec3 &al, Vec3 &point, Vec3 &dir,
       double radius)
      : Shape(p, specularity, al), orig_(point), dir_(dir), radius_(radius) {
    TestArgs(point, dir, radius);
  }
  Line(Pixel &p, double specularity, Vec3 &al, double radius)
      : Shape(p, specularity, al), orig_(0, 0, 0), dir_(1, 1, 1),
        radius_(radius) {}
  Line(Material m, Vec3 orig, Vec3 dir, double radius)
      : Shape(m), orig_(orig), dir_(dir), radius_(radius) {
    TestArgs(orig, dir, radius);
  }

  /**
   * * Description: Getter for Orig
   *
   * * Input: None
   *
   * * Output: value of orig_
   *
   */
  Vec3 getOrig() const { return orig_; }
  /**
   * * Description: Getter for dir_ field
   *
   * * Input: None
   *
   * * Output: value of dir_ field
   *
   */
  Vec3 getDir() const { return dir_; }

  virtual Vec3 GetNormal(const Vec3 &point) const;

  virtual bool LightingAware() const;

  virtual bool RayIntersect(const Vec3 &orig, const Vec3 &dir,
                            double &t0) const;
};
