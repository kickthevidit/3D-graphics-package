#include "env.h"

Pixel Environment::CastRay(Pos& orig, Dir& dir, Object& obj) {
	float obj_dist = std::numeric_limits<float>::max();

	if (!obj.RayIntersect(orig, dir, obj_dist)) {
		return Pixel {77, 137, 99};
	}
	return Pixel {159, 2, 81};
}

void Environment::RenderObj(const Object& obj) {
	constexpr float fov = 1.05; // 60 degree field of view in radians
	for (size_t y = 0; y < h_; ++y) {
		for (size_t x = 0; x < w_; ++x) {
			float a = (2 * (x + 0.5) / (float)w_ - 1) * tan(fov / 2.) * w_ / (float) h_;
			float b = -(2 * (y + 0.5) / (float)h_ - 1) * tan(fov / 2.);
        }
	}
}

