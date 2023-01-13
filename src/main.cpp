#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "pixel.h"
#include "render.cpp"
#include "shapes/line.h"

using std::map;
using std::string;
using std::vector;

/**
 * * Description: Throws an error that an invalid Command Line Argument is given
 *
 * * Input:
 * @param char* that is the invalid cli
 *
 * * Output:
 * None: throws invalid CLI
 *
 */
void throwErrorCLIArg(char *cli_arg) {
  string error_output =
      "Error: \'" + string(cli_arg) + "\' is not a valid argument. \n";
  throw std::invalid_argument(error_output);
}

/**
 *
 * Valid Command line Arguments:
 *
 * -o : file name to output into in the bin folder
 * -h : set height of output ppm image
 * -w : set width of output ppm image
 *
 */
int main(int argc, char *argv[]) {

  constexpr unsigned DEFAULT_WIDTH = 1024;
  constexpr unsigned DEFAULT_HEIGHT = 768;
  const string DEFAULT_FILE_NAME = "output_img";
  const Vec3 DEFAULT_CAMERA_POS(0, 0, 0);
  const Vec3 DEFAULT_CAMERA_DIR(1, 1, 1);

  unsigned width = DEFAULT_WIDTH;
  unsigned height = DEFAULT_HEIGHT;
  constexpr unsigned frames = 10;
  string file_name = DEFAULT_FILE_NAME;

  // parsing CLI args
  if (argc > 1) {
    int i = 1;
    for (; i < argc; ++i) {
      char *arg = argv[i];
      if (*arg == '-') {
        char *command = arg + 1;
        switch (*command) {
        case 'o':
          file_name = string(argv[++i]);
          break;
        case 'w':
          width = (unsigned)std::stoi(string(argv[++i]));
          break;
        case 'h':
          height = (unsigned)std::stoi(string(argv[++i]));
          break;
        default:
          throwErrorCLIArg(arg);
          break;
        }
      } else {
        throwErrorCLIArg(argv[i]);
      }
    }
  }

  string file_path = string("bin/").append(file_name);

  Vec3 orig(0, 0, 0);
  Pixel bg_col = Pixel(73, 178, 203);
  // Pixel bg_col = Pixel(255, 55, 255);

  vector<vector<Pixel>> sample_img(height,
                                   vector<Pixel>(width, Pixel(0, 0, 0)));

  map<string, Pixel> cmap = {
      {"Tech_Green", Pixel(33, 189, 3)}, {"Purple", Pixel(67, 0, 89)},
      {"Pink", Pixel(255, 51, 153)},     {"orange", Pixel(237, 128, 2)},
      {"yellow", Pixel(255, 247, 0)},    {"blue", Pixel(55, 0, 255)},
      {"red", Pixel(200, 0, 0)},         {"blue1", Pixel(4, 158, 209)},
      {"white", Pixel(255, 255, 255)},   {"pearl", Pixel(180, 190, 240)},
      {"red1", Pixel(80, 0, 0)},         {"black", Pixel(0, 0, 0)},
      {"grey", Pixel(211, 211, 211)},    {"grey-green", Pixel(47, 79, 79)},
      {"green", Pixel(108, 183, 53)},    {"glass_green", Pixel(209, 234, 197)}};
  map<string, Material> mmap = {
      {"ivory", Material(cmap.at("white"), 50., Vec3{0.6, 0.3, 0.1})},
      {"pearl", Material(cmap.at("pearl"), 45., Vec3{0.6, 0.3, 0.2})},
      {"red_rubber", Material(cmap.at("red1"), 10., Vec3(0.9, 0.1, 0.0))},
      {"mirror", Material(cmap.at("black"), 1425., Vec3(1, 1, 1))},
      {"plane_material", Material(cmap.at("white"), 0, Vec3(1, 0.0, 0.0))},
      {"line_material", Material(cmap.at("grey"), 0.f, Vec3(1, 1, 0))},
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