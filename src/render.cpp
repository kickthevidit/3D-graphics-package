#include "render.h"

// Render::Write(char const *filename, int w = 1024, int h = 737, int comp = 3, const void *data,
//                  int stride_in_bytes) {
// 	stbi_write_png(filename, w, h, comp, const void *data, int stride_in_bytes);
// }


void Render::MakeJPG(char const* file_name, vector<vector<Pixel>>& data) {
    const unsigned w = data.size();
    const unsigned h = data[0].size();
	constexpr unsigned channels = 3;

	uint8_t* pixels = new uint8_t(w * h * channels);

	unsigned i = 0;
	for (auto& a: data) {
		for (auto& b: a) {
			pixels[++i] = b.x;
			pixels[++i] = b.y;
			pixels[++i] = b.z;
		}
	}

	// vector<unsigned char> pixmap(w * h * 3);
	// for (size_t i = 0; i < w * h; ++i) {

	// 	for (size_t j = 0; j < 3; ++j) {
	// 		pixmap[i * 3 + j] = (unsigned char) (255 * )
	// 	}
	// }

    stbi_write_jpg(file_name, w, h, channels, pixels, 90);
}