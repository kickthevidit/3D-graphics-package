#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <cmath>
//#include <Eigen/Dense>

using std::vector;
using std::string;

struct Pixel {
	Pixel() = default;
	Pixel(unsigned x, unsigned y, unsigned z) {
        if (x < 0 || x > 255 || y < 0 || y > 255 || z < 0 || z > 255) throw std::invalid_argument("Pixel::Pixel() -> pixel constructor parameters out of bounds");
		X = x;
		Y = y;
		Z = z;
    }

	friend std::ostream& operator<<(std::ostream& os, const Pixel& p);

	unsigned X = 0;
	unsigned Y = 0;
	unsigned Z = 0;
};

std::ostream& operator<<(std::ostream& os, const Pixel& p) {
	os << p.X << ' ' << p.Y << ' ' << p.Z;
	return os;
}

struct Vec3 {
	Vec3() = default;
	Vec3(double i, double j, double k): I(i), J(j), K(k) {}
	Vec3(const Vec3& copy): I(copy.I), J(copy.J), K(copy.K) {}
	
	Vec3& operator=(const Vec3& copy) {
		if (&copy == this) return *this;

		I = copy.I;
		J = copy.J;
		K = copy.K;
		return *this;
	}

	Vec3 operator+(const Vec3& v) const {
		Vec3 r;
		r.I = this->I + v.I;
		r.J = this->J + v.J;
		r.K = this->K + v.K;
		return r;
	}

	Vec3 operator-(const Vec3& v) const {
		Vec3 r;
		r.I = this->I - v.I;
		r.J = this->J - v.J;
		r.K = this->K - v.K;
		return r;
    }

	// dot product
	double operator*(const Vec3& v) const {
		return (this->I * v.I) + (this->J * v.J) + (this->K * v.K); 
	}

	double M() {
		return sqrt(I*I + J * J + K * K);
	}

	void N() {
		I = I / M();
		J = J / M();
		K = K / M();
	}

	friend Vec3 operator*(double i, const Vec3& v) {
		Vec3 r;
		r.I = v.I * i;
		r.J = v.J * i;
		r.K = v.K * i;
		return r;	
	}

	double I = 0.f;
	double J = 0.f;
	double K = 0.f;
};

class Shape {
public:
	Shape() = default;
	Shape(Pixel& p): color(p) {}

	virtual bool RayIntersect(const Vec3& orig, const Vec3& dir, double& t0) const {return false;}// check if a ray intersects with our shape

	Pixel GetColor() const {return color;}

	bool CastRay(const Vec3 & orig, const Vec3& dir, const Shape& sh) const;
private:
	Pixel color = Pixel{0,0,0};
};

bool Shape::CastRay(const Vec3& orig, const Vec3& dir, const Shape& sh) const {
	double shape_dist = std::numeric_limits<double>::max();
	return (sh.RayIntersect(orig, dir, shape_dist));
}

class Sphere: public Shape {
	Vec3 center = Vec3(0, 0, 0);
	double radius = 1;

public:
	Sphere() = default;
	Sphere(const Vec3& c, double r): center(c), radius(r), Shape() {}
	Sphere(const Vec3& c, double r, Pixel& p): center(c), radius(r), Shape(p) {}

	virtual bool RayIntersect(const Vec3 &orig, const Vec3 &dir, double& t0) const;
};

bool Sphere::RayIntersect(const Vec3& orig, const Vec3& dir, double& t0) const {	
	Vec3 L = center - orig;
	double tca = L * dir;
	double d2 = L * L - tca * tca;
	if (d2 > radius * radius) return false;
	double thc = sqrtf(radius * radius - d2);
	t0 = tca - thc;
	double t1 = tca + thc;
	if (t0 < 0) t0 = t1;
	if (t0 < 0) return false;
	return true;
}


class Line: public Shape {
	Vec3 orig = Vec3(0,0,0);
	Vec3 dir = Vec3(1,1,-1);

public:
	Line() = default;
	Line(const Vec3& o, const Vec3& d): orig(o), dir(d), Shape() {
		if (dir.I == 0 && dir.J == 0 && dir.K == 0) throw std::invalid_argument("Line::Line() -> dir cannot be 0");
	}
	Line(const Vec3& o, const Vec3& d, Pixel& p): orig(o), dir(d), Shape(p) {
		if (dir.I == 0 && dir.J == 0 && dir.K == 0) throw std::invalid_argument("Line::Line() -> dir cannot be 0");
    }


	bool RayIntersect(const Vec3& orig, const Vec3& dir, double& t0) const {
		return false;
	}
};

/* class Canvas {
        unsigned width = 0;
        unsigned height = 0;

        vector<Shape> objects;
        vector<vector<Pixel>> env = {};
        Pixel bg_col = Pixel(52, 61, 82);
public:
        Canvas() = default;
        Canvas(unsigned w, unsigned h) : width(w), height(h), bg_col(Pixel(52, 61, 82)) ,env(vector<vector<Pixel>>(h, vector<Pixel>(w, bg_col))), objects(vector<Shape>()) {}

        Pixel GetBg() const {return bg_col;}

        void Render() const; // function that renders this shape onto the canvas
}; */

namespace Render {
	void WritePPI(vector<vector<Pixel>>& pixels, string& file_path) {

		if (file_path.substr(file_path.size()-4, 4) != ".ppm") file_path+=".ppm";

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
	// clear; clang++ -std=c++20 main.cpp

	constexpr unsigned width = 1024;
	constexpr unsigned height = 1024; // bug where if canvas is not square it doesn't work
	const Vec3 orig(0,0,0);
    Pixel bg_col = Pixel(52, 61, 82);

	string file_path = "test/img/1";
	vector<vector<Pixel>> sample_img(height, vector<Pixel>(width, Pixel(0,0,0)));
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			sample_img[i][j] = bg_col; // base color
		}
	}

	vector<Vec3> positions = {Vec3(0, 0, -8), Vec3(0, 0, -1000)};
	vector<Pixel> colors = {Pixel(33, 189, 3), Pixel(67, 0, 89)};
	vector<Shape*> objects;
	Sphere *a = new Sphere(positions[0], 1, colors[0]);
	Sphere *b = new Sphere(positions[1], 500, colors[1]);
	objects.push_back(b);
	objects.push_back(a);

    double fov = atan((double) width / 2);

	for (Shape* s: objects) {
		for (unsigned i = 0; i < height; ++i) {
			for (unsigned j = 0; j < width; ++j) {
				double x = (2 * (i + 0.5) / (double) width - 1) * tan(fov / 2.) * width / (double) height;
                double y = -(2 * (j + 0.5) / (double) height - 1) * tan(fov / 2.);
				Vec3 dir = Vec3(x, y, -1);
				dir.N(); // normalize
				if (s->CastRay(orig, dir, *s)) {
					sample_img[i][j] = s->GetColor();
				}
				//std::cout << '(' << i << ',' << j << ')' << ':' << sample_img[i][j] << '\t' << bg_col << '/' << s->GetColor() << '\n';
            }
		}
	}

	Render::WritePPI(sample_img, file_path);

	std::cout << "Successfully Finished\n";
}



