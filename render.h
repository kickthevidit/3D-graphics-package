#pragma once

#include <cmath>
#include <iostream>
#include <map>
#include <string>

#include "Vec3.h"
#include "pixel.h"
#include "shapes/shape.h"
#include "shapes/sphere.h"

using std::map;
using std::min;
using std::string;

namespace Render {
vector<Shape *> CreateObjects(map<string, Material> &mmap,
                              map<string, Pixel> &cmap) {
  vector<Shape *> objects;

  Sphere *b = new Sphere(Vec3(-0.08, -0.5, -6), 0.6, mmap.at("red_rubber"));
  Sphere *d = new Sphere({0, -1.5, -7}, 0.3, mmap.at("pearl"));
  //   Sphere *e = new Sphere({-4, -3, -20}, 1.5, mmap.at("mirror"));

  objects.push_back(b);
  objects.push_back(d);
  //   objects.push_back(e);

  return objects;
}

vector<Light> CreateLighting() {
  vector<Light> lights;

  lights.push_back(Light(Vec3(-20, 20, 20), 4.));
  lights.push_back(Light(Vec3(30, 50, -25), 3.));
  lights.push_back(Light(Vec3(30, 20, 30), 1.7));

  return lights;
}

void UpdateLighting(vector<Light> &l) { l[0].pos = l[0].pos + Vec3(0, 1, 0); }

vector<vector<Pixel>> CreateCanvas(const unsigned &width,
                                   const unsigned &height, Pixel &bg_col,
                                   Vec3 &orig, vector<Shape *> &objects,
                                   vector<Light> &lights) {
  vector<vector<Pixel>> img(height, vector<Pixel>(width, Pixel(0, 0, 0)));

  double fov = M_PI_2 / 3;

  for (unsigned i = 0; i < height; ++i) {
    for (unsigned j = 0; j < width; ++j) {
      double x = (2 * (i + 0.5) / (double)width - 1) * tan(fov / 2.) * width /
                 (double)height;
      double y = -(2 * (j + 0.5) / (double)height - 1) * tan(fov / 2.);
      Vec3 dir = Vec3(x, y, -1);
      dir.N(); // normalize

      img[i][j] = CastRay(orig, dir, objects, lights, bg_col);
    }
  }

  return img;
}

void WritePPM(vector<vector<Pixel>> &pixels, string &file_path) {

  if (file_path.substr(file_path.size() - 4, 4) != ".ppm")
    file_path += ".ppm";

  std::ofstream ofs;
  ofs.open(file_path);
  ofs << "P3\n" << pixels.at(0).size() << " " << pixels.size() << "\n255\n";
  for (vector<Pixel> &row : pixels) {
    for (Pixel &pixel : row) {
      ofs << pixel << "\t";
    }
    ofs << '\n';
  }
  ofs.close();
}

vector<uint8_t> PixelsToData(vector<vector<Pixel>> &p) {
  vector<uint8_t> out(p.at(0).size() * p.size() * 4, 0);
  for (unsigned y = 0; y < p.size(); ++y) {
    for (unsigned x = 0; x < p.at(y).size(); ++x) {
      Pixel &a = p.at(y).at(x);
      out.at(4 * y * p.at(y).size() + 4 * x) = a.X;
      out.at(4 * y * p.at(y).size() + 4 * x + 1) = a.Y;
      out.at(4 * y * p.at(y).size() + 4 * x + 2) = a.Z;
      out.at(4 * y * p.at(y).size() + 4 * x + 3) = 15;
    }
  }
  return out;
}