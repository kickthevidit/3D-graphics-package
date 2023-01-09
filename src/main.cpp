#include <iostream>
#include <string>
#include <vector>

// #include "Vec3.h"
#include "pixel.h"
#include "render.cpp"
#include "shapes/line.h"

using std::string;
using std::vector;

int main(int argc, char *argv[]) {

  // clear; clang++ -std=c++20 main.cpp; ./a.out
  // clear; clang++ -std=c++20 main.cpp

  if (argc > 1) {
    std::cout << "Input args: ";
    for (int i = 1; i < argc; ++i)
      std::cout << argv[i] << '\t';
    std::cout << '\n';
  }

  /*
  Testing Line->RayIntersect

  Vec3 p1(1, 1, 0), d1(2, 1, 1);
  Vec3 p2(0, 0, 0), d2(3, 2, 1);

  Line a(p1, d1);
  Line b(p2, d2);
  double t0 = 0.f;

  std::cout << ((a.RayIntersect(b.getOrig(), b.getDir(), t0))
                    ? "Intersects"
                    : "Does not Intersect")
            << '\n'; */

  constexpr unsigned width = 1024;
  constexpr unsigned height = 768;
  constexpr unsigned frames = 10;
  Vec3 orig(0, 0, 0);
  //   Pixel bg_col = Pixel(73, 178, 203);
  Pixel bg_col = Pixel(255, 55, 255);

  string file_path = "bin/output3.ppm";
  vector<vector<Pixel>> sample_img(height,
                                   vector<Pixel>(width, Pixel(0, 0, 0)));

  map<string, Pixel> cmap = {
      {"Tech_Green", Pixel(33, 189, 3)}, {"Purple", Pixel(67, 0, 89)},
      {"Pink", Pixel(255, 51, 153)},     {"orange", Pixel(237, 128, 2)},
      {"yellow", Pixel(255, 247, 0)},    {"blue", Pixel(55, 0, 255)},
      {"red", Pixel(200, 0, 0)},         {"blue1", Pixel(4, 158, 209)},
      {"white", Pixel(255, 255, 255)},   {"pearl", Pixel(180, 190, 240)},
      {"red1", Pixel(80, 0, 0)},         {"black", Pixel(0, 0, 0)},
      {"green", Pixel(108, 183, 53)},    {"glass_green", Pixel(209, 234, 197)}};
  map<string, Material> mmap = {
      {"ivory", Material(cmap.at("white"), 50., Vec3{0.6, 0.3, 0.1})},
      {"pearl", Material(cmap.at("pearl"), 45., Vec3{0.6, 0.3, 0.2})},
      {"red_rubber", Material(cmap.at("red1"), 10., Vec3(0.9, 0.1, 0.0))},
      {"mirror", Material(cmap.at("black"), 1425., Vec3(1, 1, 1))},
      {"plane_material", Material(cmap.at("white"), 0, Vec3(1, 0.0, 0.0))},
      {"line_material", Material(cmap.at("green"), 0.f, Vec3(1, 1, 0))},
  };
  vector<Shape *> objects = Render::CreateObjects(mmap, cmap);
  vector<Light> lights = Render::CreateLighting();

  sample_img =
      Render::CreateCanvas(width, height, bg_col, orig, objects, lights);

  Render::WritePPM(sample_img, file_path);

  std::cout << "Successfully Finished\n";

  std::ofstream ofs;

  return 0;
};