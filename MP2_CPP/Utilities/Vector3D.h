#ifndef __VECTOR3D__
#define __VECTOR3D__

class Vector3D{
    public:
        // Members
        double x, y, z;

        // Constructors
        // default, regular, copy, destructor
        Vector3D(void);
		Vector3D(double x, double y, double z);
		Vector3D(const Vector3D& v);
		~Vector3D (void);

        // Operators 
        // assignment, negation, scalar multiplication, scalar division
		Vector3D& operator= (const Vector3D& rhs);
		Vector3D operator- (void) const;
		Vector3D operator* (const double a) const;
        Vector3D operator/ (const double a) const;
        // dot product, cross product, vector addition, vector subtraction, element-wise multiplication
        double operator* (const Vector3D& v) const;
		Vector3D operator^ (const Vector3D& v) const;
		Vector3D operator+ (const Vector3D& v) const;
		Vector3D operator- (const Vector3D& v) const;
        Vector3D operator% (const Vector3D& v) const;

        // Member Functions
        // get length of vector, normalizes vector, get distance between vectors
        double length(void);
        void normalize(void);
        double distance(const Vector3D& v);
        Vector3D& hat(void);

};

// Non-member functions
Vector3D operator* (const double a, const Vector3D& v);

#endif