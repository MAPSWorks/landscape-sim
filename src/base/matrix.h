#pragma once
#include <vector>
#include <stdexcept>
#include <string>
#include <sstream> 
#include "log.h"

// Wraps 1d vector to get 2d matrix semantics
// Row-major storage
namespace base {
template<typename T>
class Matrix {
public:
    // Matrix is initialized, memory is allocated of the given size
    // and values are zero-initialized
    Matrix(size_t rows, size_t cols);
    // Matrix is initialized with given value
    Matrix(size_t rows, size_t cols, T init_value);
    // Get costant reference for given indexes.
    // Throws if bounds are incorrect.
    const T& operator()(size_t row, size_t column) const;
    size_t GetRows() const;
    size_t GetCols() const;
    // Log contents
    void Log() const;
private:
    const size_t rows_;
    const size_t columns_;
    // 2d matrix is stored as a vector and index combination
    // is used to access elements as if it is 2d array.
    // Elements are stored row-by-row
    const std::vector<T> matrix_;
};

template <typename T>
inline Matrix<T>::Matrix(size_t rows, size_t cols) :
    rows_(rows),
    columns_(cols),
    matrix_(rows_* columns_)
{}

template<typename T>
inline Matrix<T>::Matrix(size_t rows, size_t cols, T init_value) :
    rows_(rows),
    columns_(cols),
    matrix_(rows_* columns_, init_value)
{}

// 2d semantics are simulated by acessing elemnt in 1d vector
// by the combination of indexes = row + column_count * colum
template<typename T>
inline const T& Matrix<T>::operator()(size_t row, size_t column) const {
    // Bound check
    if (row > rows_ || column > columns_) {
        throw std::invalid_argument("Base: matrix access index(es) are out of bounds - ("+std::to_string(row) +","+
            std::to_string(column) + ")");
    }
    return matrix_.at(row + columns_ * column);
}

template<typename T>
inline size_t Matrix<T>::GetRows() const {
    return rows_;
}

template<typename T>
inline size_t Matrix<T>::GetCols() const {
    return columns_;
}

template<typename T>
inline void Matrix<T>::Log() const {
    std::ostringstream matrix_log_str;
    for (size_t i = 0; i < GetRows(); i++) {
        for (size_t j = 0; j < GetCols(); j++) {
            matrix_log_str << (*this)(i, j) << ' ';
        }
        matrix_log_str << "\n";
    }
    Log::Info("Base: matrix contents - \n", matrix_log_str.str());
}

};