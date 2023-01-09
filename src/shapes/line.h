#pragma once

#include "shape.h"

class Line : public Shape {
  Vec3 orig_ = Vec3(0, 0, 0);
  Vec3 dir_ = Vec3(1, 1, 1); // non zero
  bool lighting_aware = false;

  void TestArgs(Vec3 &point, Vec3 &dir);
  void _GetNormalHelper(const Vec3 &point);

public:
  Line() = default;
  Line(Vec3 &pt, Vec3 &dir) : Shape(), orig_(pt), dir_(dir) {
    TestArgs(pt, dir);
  }
  Line(Pixel &p, double specularity, Vec3 &al, Vec3 &point, Vec3 &dir)
      : Shape(p, specularity, al), orig_(point), dir_(dir) {
    TestArgs(point, dir);
  }
  Line(Pixel &p, double specularity, Vec3 &al)
      : Shape(p, specularity, al), orig_(0, 0, 0), dir_(1, 1, 1) {}
  Line(Material m, Vec3 orig, Vec3 dir) : Shape(m), orig_(orig), dir_(dir) {}

  Vec3 getOrig() const { return orig_; }
  Vec3 getDir() const { return dir_; }

  /**
   * Input: point where ray of light hits a point onto this line.
   * Output:
   * Description:
   */
  virtual Vec3 GetNormal(const Vec3 &point) const;

  /**
   * Input:
   * Output:
   * Description:
   */
  virtual bool LightingAware() const;

  /**
   * Input:
   * Output:
   * Description:
   */
  virtual bool RayIntersect(const Vec3 &orig, const Vec3 &dir,
                            double &t0) const;
};
