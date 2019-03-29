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
            // throw}
            return contents[i];
        }
    }
    int &operator[](const size_t i)
    {
        if (i >= contents.size())
        {
            // throw
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
            // throw
        }
        return mtx[i];
    }
    row &operator[](const size_t i)
    {
        if (i >= heigth)
        {
            // throw
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

};

void
show(const Matrix &mtx)
{
    for (int i = 0; i < mtx.getRows(); ++i)
    {
        std::cout << "[ ";
        for (int j = 0; j < mtx.getColumns(); ++j)
        {
            std::cout << mtx[i][j] << " ";
        }
        std::cout << "]\n";
    }
}

int main()
{
    const size_t rows = 5;
    const size_t cols = 3;

    Matrix m(rows, cols);

    assert(m.getRows() == 5);
    assert(m.getColumns() == 3);

    m[1][2] = 5; // строка 1, колонка 2
    double x = m[4][1];

    m *= 3; // умножение на число
    std::cout << "\n";

    auto m2 = m *= 5;

    show(m);

    Matrix m1(rows, cols);
    return 0;
}
// if (m1 == m)
// {
// }
