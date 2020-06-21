//
//  main.cpp
//  exercise1
//
//  Created by Hoàng Minh Trị on 6/15/20.
//  Copyright © 2020 Hoang Minh Tri. All rights reserved.
//

#include <iostream>
#include <fstream>

using namespace std;

struct freq{
    uint32_t counters_[256] = { 0 };
    freq(){}
    void operator()(uint8_t val){
        ++counters_[val];
    }
};

int main(int argc, const char * argv[]) {
    
    char input[40] = { "small.txt"};
    char output[40] = { "output.txt"};
    
    ifstream is(input, ios::binary);
    if(!is){
        return EXIT_FAILURE;
    }
    
    freq f;
    while(true){
        uint8_t val;
        // Read
        is.read((char*)(&val), 1);
        // Check
        if (!is){
            break;
        }
        // Use
        f(val);
    }
    
    ofstream os(output);
    if(!os){
        return EXIT_FAILURE;
    }
    for (size_t i =0; i<256; i++){
        if(f.counters_[i] > 0){
            os << hex << i << '\t' << f.counters_[i] << '\n';
        }
    }
    
    
    return EXIT_SUCCESS;
    
}
