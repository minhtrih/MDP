//
//  main.c
//  drawings_c
//
//  Created by Hoàng Minh Trị on 6/13/20.
//  Copyright © 2020 Hoang Minh Tri. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "canvas.h"

#define v(x) (void*)(x)

typedef void *(*virtual_fn_t)(struct shape *this, ...);

typedef struct shape {
    int x_, y_;
    char style_;
    char *name_; // owned

    // Virtual functions
    virtual_fn_t *vtbl_;
} shape;
void shape_draw(const shape *this, canvas *c) {
    canvas_set(c, this->x_, this->y_, this->style_);
}

shape *shape_destroy(shape *this) {
    free(this->name_);
    return this;
}

virtual_fn_t shape_vtbl_[2] = { shape_draw, shape_destroy };

shape *shape_create(shape *this, int x, int y, char style, const char *name) {
    this->x_ = x;
    this->y_ = y;
    this->style_ = style;
    this->name_ = strcpy(malloc(strlen(name)+1), name); // need to take ownership so we copy
    this->vtbl_ = shape_vtbl_;
    return this;
}

shape *new_shape(int x, int y, char style, const char *name) {
    return shape_create(malloc(sizeof(shape)), x, y, style, name);
}
void delete_shape(shape *this) {
    free(this->vtbl_[1](this));
}
const char *shape_name(const shape *this) {
    return this->name_;
}


typedef struct square {
    shape base_;
    int side_;
    int *p_; // Just for the sake of making an example
} square;

void square_draw(const square *this, canvas *c) { // overrides shape_draw
    canvas_rectangle(c, this->base_.x_, this->base_.y_,
        this->base_.x_ + this->side_ - 1, this->base_.y_ + this->side_ - 1,
        this->base_.style_);
}

square *square_destroy(square *this) {
    shape_destroy(v(this));
    free(this->p_);
    return this;
}

virtual_fn_t square_vtbl_[2] = { square_draw, square_destroy };

square *square_create(square *this, int x, int y, int side, char style, const char *name) {
    shape_create(v(this), x, y, style, name);
    this->base_.vtbl_ = square_vtbl_;
    this->side_ = side;
    this->p_ = malloc(sizeof(int));
    return this;
}

square *new_square(int x, int y, int side, char style, const char *name) {
    return square_create(malloc(sizeof(square)), x, y, side, style, name);
}
void delete_square(square *this) {
    delete_shape(v(this));
}

typedef struct circle {
    shape base_;
    int radius_;
} circle;

void circle_draw(const circle *this, canvas *c) { // overrides shape_draw
    canvas_circle(c, this->base_.x_, this->base_.y_, this->radius_, this->base_.style_);
}
circle *circle_destroy(circle *this) {
    shape_destroy(v(this));
    return this;
}

virtual_fn_t circle_vtbl_[2] = { circle_draw, circle_destroy };
circle *circle_create(circle *this, int x, int y, int radius, char style, const char *name) {
    shape_create(v(this), x, y, style, name);
    this->base_.vtbl_ = circle_vtbl_;
    this->radius_ = radius;
    return this;
}

circle *new_circle(int x, int y, int radius, char style, const char *name) {
    return circle_create(malloc(sizeof(circle)), x, y, radius, style, name);
}
void delete_circle(circle *this) {
    delete_shape(v(this));
}

typedef struct shape_manager {
    int n_; // number of shapes
    shape **shapes_; // shapes are owned by the manager
} shape_manager;
shape_manager *shape_manager_create(shape_manager *this, int n) {
    this->n_ = n;
    this->shapes_ = malloc(n * sizeof(shape*));
    return this;
}
shape_manager *shape_manager_destroy(shape_manager *this) {
    for (int i = 0; i < this->n_; ++i) {
        delete_shape(this->shapes_[i]);
    }
    free(this->shapes_);
    return this;
}
shape_manager *new_shape_manager(int n) {
    return shape_manager_create(malloc(sizeof(shape_manager)), n);
}
void delete_shape_manager(shape_manager *this) {
    free(shape_manager_destroy(this));
}
void shape_manager_print_names(const shape_manager *this, FILE *f) {
    for (int i = 0; i < this->n_; ++i) {
        fprintf(f,"%s\n", shape_name(this->shapes_[i]));
    }
}
void shape_manager_draw(const shape_manager *this, canvas *c) {
    for (int i = 0; i < this->n_; ++i) {
        this->shapes_[i]->vtbl_[0](this->shapes_[i], c);
    }
}


int main(void)
{
    canvas *c = new_canvas(80, 25);
    canvas_rectangle(c, 0, 0, 79, 24, '*');

    shape_manager *sm = new_shape_manager(4);

    /* Use shapes */
    sm->shapes_[0] = new_shape(5, 20, 'x', "first");
    sm->shapes_[1] = new_shape(50, 10, 'y', "second");
    /* Use square */
    sm->shapes_[2] = v(new_square(30, 15, 6, '$', "third"));
    /* Use circle */
    sm->shapes_[3] = v(new_circle(6, 6, 5, '*', "fourth"));

    shape_manager_draw(sm, c);

    canvas_out(c, stdout);

    shape_manager_print_names(sm, stdout);

    delete_shape_manager(sm);
    delete_canvas(c);
        
    return EXIT_SUCCESS;
}
