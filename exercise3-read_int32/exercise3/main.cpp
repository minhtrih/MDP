//
//  main.cpp
//  exercise3
//
//  Created by Hoàng Minh Trị on 6/15/20.
//  Copyright © 2020 Hoang Minh Tri. All rights reserved.
//

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, const char * argv[]) {
    
    char input[40] = { "output.bin"};
    char output[40] = { "output.txt"};
    
    ifstream is(input, ios::binary);
    if(!is){
        return EXIT_FAILURE;
    }
    
    ofstream os(output);
    if(!os){
        return EXIT_FAILURE;
    }
    
//    int32_t val;
//    while (is >> val){
//        os.write(reinterpret_cast<const char*>(&val), 4);
//    }
    
    int32_t val;
    while (is.read(reinterpret_cast<char*>(&val), 4)) {
        os << val << "\n";
    }
    
//    while (true){
//        int32_t val;
//        is.read(reinterpret_cast<char*>(&val), 4);
//        if(!is){
//            break;
//        }
//        os << val << "\n";
//    }
    
    return EXIT_SUCCESS;
    
}
