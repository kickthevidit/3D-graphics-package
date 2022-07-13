#ifndef RENDER_
#define RENDER_

#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h" // image output library
#include "pixel.h"

using std::vector;

namespace Render {

void MakeJPG(char const* file_name, vector<vector<Pixel>>& data);

}



#endif

