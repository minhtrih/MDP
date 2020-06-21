//
//  main.cpp
//  exercise2
//
//  Created by Hoàng Minh Trị on 6/15/20.
//  Copyright © 2020 Hoang Minh Tri. All rights reserved.
//

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, const char * argv[]) {
    
    using clock = chrono::steady_clock;
    cout << "Starting...\n";
    auto start_time = clock::now();
    
    char input[40] = { "small.txt"};
    char output[40] = { "output.txt"};
    
    ifstream is(input, ios::binary);
    if(!is){
        return EXIT_FAILURE;
    }
    
    ofstream os(output, ios::binary);
    if(!os){
        return EXIT_FAILURE;
    }
    
//    int32_t val;
//    while (is >> val){
//        os.write(reinterpret_cast<const char*>(&val), 4);
//    }
    
    while (true){
        int32_t val;
        is >> val;
        if(!is){
            break;
        }
        os.write(reinterpret_cast<const char*>(&val), 4);
    }
    
    auto stop_time = clock::now();
    cout << "Stop.\n";
    auto diff = stop_time - start_time;
    cout << "Elapsed: " << diff.count() << "\n";
    
    return EXIT_SUCCESS;
    
}
