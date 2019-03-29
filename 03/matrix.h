#include <cstdlib>
#include <vector>
#include <cassert>
#include <ostream>
#include <iostream>

class row
{
    std::vector<int> contents;

  public:
    row(size_t col_numb) : contents(col_numb) {}
    row(const row &other) : contents(other.contents) {}

    const int &operator[](const size_t i) const
    {
        if (i >= contents.size())
        {
            throw std::out_of_range("");
        }
        return contents[i];
    }
    int &operator[](const size_t i)
    {
        if (i >= contents.size())
        {
            throw std::out_of_range("");
        }
        return contents[i];
    }
    size_t getLen() const
    {
        return contents.size();
    }
    bool operator!=(const row &other) const
    {
        return contents != other.contents;
    }
    void operator*=(int mul)
    {
        for (int i = 0; i < contents.size(); ++i)
        {
            contents[i] *= mul;
        }
    }
    ~row() {}
};

class Matrix
{
    size_t heigth;
    std::vector<row> mtx;

  public:
    Matrix(size_t rows, size_t colums) : heigth(rows), mtx(heigth, row(colums)) {}
    Matrix(const Matrix &other) : heigth(other.heigth), mtx(other.mtx) {}

    size_t getRows() const
    {
        return heigth;
    }
    size_t getColumns() const
    {
        return mtx[0].getLen();
    }
    const row &operator[](const size_t i) const
    {
        if (i >= heigth)
        {
            throw std::out_of_range("");
        }
        return mtx[i];
    }
    row &operator[](const size_t i)
    {
        if (i >= heigth)
        {
            throw std::out_of_range("");
        }
        return mtx[i];
    }
    bool operator==(const Matrix &other) const
    {
        if (this == &other)
        {
            return true;
        }
        if (this->heigth != other.heigth)
        {
            return false;
        }
        for (int i = 0; i < heigth; i++)
        {
            if (mtx[i] != other[i])
            {
                return false;
            }
        }
        return true;
    }
    Matrix &operator*=(int mul)
    {
        for (int i = 0; i < heigth; ++i)
        {
            mtx[i] *= mul;
        }
        return *this;
    }
    bool operator!=(const Matrix& other) const {
        return !((*this) == other);
    }
};