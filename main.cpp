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
	~Vec3() = default;
	
	Vec3& operator=(const Vec3& copy) {
		if (&copy == this) return *this;

		I = copy.I;
		J = copy.J;
		K = copy.K;
		return *this;
	}

	bool operator==(const Vec3& vec) const {return (I == vec.I && J == vec.J && K == vec.K);}

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


	double M() const {
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

	friend Vec3 operator/(const Vec3& v, double d) {
		Vec3 r;
		r.I = v.I / d;
		r.J = v.J / d;
		r.K = v.K / d;
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
	Material(Pixel& d, double s, Vec3 al): diffuse_color(d), specular_exponent(s), albedo(al) {}
	Material(Pixel& p): diffuse_color(p) {}

	Pixel diffuse_color = Pixel(255, 255, 255);
	double specular_exponent = 8;
	Vec3 albedo = Vec3{1, 1, 1}; // diffuse color, specular color, reflective color
};

class Shape {
public:
	Shape() = default;
	Shape(Pixel& p, double s, Vec3& al): M(p, s, al) {}
	Shape(Pixel& p): M(p) {}
	Shape(Material& m): M(m) {}

	virtual bool RayIntersect(const Vec3& orig, const Vec3& dir, double& t0) const {return false;}// check if a ray intersects with our shape

	Pixel GetColor() const {return M.diffuse_color;}
	Material GetMaterial() const {return M;}

	virtual Vec3 GetNormal(const Vec3& point) const {return Vec3(0,0,0);}

private:
	Material M = Material();
};

class Sphere : public Shape {
  Vec3 center = Vec3(0, 0, 0);
  double radius = 1;

public:
	Sphere() = default;
	Sphere(const Vec3& c, double r): center(c), radius(r), Shape() {if (radius < 0) radius*=-1;}
	Sphere(const Vec3& c, double r, Pixel& p, double d ,Vec3 al): center(c), radius(r), Shape(p, d, al) {if (radius < 0) radius*=-1;}
	Sphere(const Vec3 &c, double r, Pixel &p): center(c), radius(r), Shape(p) {if (radius < 0) radius *= -1;}
	Sphere(const Vec3 &c, double r, Material &m): center(c), radius(r), Shape(m) {if (radius < 0)radius *= -1;}

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

// TODO: finish plane, add bounds to turn it into a rectangle

class Checkerboard: public Shape {
	Pixel color_2;
	Vec3 normal;
	double d; // distance of origin to plane
	Vec3 side; // dir vec showing board orientation, going from one corner to another
	Vec3 centre;
public:
	Checkerboard(Vec3 n, double d_, Pixel p, Vec3 cen, Vec3 s , Material& m): normal(n), d(d_), color_2(p), Shape(m), centre(cen), side(s) {}

	bool RayIntersect(const Vec3& orig, const Vec3& dir, double& t0) const;
	virtual Vec3 GetNormal(const Vec3& point) const {return normal;}
};

bool Checkerboard::RayIntersect(const Vec3& orig, const Vec3& dir, double& t0) const {
	// halfway done
	double Vd = normal * dir ;
	if (Vd == 0) return false; // ray is parallel to plane}
	double v0 = (-(normal * orig + d));
	t0 = v0 / Vd;
	if (t0 < 0) return false; // intersects behind camera
	Vec3 cont_pt = orig + dir * t0;
	if ((((centre - cont_pt) * side) / pow(side.M(), 2)) > (side / 2).M()) return false;
	return true;
}

class Line: public Shape{
	Vec3 pos;
	Vec3 dir_;

public:
	Line(Vec3 o, Vec3 d, Material m): pos(o), dir_(d), Shape(m){}

	bool RayIntersect(const Vec3& orig, const Vec3& dir, double& t0) const;
	virtual Vec3 GetNormal(const Vec3& point) const {return dir_;}
};

bool Line::RayIntersect(const Vec3& orig, const Vec3& dir, double& t0) const {
	if (dir == dir_) return false; // parallel lines

	double denom = dir.J - (dir.I * dir_.J) / dir_.I;
	if (denom == 0) return false;

	t0 = ((pos - orig).J - (pos - orig).I*(dir_.J / dir_.I)) / denom;
	if (t0 < 0) return false;
	return true;
}

struct Light {
  Light() = default;
  Light(const Vec3& p, const double& i) : pos(p), intensity(i) {}

  Vec3 pos = Vec3(0, 0, 0);
  double intensity = 0.5;
};

Vec3 Reflect(const Vec3& I, const Vec3& N) {
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
			if (point_n == Vec3(0,0,0)) std::cout << point << ',' << point_n << '\n';
			// std::cout << "Normal: " << point_n << '\n';
			obj_mat = s->GetMaterial();
		}
	}
	shape_dist = min_dist;

	// double checkerboard_dist = std::numeric_limits<double>::max();
	// if (fabs(dir.J) > 1e-3) {
	// 	double d  = -(orig.J + 5) / dir.J;
	// 	Vec3 pt = orig + dir*d;
	// 	if (d > 0 && fabs(pt.I) < 10 && pt.K > -30 && d < min_dist) {
	// 		checkerboard_dist = d;
	// 		point = pt;
	// 		point_n = Vec3(0,1,0);
	// 		obj_mat.diffuse_color = (int(.5 * point.I + 1000) + int(.5 * point.K)) & 1 ? Vec3(1, 1, 1) : Vec3(1, .7, .3);
	// 		obj_mat.diffuse_color = obj_mat.diffuse_color * .3;
    //     }
	// }

	double checkerboard_dist = std::numeric_limits<double>::max();
	if (fabs(dir.I) > 1e-3) {
		double d  = -(orig.I - 0.5) / dir.I;
		Vec3 pt = orig + dir*d;
		if (d > 0 && fabs(pt.J) < 10 && pt.K > -30 && d < min_dist) {
			checkerboard_dist = d;
			point = pt;
			point_n = Vec3(1,0,0);
			obj_mat.diffuse_color = (int(.5 * point.J + 1000) + int(.5 * point.K)) & 1 ? Vec3(1, 1, 1) * 255 : Vec3(1, .7, .3) * 255;
			obj_mat.diffuse_color = obj_mat.diffuse_color * .3;
			obj_mat.albedo = Vec3(0.9, 0.1, 0.);
        }
	}

	return min(min_dist, checkerboard_dist) < std::numeric_limits<double>::max();
	// return min_dist < std::numeric_limits<double>::max()
;}

Pixel CastRay(const Vec3& orig, const Vec3& dir, const vector<Shape*>& objects, const vector<Light>& lights, Pixel& bg_col, unsigned depth = 0) {
	Vec3 coord, coord_n;
	Material mat;
	double min_dist = std::numeric_limits<double>::max();

	if (depth > 4 || !EnvironmentIntersect(orig, dir, objects, min_dist, coord, coord_n, mat)) return bg_col;

	Vec3 reflect_dir = Reflect(dir, coord_n);
	Vec3 reflect_orig = reflect_dir * coord_n < 0 ? coord - coord_n * 1e-3 : coord + coord_n * 1e-3; // shift the orig to prevent obstruction by object
	Pixel reflect_color = CastRay(reflect_orig, reflect_dir, objects, lights, bg_col, depth + 1);

	double diffuse_light_intensity = 0, specular_light_intensity = 0;
	for (const Light& l: lights) {
		Vec3 light_dir = (l.pos - coord);
		light_dir.N(); // convert to unit vector
		double light_distance = (l.pos - coord).M();
		double shadow_dist;

		// shadows processing
		Vec3 shadow_orig = light_dir * coord_n < 0 ? coord - coord_n * 1e-3 : coord + coord_n * 1e-3; // shift point a bit to prevent collision
		Vec3 shadow_coord, shadow_n;
		Material tmp_mat;
		if ((EnvironmentIntersect(shadow_orig, light_dir, objects, shadow_dist, shadow_coord, shadow_n, tmp_mat)) && ((shadow_coord - shadow_orig).M() < light_distance)) continue;

		diffuse_light_intensity += l.intensity * max((double) 0, light_dir * coord_n);
		// if (diffuse_light_intensity == 0) std::cout << l.intensity << ',' << light_dir * coord_n << '\n'; 
		specular_light_intensity += pow(max((double)0, Reflect(light_dir, coord_n)*dir), mat.specular_exponent) * l.intensity;
	}

	// std::cout << "Final Color: " << (mat.diffuse_color * diffuse_light_intensity * mat.albedo.I + Pixel(Vec3(1,1,1)) * specular_light_intensity * mat.albedo.J + reflect_color * mat.albedo.K) << '\n';

	Pixel out = mat.diffuse_color * diffuse_light_intensity * mat.albedo.I + Pixel(Vec3(1,1,1)) * specular_light_intensity * mat.albedo.J + reflect_color * mat.albedo.K;

	/* if (out.X == 0 && out.Y == 0 && out.Z == 0) {
		std::cout << "Final Color: " << (mat.diffuse_color * diffuse_light_intensity * mat.albedo.I + Pixel(Vec3(1,1,1)) * specular_light_intensity * mat.albedo.J + reflect_color * mat.albedo.K) << '\t';
		std::cout << mat.diffuse_color << ',' << diffuse_light_intensity << ',' << mat.albedo.I << '\t';
		std::cout << Pixel(Vec3(1,1,1)) * specular_light_intensity << ',' << mat.albedo.J << '\t';
		std::cout << reflect_color << ',' << mat.albedo.K << '\n';
    } */
	
	return out;
}

namespace Render {
	void WritePPI(vector<vector<Pixel>>& pixels, string& file_path) {

		if (file_path.substr(file_path.size()-4, 4) != ".ppm") file_path+=".ppm";

		std::ofstream ofs;
		ofs.open(file_path);
		ofs << "P3\n" << pixels.at(0).size() << " " << pixels.size() << "\n255\n";
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
	constexpr unsigned height = 768; // bug where if canvas is not square it doesn't work
	const Vec3 orig(0,0,0);
    Pixel bg_col = Pixel(73,178,203);

	string file_path = "test/img/11";
	vector<vector<Pixel>> sample_img(height, vector<Pixel>(width, Pixel(0,0,0)));

	map<string, Pixel> cmap = {
		{"Tech_Green", Pixel(33, 189, 3)}, 
		{"Purple", Pixel(67, 0, 89)},
		{"Pink", Pixel(255, 51, 153)},
		{"orange", Pixel(237, 128, 2)},
		{"yellow", Pixel(255, 247, 0)},
		{"blue", Pixel(55, 0, 255)},
		{"red", Pixel(200, 0, 0)},
		{"blue1", Pixel(4, 158, 209)},
		{"white", Pixel(255, 255, 255)},
		{"pearl", Pixel(180, 190, 240)},
		{"red1", Pixel(80, 0, 0)},
		{"black", Pixel(0,0,0)},
		{"green", Pixel(108,183,53)},
		{"glass_green", Pixel(209,234,197)}
	};
	vector<Shape*> objects;
	vector<Light> lights;

	Material ivory(cmap.at("white"), 50., Vec3{0.6, 0.3, 0.1});
	Material pearl(cmap.at("pearl"), 45., Vec3{0.6, 0.3, 0.2});
	Material red_rubber(cmap.at("red1"), 10., Vec3(0.9, 0.1, 0.0));
	Material mirror(cmap.at("black"), 1425., Vec3(1, 1, 1));
	Material plane_material(cmap.at("white"), 0, Vec3(1, 0.0, 0.0));
	Material line_material(cmap.at("white"), 12451251., Vec3(1, 1, 0));

	
	// Sphere *a = new Sphere(Vec3(-3,    0,   -16), 1.2, ivory);
	Sphere *b = new Sphere(Vec3(-2, -0.5, -18), 2.3, red_rubber);
	Sphere *d = new Sphere({-1, -1.5, -12}, 1.05, pearl);
	// Sphere *e = new Sphere({-2, -1, -30}, 4, mirror);
	// Checkerboard *f = new Checkerboard({-6, 0, 0}, 6, cmap.at("orange"), {0,0,0}, {0, 100, 0}, plane_material);

	vector<Line*> lines;
	for (int i = 1; i < 2; ++i) {
		// lines.push_back(new Line({(double)i, 0, 0}, {0,0,(double)i}, line_material));
		lines.push_back(new Line({0, 0,(double) i}, {(double) i,0,0}, line_material));
	}

	// objects.push_back(a);
	objects.push_back(b);
	// objects.push_back(c);
	objects.push_back(d);
	// objects.push_back(e);
	// objects.push_back(f);
	// for(Line* l: lines) objects.push_back(l);

	lights.push_back(Light(Vec3(-20,20,20), 1.5));
	lights.push_back(Light(Vec3(30,50,-25), 1.8));
	lights.push_back(Light(Vec3(30,20, 30), 1.7));

    double fov = M_PI_2 / 3;

	for (unsigned i = 0; i < height; ++i) {
		for (unsigned j = 0; j < width; ++j) {
			double x = (2 * (i + 0.5) / (double)width - 1) * tan(fov / 2.) * width / (double)height;
			double y = -(2 * (j + 0.5) / (double)height - 1) * tan(fov / 2.);
			Vec3 dir = Vec3(x, y, -1);
			dir.N(); // normalize

			sample_img[i][j] = CastRay(orig, dir, objects, lights, bg_col);
		}
	}

	Render::WritePPI(sample_img, file_path);

	std::cout << "Successfully Finished\n";
}