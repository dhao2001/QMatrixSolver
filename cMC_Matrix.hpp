#ifndef CMC_MATRIX_H
#define CMC_MATRIX_H

#include "gmpxx.h"
#include <iostream>
#include <vector>

namespace cMatrixCalc
{
    class Matrix
    {
    protected:
        unsigned int _row;
        unsigned int _column;
        mpf_class *_mat;

        void swapMpf(mpf_class &, mpf_class &);
        void swapRow(unsigned int, unsigned int);
        void init(unsigned int, unsigned int);

    public:
        Matrix(unsigned int);
        Matrix(unsigned int, unsigned int);
        Matrix(double **array, unsigned int, unsigned int);
        Matrix(const Matrix &);
        ~Matrix();

        unsigned int row();
        unsigned int column();
        unsigned int getSuffix(unsigned int, unsigned int) const;
        static unsigned int getSuffix(unsigned int, unsigned int, unsigned int);
        const mpf_class &cat(unsigned int, unsigned int) const;
        mpf_class &at(unsigned int, unsigned int);
        int linsolve(std::vector<mpf_class> &);
        void norm(double &);
        void print();
    };

    Matrix::Matrix(unsigned int N)
    {
        this->init(N, N+1);
    }

    Matrix::Matrix(unsigned int row, unsigned int column)
    {
        this->init(row, column);
    }

    Matrix::Matrix(double **array, unsigned int row, unsigned int column)
    {
        this->init(row, column);
        for (unsigned int i = 0; i < row; i++)
        {
            for (unsigned int j = 0; j < column; j++)
            {
                this->_mat[this->getSuffix(i, j)] = *(*(array + (i * column)) + j);
            }
        }
    }

    Matrix::Matrix(const Matrix &m)
    {
        this->init(m._row, m._column);
        for (unsigned int i = 0; i < _row; i++)
        {
            for (unsigned int j = 0; j < _column; j++)
            {
                this->_mat[this->getSuffix(i, j)] = m.cat(i, j);
            }
        }
    }

    Matrix::~Matrix()
    {
        delete[] this->_mat;
    }

    //Constructor and Destructor

    
    void Matrix::init(unsigned int row, unsigned int column)
    {
        this->_row = row;
        this->_column = column;
        this->_mat = new mpf_class[row * column]();
    }

    void Matrix::swapMpf(mpf_class &a, mpf_class &b)
    {
        mpf_class tmp(a);
        a = b;
        b = tmp;
    }

    void Matrix::swapRow(unsigned int row_a, unsigned int row_b)
    {
        for (int c = 0; c < this->_column; c++)
        {
            swapMpf(this->at(row_a, c), this->at(row_b, c));
        }
    }

    //Private Methods

    unsigned int Matrix::row()
    {
        return this->_row;
    }

    unsigned int Matrix::column()
    {
        return this->_column;
    }

    unsigned int Matrix::getSuffix(unsigned int row, unsigned int column) const
    {
        return getSuffix(row, column, this->_column);
    }

    unsigned int Matrix::getSuffix(unsigned int row, unsigned int column, unsigned N)
    {
        return row * N + column;
    }

    const mpf_class &Matrix::cat(unsigned int row, unsigned int column) const
    {
        return _mat[getSuffix(row, column)];
    }

    mpf_class &Matrix::at(unsigned int row, unsigned int column)
    {
        return _mat[getSuffix(row, column)];
    }

    int Matrix::linsolve(std::vector<mpf_class> &solve_container)
    {
        double norm;
        Matrix tMat(*this);
        for (int curN = 0; curN < tMat.row(); curN++)
        {
            unsigned int maxRow = curN;
            mpf_class maxColumnEleme = abs(tMat.cat(curN, curN));

            for (int r = curN + 1; r < tMat.row(); r++)
            {
                if (maxColumnEleme < abs(tMat.cat(r, curN)))
                {
                    maxColumnEleme = abs(tMat.cat(r, curN));
                    maxRow = r;
                }
            }
            
            if (maxColumnEleme == 0)
            {
                return 0;
            }
            if (maxRow != curN)
            {
                tMat.swapRow(maxRow, curN);
            }

            for (unsigned int r = curN + 1; r < tMat.row(); r++)
            {
                mpf_class m = tMat.cat(r, curN) / tMat.cat(curN, curN);
                for (unsigned int c = curN; c < tMat.column(); c++)
                {
                    tMat.at(r, c) -= m * tMat.at(curN, c);
                }
            }

            mpf_class m = tMat.cat(curN, curN);
            for (unsigned int c = curN; c < tMat.column(); c++)
            {
                tMat.at(curN, c) /= m;
            }
        }
        
        for (unsigned int curN = tMat.row(); curN > 0; curN--)
        {
            for (unsigned int r = 0; r < curN - 1; r++)
            {
                mpf_class s = tMat.cat(r, curN - 1);
                tMat.at(r, curN - 1) -= s;
                tMat.at(r, tMat.row()) -= s * tMat.cat(curN - 1, tMat.row());
            }
        }

        solve_container.clear();
        for (unsigned int r = 0; r < tMat.row(); r++)
        {            
            solve_container.push_back(tMat.cat(r, tMat.column() - 1));
        }

        tMat.norm(norm);
        return tMat.row();
    }

    void Matrix::print()
    {
        const int printWidth = 12;

        using namespace std;

        for (int i = 0; i < _row; i++)
        {
            cout << "[";
            for (int j = 0; j < _column; j++)
            {
                cout << cout.width(printWidth);
                cout << this->cat(i, j);
            }
            cout << "]" << endl << endl;
        }
    }

    void Matrix::norm(double &norm)
    {

    }
    //Public Methods

} // namespace cMatrixCalc

#endif
