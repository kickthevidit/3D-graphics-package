#ifndef OBJECTS_
#define OBJECTS_

#include <iostream>

struct Pos {
	Pos(unsigned X, unsigned Y, unsigned Z): x(X), y(Y), z(Z) {}

	unsigned x; // between 0 and 1
	unsigned y;
	unsigned z;

	operator-()
};

struct Dir {
	Dir(unsigned X, unsigned Y, unsigned Z): x(X), y(Y), z(Z) {}

	float x;
	float y;
	float z;
};

struct Object {
	virtual bool RayIntersect(Pos& origin, Dir& direction) = 0;
};

struct Sphere: public Object {
	Sphere(unsigned X, unsigned Y, unsigned Z, float r): center_(X,Y,Z), radius_(r) {};

	bool RayIntersect(Pos& origin, Dir& direction) const;

	Pos center_;
	float radius_;
};

#endif