#ifndef OBJECTS_
#define OBJECTS_

#include <iostream>
#include <cmath>
#include <vector>
#include <limits>

template <typename T>
struct Vector3 {
	Vector3<T>(T& X, T& Y, T& Z): x(X), y(Y), z(Z) {}

	float M() const {return sqrt(x * x + y * y + z * z);}

	Vector3<T> operator+(const Vector3& rhs) const;
	Vector3<T> operator-(const Vector3& rhs) const;

    friend Vector3<float> operator*(const Vector3<float>& b, const float& a);

    T x;
	T y;
	T z;
};

struct Material {
	Material(const Vector3<float>& color): diffuse_color(color){}
	Material(): diffuse_color() {}

	Vector3<float> diffuse_color;
}

struct Pos {
  Pos(Vector3<float>& v) : vec(v.x, v.y, v.z) {}
  Pos(float X, float Y, float Z) : vec(X, Y, Z) {}

  Pos operator+(const Pos &rhs) const {
    Vector3<float> out = vec + rhs.vec; return Pos(out);
  }
  Pos operator-(const Pos &rhs) const {
    Vector3<float> out = vec - rhs.vec;
    return Pos(out);
  }

  friend Pos operator*(const float &a, const Vector3<float> &b) {
    Vector3<float> out = b * a;
    return Pos(out);
  }

  Vector3<float> vec;
};

struct Dir {
	Dir(Vector3<float>& v) : vec(v.x, v.y, v.z) {}
	Dir(float X, float Y, float Z) : vec(X, Y, Z) {}

	Dir operator+(const Dir &rhs) const {
    	Vector3<float> out = vec + rhs.vec; return Dir(out);
  	}
  	Dir operator-(const Dir &rhs) const {
		Vector3<float> out = vec - rhs.vec; return Dir(out);
  	}

	friend Dir operator*(const float &a, const Dir& b) {
	Vector3<float> out = b.vec * a;
	return Dir(out);
	}

	Vector3<float> vec;
};


struct Object {
	virtual bool RayIntersect(Pos& origin, Dir& direction) = 0;
};

struct Sphere: public Object {
	Sphere(float X, float Y, float Z, float r): center_(X,Y,Z), radius_(r), material_() {};
	Sphere(float X, float Y, float Z, float r, Material& m): center_(X,Y,Z), radius_(r), material_(m) {};

	bool RayIntersect(const Pos& origin, const Dir& direction, const float& dist) const;

	Pos center_;
	float radius_;
	Material material_;
};

#endif