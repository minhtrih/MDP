//
//  main.cpp
//  drawings_c++
//
//  Created by Hoàng Minh Trị on 6/13/20.
//  Copyright © 2020 Hoang Minh Tri. All rights reserved.
//

#include <iostream>
#include <string>
#include <ostream>
#include <fstream>

#include "canvas.h"

using namespace std;
// Shape
struct shape {
    int x_, y_;
    char style_;
    string name_; // owned

    shape(int x, int y, char style, const string& name)
        : x_(x), y_(y), style_(style), name_(name){}
    virtual ~shape() = default;
    const string& name() const {
        return name_;
    }
    virtual void draw(canvas& c) const {
        c.set(x_, y_, style_);
    }
};


struct square : public shape {
    int side_;
    int *p_; // Just for the sake of making an example

    square(int x, int y, int side, char style, const string& name)
        : shape(x, y, style, name), side_(side), p_(new int) {}
    ~square() {
        delete p_;
    }
    void draw(canvas& c) const override {
        c.rectangle(x_, y_, x_ + side_ - 1, y_ + side_ - 1, style_);
    }
};

struct circle : public shape {
    int radius_;

    circle(int x, int y, int radius, char style, const string& name)
        : shape(x, y, style, name), radius_(radius) {}
    void draw(canvas& c) const override {
        c.circle(x_, y_, radius_, style_);
    }
};

struct shape_manager {
    int n_; // number of shapes
    shape **shapes_; // shapes are owned by the manager

    shape_manager(int n) : n_(n), shapes_(new shape*[n]) {}

    ~shape_manager() {
        for (int i = 0; i < n_; ++i) {
            delete shapes_[i];
        }
        delete[] shapes_;
    }
    void print_names(ostream& os) const {
        for (int i = 0; i < n_; ++i) {
            os << shapes_[i]->name() << "\n";
        }
    }
    void draw(canvas& c) const {
        for (int i = 0; i < n_; ++i) {
            shapes_[i]->draw(c);
        }
    }

    shape*& operator[](int pos) { return shapes_[pos]; }
};


int main(void)
{
    {
        ofstream os("output.txt");
        canvas c(80, 25);
        c.rectangle(0, 0, 79, 24, '*');

        shape_manager sm(4);

        /* Use shapes */
        sm[0] = new shape(5, 20, 'x', "first");
        sm[1] = new shape(50, 10, 'y', "second");
        /* Use square */
        sm[2] = new square(30, 15, 6, '$', "third");
        /* Use circle */
        sm[3] = new circle(6, 6, 5, '*', "fourth");

        sm.draw(c);

        c.out(cout);

        sm.print_names(cout);
    }
    return EXIT_SUCCESS;
}
