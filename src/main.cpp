#include <iostream>
#include <string>
#include <vector>

#include "render.h"

using std::string;
using std::vector;

int main() {
	vector<vector<Pixel>> out(500, vector<Pixel>(500, Pixel{123, 141, 16}));

	Render::MakeJPG("test/img/1.jpg", out);

	std::cout << "Hello World!" << '\n';
}