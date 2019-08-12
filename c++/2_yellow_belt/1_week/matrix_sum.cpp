#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <vector>

using namespace std;


bool check(int x, int edge) {
    if (x < 0 || x >= edge) {
        throw out_of_range(to_string(x));
    }
    return true;
}

class Matrix {
public:
    Matrix() = default;

    Matrix(int num_rows, int num_cols) {
        if (num_rows < 0 || num_cols < 0) {
            throw out_of_range("Negative digits" + to_string(num_rows) + " " + to_string(num_cols));
        }
        n_rows = num_rows;
        n_cols = num_cols;
        matr.resize(n_rows, vector<int>(n_cols));
    }

    void Reset(int num_rows, int num_cols) {
        matr.clear();
        *this = Matrix(num_rows, num_cols);
    }

    int GetNumRows() const {
        return n_rows;
    }

    int GetNumColumns() const {
        return n_cols;
    }

    int At(int num_row, int num_col) const {
        check(num_row, n_rows);
        check(num_col, n_cols);
        return matr[num_row][num_col];
    }

    int &At(int num_row, int num_col) {
        check(num_row, n_rows);
        check(num_col, n_cols);
        return matr[num_row][num_col];
    }

private:
    int n_rows{0};
    int n_cols{0};
    vector<vector<int>> matr;
};

istream &operator>>(istream &s, Matrix &rhs) {
    int n_r{0};
    int n_c{0};
    s >> n_r >> n_c;
    rhs.Reset(n_r, n_c);
    for (int i = 0; i < rhs.GetNumRows(); ++i) {
        for (int j = 0; j < rhs.GetNumColumns(); ++j) {
            s >> rhs.At(i, j);
        }
    }
    return s;
}

ostream &operator<<(ostream &out, const Matrix &rhs) {
    int n_r = rhs.GetNumRows();
    int n_c = rhs.GetNumColumns();
    out << n_r << " " << n_c << endl;
    for (int i = 0; i < n_r; ++i) {
        for (int j = 0; j < n_c; ++j) {
            out << rhs.At(i, j);
            if (j + 1 == n_c){
                out << endl;
            } else {
                out << " ";
            }
        }
    }
    return out;
}

Matrix operator+(const Matrix &a, const Matrix &b) {
    if (a.GetNumRows() != b.GetNumRows() || a.GetNumColumns() != b.GetNumColumns()) {
        throw invalid_argument("");
    }
    Matrix c(a.GetNumRows(), a.GetNumColumns());
    for (int i = 0; i < c.GetNumRows(); ++i) {
        for (int j = 0; j < c.GetNumColumns(); ++j) {
            c.At(i, j) = a.At(i, j) + b.At(i, j);
        }
    }
    return c;
}

bool operator==(const Matrix &lhs, const Matrix &rhs) {
    if (lhs.GetNumRows() == rhs.GetNumRows() &&
        lhs.GetNumColumns() == rhs.GetNumColumns()) {
        for (int i = 0; i < lhs.GetNumRows(); ++i) {
            for (int j = 0; j < lhs.GetNumColumns(); ++j) {
                if (lhs.At(i, j) != rhs.At(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}
