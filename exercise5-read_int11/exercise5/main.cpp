//
//  main.cpp
//  exercise5
//
//  Created by Hoàng Minh Trị on 6/15/20.
//  Copyright © 2020 Hoang Minh Tri. All rights reserved.
//

#include <iostream>
#include <fstream>

using namespace std;

class bitreader{
private:
    istream& is_;
    uint8_t buffer_;
    uint8_t n_ = 0;
    
    uint32_t read_bit(){
        if(n_ == 0){
            is_.read(reinterpret_cast<char*>(&buffer_), sizeof(buffer_));
            n_ = 8;
        }
        return (buffer_ >> --n_) & 1;
    }
    
public:
    bitreader(istream& is) : is_(is){}
    ~bitreader(){
        flush();
    }
    
    istream& operator()(uint32_t& u, uint8_t n){
        u = 0;
        while (n --> 0) {
            u = (u << 1) | read_bit();
        }
        return is_;
    }
    
    void flush(uint32_t u = 0) {
        while (n_ > 0) {
            read_bit();
        }
    }
};

int main(int argc, const char * argv[]) {
    
    cout << "Start...\n";
    
    char input[40] = { "file.bin11"};
    char output[40] = { "output.txt"};
    
    ifstream is(input, ios::binary);
    if(!is){
        return EXIT_FAILURE;
    }
    
    ofstream os(output);
    if(!os){
        return EXIT_FAILURE;
    }
    
    
    bitreader br(is);
    uint32_t val;
    while (br(val, 11)){
        int32_t sval = val;
        if (sval & 0x400){
            sval -= 0x800;
        }
        os << sval << "\n";
    }
    
    cout << "Stop...\n";
    
    return EXIT_SUCCESS;
    
}

// 0F 00 8F CF 6A 00 AE 3B 02 A8
