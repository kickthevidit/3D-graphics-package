#pragma once

#include <iostream>
#include <vector>

#include "material.h"
#include "shapes/shape.h"

using std::vector;

struct Light {
  Light() = default;
  Light(const Vec3 &p, const double &i) : pos(p), intensity(i) {}

  Vec3 pos = Vec3(0, 0, 0);
  double intensity = 0.5;
};

Vec3 Reflect(const Vec3 &I, const Vec3 &N) {
  return I - N * (double)2 * (I * N);
}

bool EnvironmentIntersect(const Vec3 &orig, const Vec3 &dir,
                          const vector<Shape *> &objects, double &shape_dist,
                          Vec3 &point, Vec3 &point_n, Material &obj_mat) {
  double min_dist = std::numeric_limits<double>::max();
  for (Shape *s : objects) {
    double dist = std::numeric_limits<double>::max();
    if (s->RayIntersect(orig, dir, dist) && dist < min_dist) {
      min_dist = dist;
      point = orig + dir * dist;
      point_n = s->GetNormal(point);
      if (point_n == Vec3(0, 0, 0))
        std::cout << point << ',' << point_n << '\n';
      obj_mat = s->GetMaterial();
    }
  }
  shape_dist = min_dist;

  double checkerboard_dist = std::numeric_limits<double>::max();
  if (fabs(dir.I) > 1e-3) {
    double d = -(orig.I - 0.5) / dir.I;
    Vec3 pt = orig + dir * d;
    if (d > 0 && fabs(pt.J) < 100 && pt.K > -300 && d < min_dist) {
      checkerboard_dist = d;
      point = pt;
      point_n = Vec3(1, 0, 0);
      obj_mat.diffuse_color = (int(.5 * point.J + 1000) + int(.5 * point.K)) & 1
                                  ? Vec3(1, 1, 1) * 255
                                  : Vec3(1, .7, .3) * 255;
      obj_mat.diffuse_color = obj_mat.diffuse_color * .3;
      obj_mat.albedo = Vec3(0.9, 0.1, 0.);
    }
  }

  return min(min_dist, checkerboard_dist) < std::numeric_limits<double>::max();
}

Pixel CastRay(const Vec3 &orig, const Vec3 &dir, const vector<Shape *> &objects,
              const vector<Light> &lights, Pixel &bg_col, unsigned depth = 0) {
  Vec3 coord, coord_n;
  Material mat;
  double min_dist = std::numeric_limits<double>::max();

  if (depth > 4 ||
      !EnvironmentIntersect(orig, dir, objects, min_dist, coord, coord_n, mat))
    return bg_col;

  Vec3 reflect_dir = Reflect(dir, coord_n);
  Vec3 reflect_orig =
      reflect_dir * coord_n < 0
          ? coord - coord_n * 1e-3
          : coord + coord_n *
                        1e-3; // shift the orig to prevent obstruction by object
  Pixel reflect_color =
      CastRay(reflect_orig, reflect_dir, objects, lights, bg_col, depth + 1);

  double diffuse_light_intensity = 0, specular_light_intensity = 0;
  for (const Light &l : lights) {
    Vec3 light_dir = (l.pos - coord);
    light_dir.N(); // convert to unit vector
    double light_distance = (l.pos - coord).M();
    double shadow_dist;

    // shadows processing
    Vec3 shadow_orig =
        light_dir * coord_n < 0
            ? coord - coord_n * 1e-3
            : coord + coord_n * 1e-3; // shift point a bit to prevent collision
    Vec3 shadow_coord, shadow_n;
    Material tmp_mat;
    if ((EnvironmentIntersect(shadow_orig, light_dir, objects, shadow_dist,
                              shadow_coord, shadow_n, tmp_mat)) &&
        ((shadow_coord - shadow_orig).M() < light_distance))
      continue;

    diffuse_light_intensity +=
        l.intensity * fmax((double)0, light_dir * coord_n);
    // if (diffuse_light_intensity == 0) std::cout << l.intensity << ',' <<
    // light_dir * coord_n << '\n';
    specular_light_intensity +=
        pow(fmax((double)0, Reflect(light_dir, coord_n) * dir),
            mat.specular_exponent) *
        l.intensity;
  }
  Pixel out = mat.diffuse_color * diffuse_light_intensity * mat.albedo.I +
              Pixel(Vec3(1, 1, 1)) * specular_light_intensity * mat.albedo.J +
              reflect_color * mat.albedo.K;

  return out;
}
