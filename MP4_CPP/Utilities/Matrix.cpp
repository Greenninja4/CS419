#include "Matrix.h"
#include <iostream>
#include <math.h>

// Big 6
Matrix::Matrix(void){
    for (int x = 0; x < 4; x++){
        for (int y = 0; y < 4; y++){
            if (x == y){
                m[x][y] = 1.0;
            } else {
                m[x][y] = 0.0;
            }
            // ADDED FOR HOMOGENOUS
            if (y == 4){
                m[x][y] = 1.0;
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
Matrix Matrix::operator/ (const double& d){
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

void Matrix::set_to_rot(float theta, string xyz){
    if (xyz == "x"){
        m[0][0] = 1;
        m[0][1] = 0;
        m[0][2] = 0;
        m[0][3] = 0;

        m[1][0] = 0;
        m[1][1] = cos(theta);
        m[1][2] = -sin(theta);
        m[1][3] = 0;

        m[2][0] = 1;
        m[2][1] = sin(theta);
        m[2][2] = cos(theta);
        m[2][3] = 0;

        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;
    } else if (xyz == "y"){
        m[0][0] = cos(theta);
        m[0][1] = 0;
        m[0][2] = sin(theta);
        m[0][3] = 0;

        m[1][0] = 0;
        m[1][1] = 1;
        m[1][2] = 0;
        m[1][3] = 0;

        m[2][0] = -sin(theta);
        m[2][1] = 0;
        m[2][2] = cos(theta);
        m[2][3] = 0;

        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;

    } else if (xyz == "z"){
        m[0][0] = cos(theta);
        m[0][1] = -sin(theta);
        m[0][2] = 0;
        m[0][3] = 0;

        m[1][0] = sin(theta);
        m[1][1] = cos(theta);
        m[1][2] = 0;
        m[1][3] = 0;

        m[2][0] = 0;
        m[2][1] = 0;
        m[2][2] = 1;
        m[2][3] = 0;

        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;

    } else {
        cout << "Bad rotation matrix" << endl;
    }
    
}

void Matrix::set_to_rot_inv(float theta, string xyz){
    if (xyz == "x"){
        m[0][0] = 1;
        m[0][1] = 0;
        m[0][2] = 0;
        m[0][3] = 0;

        m[1][0] = 0;
        m[1][1] = cos(theta);
        m[1][2] = sin(theta);
        m[1][3] = 0;

        m[2][0] = 1;
        m[2][1] = -sin(theta);
        m[2][2] = cos(theta);
        m[2][3] = 0;

        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;
    } else if (xyz == "y"){
        m[0][0] = cos(theta);
        m[0][1] = 0;
        m[0][2] = -sin(theta);
        m[0][3] = 0;

        m[1][0] = 0;
        m[1][1] = 1;
        m[1][2] = 0;
        m[1][3] = 0;

        m[2][0] = sin(theta);
        m[2][1] = 0;
        m[2][2] = cos(theta);
        m[2][3] = 0;

        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;

    } else if (xyz == "z"){
        m[0][0] = cos(theta);
        m[0][1] = sin(theta);
        m[0][2] = 0;
        m[0][3] = 0;

        m[1][0] = -sin(theta);
        m[1][1] = cos(theta);
        m[1][2] = 0;
        m[1][3] = 0;

        m[2][0] = 0;
        m[2][1] = 0;
        m[2][2] = 1;
        m[2][3] = 0;

        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;

    } else {
        cout << "Bad rotation matrix" << endl;
    }
    
}

void Matrix::set_to_scale(float scale){
        m[0][0] = scale;
        m[0][1] = 0;
        m[0][2] = 0;
        m[0][3] = 0;

        m[1][0] = 0;
        m[1][1] = scale;
        m[1][2] = 0;
        m[1][3] = 0;

        m[2][0] = 0;
        m[2][1] = 0;
        m[2][2] = scale;
        m[2][3] = 0;

        m[3][0] = 0;
        m[3][1] = 0;
        m[3][2] = 0;
        m[3][3] = 1;
}