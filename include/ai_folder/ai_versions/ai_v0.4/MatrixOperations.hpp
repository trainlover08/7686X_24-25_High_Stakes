#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>

using Matrix = std::vector<std::vector<double>>;

Matrix multiply(const Matrix& A, const Matrix& B) {
    int m = A.size();
    int n = A[0].size();
    int p = B[0].size();
    
    std::cout << "m = " << m << "n = " << n << "p = " << p << '\n';

    if (B.size() != n) {
        throw std::invalid_argument("Matrix dimentions do not match for multiplication");
    }
    
    Matrix result(m , std::vector<double>(p, 0));
        
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < p; ++j) {
                for (int k = 0; k < n; ++k) {
                    result[i][j] += A[i][k] * B[k][j];
                }    
            }    
        }
    
    return result;
}

Matrix add(const Matrix& A, const Matrix&B) {
    int m = A.size();
    int n = A[0].size();
    
    if (B.size() != m || B[0].size() != n) {
        throw std::invalid_argument("Matrix dimentions do not match for addition");
    }
    
    Matrix result(m, std::vector<double>(n, 0));
    
    for (int j = 0; j < m; ++j) {
        for (int k = 0; k < n; ++k) {
            result[j][k] = A[j][k] + B[j][k];
        }    
    }
    
    return result;
}

Matrix transpose(const Matrix& A) {
    int m = A.size();          // Number of rows in A
    int n = A[0].size();       // Number of columns in A
    
    Matrix result(n, std::vector<double>(m, 0.0)); // Transposed matrix of size (n x m)
    
    for (int j = 0; j < m; ++j) {  // Iterate over rows of A
        for (int k = 0; k < n; ++k) { // Iterate over columns of A
            result[k][j] = A[j][k]; // Set the transposed value
        }    
    }
    
    return result;
}

Matrix addScalar(const Matrix& A, double scalar) {
    int m = A.size();
    int n = A[0].size();
    
    Matrix result(m, std::vector<double>(n, 0));
    
    for (int j = 0; j < m; ++j) {
        for (int k = 0; k < n; ++k) {
            result[j][k] = A[j][k] + scalar;
        }    
    }
    
    return result;
}

Matrix elementWiseMultiply(const Matrix& A, const Matrix& B) {
    int m = A.size();
    int n = A[0].size();
    
    if (B.size() != m || B[0].size() != n) {
        throw std::invalid_argument("Matrix dimentions do not match for element-wise-multiplication");    
    }
    
    Matrix result(m, std::vector<double>(n, 0));
    
    for (int j = 0; j < m; ++j) {
        for (int k = 0; k < n; ++n) {
            result[j][k] = A[j][k] * B[j][k];
        }    
    }
    
    return result;
}

// Element-wise matrix division (A / B)
Matrix elementWiseDivide(const Matrix& A, const Matrix& B) {
    int m = A.size();
    int n = A[0].size();
    
    if (B.size() != m || B[0].size() != n) {
        throw std::invalid_argument("Matrix dimensions do not match for element-wise division");
    }
    
    Matrix result(m, std::vector<double>(n, 0));
    
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (B[i][j] == 0) {
                throw std::domain_error("Division by zero in element-wise division");
            }
            result[i][j] = A[i][j] / B[i][j];
        }
    }
    
    return result;
}

// Element-wise square root (sqrt(A))
Matrix elementWiseSqrt(const Matrix& A) {
    int m = A.size();
    int n = A[0].size();
    
    Matrix result(m, std::vector<double>(n, 0));
    
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (A[i][j] < 0) {
                throw std::domain_error("Negative value encountered in element-wise square root");
            }
            result[i][j] = std::sqrt(A[i][j]);
        }
    }
    
    return result;
}

// Subtract matrix B from matrix A (A - B)
Matrix subtract(const Matrix& A, const Matrix& B) {
    int m = A.size();
    int n = A[0].size();
    
    if (B.size() != m || B[0].size() != n) {
        throw std::invalid_argument("Matrix dimensions do not match for subtraction");
    }
    
    Matrix result(m, std::vector<double>(n, 0));
    
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i][j] = A[i][j] - B[i][j];
        }
    }
    
    return result;
}

void printMatrix(const Matrix& mat) {
    for (const auto& row : mat) {
        for (const auto& elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}