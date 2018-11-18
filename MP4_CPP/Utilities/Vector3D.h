#ifndef __VECTOR3D__
#define __VECTOR3D__

#include "Matrix.h"

class Vector3D{
    public:
        // Variables
        double x, y, z; // XYZ-Coordinates

        // Big 6
        Vector3D(void);
		Vector3D(const double& x, const double& y, const double& z);
        Vector3D(const double& xyz);
		Vector3D(const Vector3D& v);
        Vector3D& operator= (const Vector3D& rhs);
		~Vector3D (void);

        // Operators
		Vector3D operator- (void) const;                // Negation
		Vector3D operator* (const double& a) const;     // Scalar Multiplication
        Vector3D operator/ (const double& a) const;     // Scalar Division
        double operator* (const Vector3D& v) const;     // Dot Product
		Vector3D operator^ (const Vector3D& v) const;   // Cross Product
		Vector3D operator+ (const Vector3D& v) const;   // Vector Addition
		Vector3D operator- (const Vector3D& v) const;   // Vector Subtraction
        Vector3D operator% (const Vector3D& v) const;   // Element-wise Multiplication

        // Functions
        double length(void);                            // Length
        double distance(const Vector3D& v) const;       // Distance
        void normalize(void);                           // Normalizes vector
        Vector3D& hat(void);                            // Normalizes vector AND returns vector
};

// Non-member Functions
Vector3D operator* (const double& a, const Vector3D& v);        // Scalar Multiplication (scalar on left)
Vector3D operator* (const Matrix& mat, const Vector3D& v);      // Point Matrix Multiplication (Transpose, Homogenous 1) (matrix on left)
Vector3D operator^ (const Matrix& mat, const Vector3D& v);      // Direction Matrix Multiplication (Transpose, Homogenous 0) (matrix on left)
Vector3D operator% (const Matrix& mat, const Vector3D& v);      // Normal Matrix Multiplication (Transpose of Inverse, Homogenous 0) (matrix on left)

#endif