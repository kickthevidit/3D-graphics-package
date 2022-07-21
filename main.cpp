#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using std::vector;
using std::string;

struct Pixel {
	Pixel(unsigned x, unsigned y, unsigned z) {
        if (x < 0 || x > 255 || y < 0 || y > 255 || z < 0 || z > 255) throw std::invalid_argument("Pixel::Pixel() -> pixel constructor parameters out of bounds");
		X = x;
		Y = y;
		Z = z;
    }

	friend std::ostream& operator<<(std::ostream& os, const Pixel& p);

	unsigned X;
	unsigned Y;
	unsigned Z;
};

std::ostream& operator<<(std::ostream& os, const Pixel& p) {
	os << p.X << ' ' << p.Y << ' ' << p.Z;
	return os;
}

namespace Render {
	void WritePPI(vector<vector<Pixel>>& pixels, string& file_path) {
		std::ofstream ofs;
		ofs.open(file_path);
		ofs << "P3\n" << pixels.size() << " " << pixels.at(0).size() << "\n255\n";
		for (vector<Pixel>& row: pixels) {
			for (Pixel& pixel: row) {
				ofs << pixel << "\t";
			}
			ofs << '\n';
		}
		ofs.close();
	}
}

int main() {
	// clear; clang++ -std=c++20 main.cpp; ./a.out
	string file_path = "test/img/1.ppm";
	vector<vector<Pixel>> sample_img(1024, vector<Pixel>(768, Pixel(0,0,0)));
	for (int i = 0; i < 1024; ++i) {
		for (int j = 0; j < 768; ++j) {
			// img[i][j] = Pixel((i*103) % 256,(j * 1023) % 256,((i+j)* 10234)% 256 * 0);
			// img[i][j] = Pixel(2*(i+j)%256,abs(j-i)%256,abs(i * j)%256);
			img[i][j] = Pixel(0,0,abs(i * j)%256);
		}
	}

	Render::WritePPI(sample_img, file_path);
}

