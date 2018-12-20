#ifndef __MATRIX__
#define __MATRIX__

#include <string>
using namespace std;

class Matrix{
    public:
        // Variables
        double m[4][4]; // Elements

        // Big 6
        Matrix(void);
        Matrix(const Matrix& matrix);
        Matrix& operator= (const Matrix& rhs);
        ~Matrix(void);

        // Operators
        Matrix operator* (const Matrix& matrix) const;  // Matrix multiplication
        Matrix operator/ (const double& d);              // Scalar division

        // Functions
        void set_to_identity(void); // Set it to the identity matrix
        void set_to_rot(float theta, string xyz);
        void set_to_rot_inv(float theta, string xyz);
        void set_to_scale(float scale);
};

#endif