#pragma once

#include "Vec3.h"
#include "pixel.h"

struct Material {
  Material() = default;
  Material(Pixel &d, double s, Vec3 al)
      : diffuse_color(d), specular_exponent(s), albedo(al) {}
  Material(Pixel &p) : diffuse_color(p) {}

  Pixel diffuse_color = Pixel(255, 255, 255);
  double specular_exponent = 8;
  Vec3 albedo =
      Vec3{1, 1, 1}; // diffuse color, specular color, reflective color
};