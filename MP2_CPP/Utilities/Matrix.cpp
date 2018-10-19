#include "Matrix.h"

// Big 6
Matrix::Matrix(void){
    for (int x = 0; x < 4; x++){
        for (int y = 0; y < 4; y++){
            if (x == y){
                m[x][y] = 1.0;
            } else {
                m[x][y] = 0.0;
            }
        }
    }
}
Matrix::Matrix(const Matrix& mat){
    for (int x = 0; x < 4; x++){
        for (int y = 0; y < 4; y++){
            m[x][y] = mat.m[x][y];
        }
    }
}
Matrix& Matrix::operator= (const Matrix& rhs){
    if (this == &rhs){
        return *this;
    }
    for (int x = 0; x < 4; x++){
        for (int y = 0; y < 4; y++){
            m[x][y] = rhs.m[x][y];
        }
    }
    return *this;
}
Matrix::~Matrix(void){}

// Operators
Matrix Matrix::operator* (const Matrix& mat) const{
    Matrix product;
    for (int y = 0; y < 4; y++){
        for (int x = 0; x < 4; x++){
            double sum = 0.0;
            for (int j = 0; j < 4; j++){
                sum += m[x][j] * mat.m[j][y];
            }
            product.m[x][y] = sum;
        }
    }
    return product;
}
Matrix Matrix::operator/ (double d){
    for (int x = 0; x < 4; x++){
        for (int y = 0; y < 4; y++){
            m[x][y] = m[x][y] / d;
        }
    }
    return *this;
}

// Functions
void Matrix::set_to_identity(void){
    for (int x = 0; x < 4; x++){
        for (int y = 0; y < 4; y++){
            if (x == y){
                m[x][y] = 1.0;
            } else {
                m[x][y] = 0.0;
            }
        }
    }
}