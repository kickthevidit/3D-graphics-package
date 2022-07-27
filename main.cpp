#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <cmath>
#include <map>
#include <array>

using std::vector;
using std::string;
using std::max;
using std::min;
using std::map;
using std::array;

struct Vec3 {
	Vec3() = default;
	Vec3(double i, double j, double k): I(i), J(j), K(k) {}
	Vec3(const Vec3& copy): I(copy.I), J(copy.J), K(copy.K) {}
	Vec3(double x): I(x), J(x), K(x) {}
	
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

	Vec3 operator*(const double& d) const {
		Vec3 ret;
		ret.I = I * d;
		ret.J = J * d;
		ret.K = K * d;
		return ret;
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

	friend std::ostream& operator<<(std::ostream& o, const Vec3& v);

	double I = 0.f;
	double J = 0.f;
	double K = 0.f;
};

struct Pixel {
	Pixel() = default;
	Pixel(unsigned x, unsigned y, unsigned z) {
        if (x < 0 || x > 255 || y < 0 || y > 255 || z < 0 || z > 255) throw std::invalid_argument("Pixel::Pixel() -> pixel constructor parameters out of bounds");
		X = x;
		Y = y;
		Z = z;
    }
	Pixel(const Vec3& v): X(min((unsigned )255, (unsigned)v.I)), Y(min((unsigned )255,(unsigned) v.J)), Z(min((unsigned )255, (unsigned) v.K)) {}

	friend std::ostream& operator<<(std::ostream& os, const Pixel& p);

	// multiplication with a ceiling of 255
	Pixel operator*(const double& d) {
		Pixel a;
		a.X = (X * d > 255) ? 255 : X * d;
		a.X = (X * d < 0) ? 0 : X * d;
		a.Y = (Y * d > 255) ? 255 : Y * d;
		a.Y = (Y * d < 0) ? 0 : Y * d;
		a.Z = (Z * d > 255) ? 255 : Z * d;
		a.Z = (Z * d < 0) ? 0 : Z * d;
		return a;
	}

	Pixel operator+(const Pixel& p) {
		return Pixel(min((unsigned)255, X + p.X),min((unsigned)255, Y + p.Z),min((unsigned)255, Z + p.Z));
	}

	unsigned X = 0;
	unsigned Y = 0;
	unsigned Z = 0;
};
std::ostream& operator<<(std::ostream& os, const Pixel& p) {
	os << p.X << ' ' << p.Y << ' ' << p.Z;
	return os;
}

std::ostream& operator<<(std::ostream& o, const Vec3& v) {
	o << v.I << ',' << v.J << ','  << v.K;
	return o;
}

struct Material {
	Material() = default;
	Material(Pixel& d, double& s, array<double, 3>& al): diffuse_color(d), specular_exponent(s), albedo(al) {}
	Material(Pixel& p): diffuse_color(p) {}

	Pixel diffuse_color = Pixel(255, 255, 255);
	double specular_exponent = 50;
	array<double, 3> albedo = {1, 0, 0};
};

class Shape {
public:
	Shape() = default;
	Shape(Pixel& p, double& s, array<double, 2>& al): M(p, s, al) {}
	Shape(Pixel& p): M(p) {}

	virtual bool RayIntersect(const Vec3& orig, const Vec3& dir, double& t0) const {return false;}// check if a ray intersects with our shape

	Pixel GetColor() const {return M.diffuse_color;}
	Material GetMaterial() const {return M;}

	virtual Vec3 GetNormal(const Vec3& point) const {
		return Vec3(0,0,0);
	}

	bool CastRay(const Vec3 & orig, const Vec3& dir, const Shape& sh) const;
	bool CastRay(const Vec3 & orig, const Vec3& dir, const Shape& sh, double& shape_dist) const;
private:
	Material M = Material();
};

bool Shape::CastRay(const Vec3& orig, const Vec3& dir, const Shape& sh) const {
	double shape_dist = std::numeric_limits<double>::max();
	return (sh.RayIntersect(orig, dir, shape_dist));
}

class Sphere : public Shape {
  Vec3 center = Vec3(0, 0, 0);
  double radius = 1;

public:
	Sphere() = default;
	Sphere(const Vec3& c, double r): center(c), radius(r), Shape() {}
	Sphere(const Vec3& c, double r, Pixel& p, double d ,array<double, 2> a): center(c), radius(r), Shape(p, d, a) {}
	Sphere(const Vec3& c, double r, Pixel& p): center(c), radius(r), Shape(p) {}

	Vec3 GetCenter() const {return center;}
	virtual Vec3 GetNormal(const Vec3& point) const;

    virtual bool RayIntersect(const Vec3 &orig, const Vec3 &dir, double &t0) const;
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

Vec3 Sphere::GetNormal(const Vec3& point) const {
	// check if the given point is on the sphere
	// if (pow((point.I - center.I), 2) + pow((point.J - center.J), 2) + pow((point.K - center.K), 2) != pow(radius, 2)) throw std::invalid_argument("Sphere::GetNormal -> point not present on the sphere");

	Vec3 normal = point - center;
	normal.N();

	return normal;
}

class Line: public Shape {
	Vec3 orig = Vec3(0,0,0);
	Vec3 dir = Vec3(1,1,-1);

public:
	Line() = default;
	Line(const Vec3& o, const Vec3& d): orig(o), dir(d), Shape() {
		if (dir.I == 0 && dir.J == 0 && dir.K == 0) throw std::invalid_argument("Line::Line() -> dir cannot be 0");
	}
	Line(const Vec3& o, const Vec3& d, Pixel& p, double& s, array<double, 2>& al): orig(o), dir(d), Shape(p, s, al) {
		if (dir.I == 0 && dir.J == 0 && dir.K == 0) throw std::invalid_argument("Line::Line() -> dir cannot be 0");
    }


	bool RayIntersect(const Vec3& orig, const Vec3& dir, double& t0) const {
		return false;
	}
};

struct Light {
  Light() = default;
  Light(const Vec3& p, const double& i) : pos(p), intensity(i) {}

  Vec3 pos = Vec3(0, 0, 0);
  double intensity = 0.5;
};

Vec3 Reflect(Vec3& I, Vec3& N) {
	return I - N * (double)2 * (I * N);
}

bool EnvironmentIntersect(const Vec3 &orig, const Vec3 &dir, const vector<Shape*>& objects, double& shape_dist, Vec3& point, Vec3& point_n, Material& obj_mat) {
	double min_dist = std::numeric_limits<double>::max();
	for (Shape *s : objects) {
		double dist = std::numeric_limits<double>::max();
		if (s->RayIntersect(orig, dir, dist) && dist < min_dist) {
			min_dist = dist;
			point = orig + dir * dist;
			/* if (dynamic_cast<Sphere*>(s) != nullptr) {
				std::cout << "Is a sphere\n";
			} */
			point_n = s->GetNormal(point);
			// std::cout << "Normal: " << point_n << '\n';
			obj_mat = s->GetMaterial();
		}
	}
	shape_dist = min_dist;
	return min_dist < std::numeric_limits<double>::max();
}

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

	string file_path = "test/img/5";
	vector<vector<Pixel>> sample_img(height, vector<Pixel>(width, Pixel(0,0,0)));
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			sample_img[i][j] = bg_col; // base color
		}
	}

	vector<Vec3> positions = {Vec3(-5, 3, -20), Vec3(0, 0, -1000), Vec3(-100, 35, -1050), Vec3(-100, -100, -1100), Vec3(0,0,-15)};
	vector<Pixel> colors = {Pixel(33, 189, 3), Pixel(67, 0, 89), Pixel(255, 51, 153)};
	map<string, Pixel> color_map = {
		{"Tech_Green", Pixel(33, 189, 3)}, 
		{"Purple", Pixel(67, 0, 89)},
		{"Pink", Pixel(255, 51, 153)},
		{"Orange", Pixel(237, 128, 2)},
		{"Yellow", Pixel(255, 247, 0)},
		{"Dark Blue", Pixel(55, 0, 255)},
		{"Red", Pixel(200, 0, 0)},
		{"Sky Blue", Pixel(4, 158, 209)},
		{"White", Pixel(255, 255, 255)},
		{"Pearl", Pixel(180, 190, 240)},
		{"Dark Red", Pixel(80, 0, 0)}
	};
	map<string, array<double, 2>> material_map = {
		{"aluminium-matte", {55,75}},
		{"aluminium-polished", {65,75}},
		{"aluminium-shiny", {80,87}},
		{"chrome-polished", {60,70}},
		{"concrete-rough", {20,30}}
	};

	vector<Shape*> objects;
	vector<Light> lights;
	// Sphere *a = new Sphere(positions[0], 1, color_map.at("Yellow"));
	// Sphere *b = new Sphere(positions[1], 250, color_map.at("Sky Blue"));
	// Sphere *c = new Sphere(positions[2], 300, color_map.at("White"));

	Sphere *f = new Sphere(Vec3(-3,    1.5,   -16), 2, color_map.at("Pearl"));
	Sphere *g = new Sphere(Vec3(-1.0, -1.5, -12), 2, color_map.at("Dark Red"), (double) 10, {0.9, 0.1});
	Sphere *h = new Sphere(Vec3(-1.0, 3, -12), 2, color_map.at("Dark Red"), (double) 10, {0.9, 0.1});

    // objects.push_back(a);
	// objects.push_back(b);
	// objects.push_back(c);
	// objects.push_back(d);
	// objects.push_back(e);
	objects.push_back(f);
	objects.push_back(g);

	// lights.push_back(Light(Vec3(-5, 4, 0), 1.3));
	// lights.push_back(Light(Vec3(5, -14, -3), 0.8));
	lights.push_back(Light(Vec3(-1,-5,-10), 1.3));
	lights.push_back(Light(Vec3(-1,20,-10), 1.3));

    double fov = atan((double) width / 2);

	for (unsigned i = 0; i < height; ++i) {
		for (unsigned j = 0; j < width; ++j) {
			double x = (2 * (i + 0.5) / (double)width - 1) * tan(fov / 2.) * width / (double)height;
			double y = -(2 * (j + 0.5) / (double)height - 1) * tan(fov / 2.);
			Vec3 dir = Vec3(x, y, -1);
			dir.N(); // normalize

			// parameters of the object being hit
			Vec3 coord;
			Vec3 coord_n; // normalised
			double min_dist = std::numeric_limits<double>::max();
			Material mat;

			if (EnvironmentIntersect(orig, dir, objects, min_dist, coord, coord_n, mat)) {
				double diffuse_light_intensity = 0, specular_light_intensity = 0;
				for (Light& l: lights) {
					Vec3 light_dir = (l.pos - coord);
					light_dir.N();

					diffuse_light_intensity += l.intensity * max((double)0, light_dir * coord_n);
					specular_light_intensity += pow(max((double) 0, Reflect(light_dir, coord_n)*dir), mat.specular_exponent)* l.intensity;
				}
				sample_img[i][j] = mat.diffuse_color * diffuse_light_intensity * mat.albedo[0] + Pixel(Vec3(1,1,1) * specular_light_intensity * mat.albedo[1]);
			}
		}
	}

	Render::WritePPI(sample_img, file_path);

	std::cout << "Successfully Finished\n";
}