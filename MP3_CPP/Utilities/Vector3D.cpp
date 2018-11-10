#include "Vector3D.h"
#include <math.h>

// Big 6
Vector3D::Vector3D(void): 
    x(0.0), 
    y(0.0), 
    z(0.0) {}
Vector3D::Vector3D(const double& x, const double& y, const double& z): 
    x(x), 
    y(y), 
    z(z) {}
Vector3D::Vector3D(const Vector3D& v): 
    x(v.x), 
    y(v.y), 
    z(v.z) {}
Vector3D::Vector3D(const double& xyz): 
    x(xyz), 
    y(xyz), 
    z(xyz) {}
Vector3D& Vector3D::operator= (const Vector3D& rhs){
    if (this == &rhs){
        return *this;
    }
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
	return *this;
}
Vector3D::~Vector3D (void){}

// Operators
Vector3D Vector3D::operator- (void) const{
    return Vector3D(-x, -y, -z);
}
Vector3D Vector3D::operator* (const double& a) const{
    return Vector3D(x * a, y * a, z * a);
}
Vector3D Vector3D::operator/ (const double& a) const{
    return Vector3D(x / a, y / a, z / a);
}
double Vector3D::operator* (const Vector3D& v) const{
    return (x * v.x + y * v.y + z * v.z);
}
Vector3D Vector3D::operator^ (const Vector3D& v) const{
    return Vector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}
Vector3D Vector3D::operator+ (const Vector3D& v) const{
    return Vector3D(x + v.x, y + v.y, z + v.z);
}
Vector3D Vector3D::operator- (const Vector3D& v) const{
    return Vector3D(x - v.x, y - v.y, z - v.z);
}
Vector3D Vector3D::operator% (const Vector3D& v) const{
    return Vector3D(x * v.x, y * v.y, z * v.z);
}

// Functions
double Vector3D::length(void){
	return sqrt(x * x + y * y + z * z);
}
double Vector3D::distance(const Vector3D& v) const{
    return  sqrt( (x - v.x)*(x - v.x)
                + (y - v.y)*(y - v.y)
                + (z - v.z)*(z - v.z) );
}
void Vector3D::normalize(void){
	double length = sqrt(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
}
Vector3D& Vector3D::hat(void){
    double length = sqrt(x * x + y * y + z * z);
	x /= length; y /= length; z /= length;
	return *this;
}

// Non-Member Functions
Vector3D operator* (const double& a, const Vector3D& v){
    return Vector3D(a * v.x, a * v.y, a * v.z);
}
Vector3D operator* (const Matrix& mat, const Vector3D& v){
    return Vector3D( mat.m[0][0] * v.x + mat.m[0][1] * v.y + mat.m[0][2] * v.z + mat.m[0][3],
                     mat.m[1][0] * v.x + mat.m[1][1] * v.y + mat.m[1][2] * v.z + mat.m[1][3],
                     mat.m[2][0] * v.x + mat.m[2][1] * v.y + mat.m[2][2] * v.z + mat.m[2][3] );
}
Vector3D operator^ (const Matrix& mat, const Vector3D& v){
    return Vector3D( mat.m[0][0] * v.x + mat.m[0][1] * v.y + mat.m[0][2] * v.z,
                     mat.m[1][0] * v.x + mat.m[1][1] * v.y + mat.m[1][2] * v.z,
                     mat.m[2][0] * v.x + mat.m[2][1] * v.y + mat.m[2][2] * v.z );
}
Vector3D operator% (const Matrix& mat, const Vector3D& v){
    return Vector3D( mat.m[0][0] * v.x + mat.m[1][0] * v.y + mat.m[2][0] * v.z,
                     mat.m[0][1] * v.x + mat.m[1][1] * v.y + mat.m[2][1] * v.z,
                     mat.m[0][2] * v.x + mat.m[1][2] * v.y + mat.m[2][2] * v.z );
}