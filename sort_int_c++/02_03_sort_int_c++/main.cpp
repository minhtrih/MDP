//
//  main.cpp
//  02_03_sort_int_c++
//
//  Created by Hoàng Minh Trị on 6/13/20.
//  Copyright © 2020 Hoang Minh Tri. All rights reserved.
//

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstdbool>
#include <cstdint>
#include <cassert>
#include <cstring>

#include <utility>
#include <algorithm>
#include <fstream>
#include <iomanip>


namespace mdp {

template<typename T>
class vector {
    size_t n_; // number of elements
    size_t c_; // vector capacity
    T* data_;  // pointer to data (owned)

    // comparison function (warning: this is not a method)
    static int compare(const void *a, const void *b)
    {
        const T* pa = (const T*)a;
        const T* pb = (const T*)b;

        if (*pb < *pa) {
            return 1;
        }
        else if (*pa < *pb) {
            return -1;
        }
        else {
            return 0;
        }
    }

public:
    // constructor with parameters (optional)
    vector(size_t size = 0) : n_(size), c_(size > 0 ? size : 1) {
        data_ = new T[c_];
        assert(data_ != nullptr);
    }
    // copy constructor
    vector(const vector& rhs) : vector(rhs.size()) { // delegating constructor
        std::copy(rhs.data_, rhs.data_ + rhs.n_, data_);
    }
    // move constructor (r-value reference)
    vector(vector&& rhs) : n_(rhs.n_), c_(rhs.c_), data_(rhs.data_) {
        rhs.data_ = nullptr;
    }
    // swap function (warning: this is not a method)
    friend void swap(vector& a, vector& b)
    {
        using std::swap; // enables ADL
        swap(a.n_, b.n_);
        swap(a.c_, b.c_);
        swap(a.data_, b.data_);
    }
    // assignment operator (copy/move and swap idiom)
    vector& operator=(vector rhs) {
        swap(*this, rhs);
        return *this;
    }
    // destructor
    ~vector() {
        delete[] data_;
    }
    // adds an element at the end of the vector (adjusts the size)
    // If you're passing an object to a function always use a const reference,
    // apart from the case in which you are making a copy, in which case is
    // better to copy the parameter and move the parameter.
    void push_back(T elem) { // this parameter is used for copying
        if (n_ == c_) {
            c_ = c_ * 2;
            T* tmp = new T[c_];
            assert(tmp != NULL);
            std::copy(data_, data_ + n_, tmp);
            delete[] data_;
            data_ = tmp;
        }
        data_[n_++] = std::move(elem);
    }
    // inplace sorting of the vector
    void sort() {
        std::qsort(data_, n_, sizeof(T), compare);
    }
    size_t size() const {
        return n_;
    }
    T& operator[](size_t pos) {
        assert(pos < n_);
        return data_[pos];
    }
    const T& operator[](size_t pos) const {
        assert(pos < n_);
        return data_[pos];
    }
};

};

template <typename T>
bool read(const char *filename, mdp::vector<T>& v)
{
    std::ifstream is(filename);
    if (!is) {
        return false;
    }

    while (true) {
        T val;
        // Read
        is >> val;
        // Check
        if (!is) {
            break;
        }
        // Use
        v.push_back(val);
    }

    return true;
}

template <typename T>
bool write(const char *filename, const mdp::vector<T>& v)
{
    //FILE *fout = fopen(filename, "w");
    std::ofstream os(filename);
    if (!os) {
        return false;
    }
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i] << "\n";
    }
    return true;
}

struct point {
    double x, y;

    bool operator<(const point& rhs) const {
        if (y < rhs.y) {
            return true;
        }
        else if (y > rhs.y) {
            return false;
        }
        else {
            return x < rhs.x;
        }
    }
};
std::istream& operator>>(std::istream& is, point& rhs)
{
    return is >> rhs.x >> rhs.y;
}
std::ostream& operator<<(std::ostream& os, const point& rhs)
{
    return os << rhs.x << "\t" << rhs.y;
}

int main(int argc, char *argv[])
{
    char input[4][40] = { "file01.txt", "file02.txt", "file03.txt", "file04.txt"};
    char output[4][40] = { "output01.txt", "output02.txt", "output03.txt", "output04.txt"};
    
    using namespace std;
    using namespace mdp;

    mdp::vector<point> arr;
    if (!read(input[3], arr)) {
        return EXIT_FAILURE;
    }

    arr.sort();

    if (!write(output[3], arr)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
