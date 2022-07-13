#ifndef ENV_
#define ENV_

#include "objects.h"
#include "pixel.h"

#include <limits>
#include <vector>
#include <cmath>

using std::vector;

class Environment {
public:
	Environment() = default;
	Environment(size_t width, size_t height): pixels_(height, vector<Pixel>(width, Pixel{0,0,0})) {}

	Pixel CastRay(Pos& orig, Dir& dir, Object& obj);

	void RenderObj(const Object& obj);

private:

	vector<Object> objs;
	vector<vector<Pixel>> pixels_;
	size_t w_;
	size_t h_;
};

#endif