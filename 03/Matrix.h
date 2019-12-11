#pragma once

class Matrix
{
private:
    int* matrix = nullptr;
    size_t rows;
    size_t cols;
    size_t N;

    class Vector {
    private:
        size_t len;
        int* vector = nullptr;

    public:
        Vector(size_t n, int* vec) : len(n), vector(vec) {}

        const int operator[](const size_t y) const {
            if ((y < 0) || (y >= len)) {
                throw std::out_of_range("");
            }
            return vector[y];
        }

        int& operator[](const size_t y) {
            if ((y < 0) || (y >= len)) {
                throw std::out_of_range("");
            }
            return vector[y];
        }
    };

public:
    Matrix() : rows(0), cols(0), N(0) {}

    Matrix(size_t x, size_t y) : rows(x), cols(y), N(x * y) {
        matrix = new int[N];
    }

    ~Matrix() {
        delete[] matrix;
    }

    Matrix(const Matrix& m)
        : rows(m.rows), cols(m.cols), N(m.N)
    {
        int* tmp = new int[m.N];
        if (matrix != nullptr)
            delete[] matrix;
        matrix = tmp;
        std::copy(m.matrix, m.matrix + N, matrix);
    }

    Matrix& operator=(const Matrix& m) {
        if (this == &m)
            return *this;
        rows = m.rows;
        cols = m.cols;
        N = m.N;
        int* tmp = new int[m.N];

        if (matrix != nullptr)
            delete[] matrix;
        matrix = tmp;
        std::copy(m.matrix, m.matrix + N, matrix);
        return *this;
    }

    Vector operator[](const size_t x) {
        if ((x < 0) || (x >= rows)) {
            throw std::out_of_range("");
        }
        return Vector(cols, matrix + x * cols);
    }

    const Vector operator[](const size_t x) const {
        if ((x < 0) || (x >= rows)) {
            throw std::out_of_range("");
        }
        return Vector(cols, matrix + x * cols);
    }

    size_t getRows() const {return rows;}

    size_t getColumns() const {return cols;}

    Matrix &operator*=(const int x) {
        for (size_t i = 0; i < N; i++) {
            matrix[i] *= x;
        }
        return *this;
    };

    bool operator==(const Matrix &right) const {
        if (rows != right.getRows() || cols != right.getColumns())
            return false;
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                if ((*this)[i][j] != right[i][j])
                    return false;
            }
        }
        return true;
    }
    
    bool operator!=(const Matrix &right) const {return !(this == &right);}
};