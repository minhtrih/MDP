//
//  main.cpp
//  exercise4
//
//  Created by Hoàng Minh Trị on 6/15/20.
//  Copyright © 2020 Hoang Minh Tri. All rights reserved.
//

#include <iostream>
#include <fstream>

using namespace std;

class bitwriter {
private:
    ostream& os_;
    uint8_t buffer_;
    uint8_t n_ = 0;
    
    void write_bit (uint32_t u){
        buffer_ = buffer_ << 1 | (u & 1);
        if(++n_ == 8){
            os_.write(reinterpret_cast<char*>(&buffer_), sizeof(buffer_));
            n_ = 0;
        }
    }
    
public:
    bitwriter(ostream& os) : os_(os){}
    ~bitwriter(){
        flush();
    }
    void operator()(uint32_t u, uint8_t n){
        while (n --> 0) {
            write_bit(u >> n);
        }
    }
    
    void flush(uint32_t u = 0) {
        while (n_ > 0) {
            write_bit(u);
        }
    }
};

int main(int argc, const char * argv[]) {
    
    cout << "Start...\n";
    
    char input[40] = { "file01.txt"};
    char output[40] = { "output.bin11"};
    
    ifstream is(input, ios::binary);
    if(!is){
        return EXIT_FAILURE;
    }
    
    ofstream os(output);
    if(!os){
        return EXIT_FAILURE;
    }
    
    bitwriter bw(os);
    
    
    int32_t val;
    while (is >> val){
        bw(val, 11);
    }
    
    cout << "Stop...\n";
    
    return EXIT_SUCCESS;
    
}
