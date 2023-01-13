#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "Vec3.h"
#include "light.h"
#include "material.h"
#include "pixel.h"
#include "shapes/checkerboard.h"
#include "shapes/line.h"
#include "shapes/sphere.h"

using std::map;
using std::min;
using std::string;

namespace Render {
/**
 * Description: Temporary Helper function to create all objects in environment
 * and return its vector
 *
 * Input:  map objects of materials and colors
 *
 * Output: vector of objects in our 3D environment
 */
vector<Shape *> CreateObjects(map<string, Material> &mmap,
                              map<string, Pixel> &cmap) {
  vector<Shape *> objects;

  Sphere *b = new Sphere(Vec3(-0.08, -0.5, -6), 0.6, mmap.at("red_rubber"));
  Sphere *d = new Sphere({0, -1.5, -7}, 0.3, mmap.at("pearl"));
  vector<Line *> lines;

  for (int i = 0; i < 10; ++i) {
    lines.push_back(new Line(mmap.at("line_material"), Vec3(i * 0.5, 0, -5),
                             Vec3(0.000000001, 1, 0), 0.01));
    lines.push_back(new Line(mmap.at("line_material"), Vec3(i * -0.5, 0, -5),
                             Vec3(0.000000001, 1, 0), 0.01));
    lines.push_back(new Line(mmap.at("line_material"), Vec3(0, i * 0.5, -5),
                             Vec3(1, 0.0000000001, 0), 0.01));
    lines.push_back(new Line(mmap.at("line_material"), Vec3(0, i * -0.5, -5),
                             Vec3(1, 0.0000000001, 0), 0.01));
  };

  objects.push_back(b);
  objects.push_back(d);
  for (auto &a : lines)
    objects.push_back(a);

  return objects;
}

vector<Light> CreateLighting() {
  vector<Light> lights;

  lights.push_back(Light(Vec3(-20, 20, 20), 4.));
  lights.push_back(Light(Vec3(30, 50, -25), 3.));
  // lights.push_back(Light(Vec3(30, 20, 30), 1.7));

  return lights;
}

void UpdateLighting(vector<Light> &l) { l[0].pos = l[0].pos + Vec3(0, 1, 0); }

vector<vector<Pixel>> CreateCanvas(const unsigned &width,
                                   const unsigned &height, Pixel &bg_col,
                                   Vec3 &orig, vector<Shape *> &objects,
                                   vector<Light> &lights) {
  vector<vector<Pixel>> img(height, vector<Pixel>(width, Pixel(0, 0, 0)));

  constexpr double fov = M_PI_2 / 3;

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
  std::cout << "Starting file WritePPM\n";
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

  std::cout << "Written file to file path: " << file_path << '\n';
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
} // namespace Render