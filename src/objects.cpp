#include "objects.h"

template <typename T> 
Vector3<T> Vector3<T>::operator+(const Vector3 &rhs) const {
	/* Addition operator for Vector3<T> */
  Vector3<T> temp;
  temp.x = this->x + rhs.x;
  temp.y = this->y + rhs.y;
  temp.z = this->z + rhs.z;
  return temp;
}

template <typename T>
Vector3<T> Vector3<T>::operator-(const Vector3 &rhs) const {
  /* subtraction operator for Vector3<T> */
  Vector3<T> temp;
  temp.x = this->x - rhs.x;
  temp.y = this->y - rhs.y;
  temp.z = this->z - rhs.z;
  return temp;
}

template <typename T>
Vector3<T> operator* (const float& a, const Vector3<T>& b) {
	/* multiplication operator for Vector3<T> */
	return Vector3<T>(a * b.x, a * b.y, a * b.z);
}

bool Sphere::RayIntersect(const Pos & origin, const Dir & direction, const float& dist) const {
	/* Method that tests if the object collides with a ray given direction and magnitude */
	Vector3<float> L = (center_ - origin).vec;
	float tca = L * direction;
	float 
}