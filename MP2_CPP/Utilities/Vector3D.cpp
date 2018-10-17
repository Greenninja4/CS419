#include <cmath>
#include "Vector3D.h"

#include <iostream>

// Constructors (default, regular, copy, destructor)
Vector3D::Vector3D(void): x(0.0), y(0.0), z(0.0){}
Vector3D::Vector3D(double x, double y, double z): x(x), y(y), z(z){}
Vector3D::Vector3D(const Vector3D& v): x(v.x), y(v.y), z(v.z){}
Vector3D::~Vector3D (void){}

// Operators
// assignment, negation, scalar multiplication, scalar division
Vector3D& Vector3D::operator= (const Vector3D& rhs){
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
	return *this;
}
Vector3D Vector3D::operator- (void) const{
    return (Vector3D(-x, -y, -z));
}
Vector3D Vector3D::operator* (const double a) const{
    return (Vector3D(x * a, y * a, z * a));
}
Vector3D Vector3D::operator/ (const double a) const{
    return (Vector3D(x / a, y / a, z / a));
}
// dot product, cross product, vector addition, vector subtraction
double Vector3D::operator* (const Vector3D& v) const{
    return (x * v.x + y * v.y + z * v.z);
}
Vector3D Vector3D::operator^ (const Vector3D& v) const{
    return (Vector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x));
}
Vector3D Vector3D::operator+ (const Vector3D& v) const{
    return (Vector3D(x + v.x, y + v.y, z + v.z));
}
Vector3D Vector3D::operator- (const Vector3D& v) const{
    return (Vector3D(x - v.x, y - v.y, z - v.z));
}
Vector3D Vector3D::operator% (const Vector3D& v) const{
    return Vector3D(x * v.x, y * v.y, z * v.z);
}

// Members
// get length of vector, normalizes vector, get distance between vectors
double Vector3D::length(void){
	return (sqrt(x * x + y * y + z * z));
}
void Vector3D::normalize(void){	
	double length = sqrt(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
}
double Vector3D::distance(const Vector3D& v){
    return (sqrt( (x - v.x)*(x - v.x)
                + (y - v.y)*(y - v.y)
                + (z - v.z)*(z - v.z) ));
}
Vector3D& Vector3D::hat(void){
    double length = sqrt(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
	return *this;
}

// Non members
// multiply by double on left
Vector3D operator* (const double a, const Vector3D& v){
    return (Vector3D(a * v.x, a * v.y, a * v.z));
}