//
//  main.c
//  02_03_sort_int
//
//  Created by Hoàng Minh Trị on 6/12/20.
//  Copyright © 2020 Hoang Minh Tri. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

int compare_int32_t(const void *a, const void *b) {
    const int32_t *pa = a;
    const int32_t *pb = b;
    printf("a: %d\n", *(int*)a);
    printf("b: %d\n", *(int*)b);
    if(*pa > * pb) {
        return 1;
    } else if(*pa < *pb) {
        return -1;
    } else {
        return 0;
    }
}

typedef struct vector {
    size_t n;
    size_t c;
    int32_t *data;
} vector;

vector *vector_constructor(vector *this){
    this->n = 0;
    this->c = 1;
    this->data = malloc(sizeof(int32_t));
    assert(this->data != NULL);
    return this;
};

vector *vector_copy(vector *this, vector *rhs){
    if(this == rhs){
        return this;
    }
    
    this->n = rhs->n;
    this->c = rhs->c;
    free(this->data);
    this->data = malloc(this->c * sizeof(int32_t));
    assert(this->data != NULL);
    memcpy(this->data, rhs->data, rhs->n * sizeof(int32_t));
    return this;
}

vector *vector_destructor(vector *this){
    free(this->data);
    return this;
};

vector *new_vector(void){
    return vector_constructor(malloc(sizeof(vector)));
}

void delete_vector(vector *this){
    free(vector_destructor(this));
}

void vector_push_back(vector *this, int32_t i){
    ++this->n;
    if (this->n > this->c) {
        this->c = this->c * 2;
        this->data = realloc(this->data, this->c * sizeof(int32_t));
        assert(this->data != NULL);
    }
    this->data[this->n - 1] = i;
}

void vector_sort(vector *this){
    qsort(this->data, this->n, sizeof(int32_t), compare_int32_t);
}

size_t vector_size(vector *this){
    return this->n;
}

int32_t vector_at(vector *this, size_t pos){
    assert(pos < this->n);
    return this->data[pos];
}

void vector_set(vector *this, size_t pos, int32_t i){
    assert(pos < this->n);
    this->data[pos] = i;
}

bool leggi_interi(const char *filename,vector *v){
    FILE *fin = fopen(filename, "r");
    if (fin == NULL){
        return false;
    }
    
    while (true) {
        int32_t i;
        // Leggi
        int ret = fscanf(fin, "%d", &i);
        // Controlla
        if (ret != 1) {
            break;
        }
        // Usa
        vector_push_back(v, i);
    }
    fclose(fin);
    return true;
}

bool scrivi_interi(const char *filename, vector *v){
    FILE *fout = fopen(filename, "w");
    if (fout == NULL){
        return false;
    }
    
    for (size_t i=0; i< vector_size(v); i++){
        if(fprintf(fout, "%d\n", vector_at(v, i)) == 0 ) {
            return false;
        }
    }
    
    fclose(fout);
    return true;
}

int main(int argc, const char * argv[]) {
    char input[2][40] = { "file01.txt", "file02.txt"};
    char output[2][40] = { "output01.txt", "output02.txt"};
//    for (int a=0; a<2; a++) {
    
    vector arr;
    vector_constructor(&arr);
    
    if(!leggi_interi(input[0], &arr)){
        return EXIT_FAILURE;
    }

    vector_sort(&arr);
    
    vector arr2;
    vector_constructor(&arr2);
    vector_copy(&arr2, &arr);
    vector_set(&arr2, 0, 7);
    
    if(!scrivi_interi(output[0], &arr)){
        vector_destructor(&arr);
        return EXIT_FAILURE;
    }
    vector_destructor(&arr);
//    }
    return EXIT_SUCCESS;
    
}
