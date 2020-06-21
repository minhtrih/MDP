//
//  main.cpp
//  exercise8
//
//  Created by Hoàng Minh Trị on 6/16/20.
//  Copyright © 2020 Hoang Minh Tri. All rights reserved.
//

#include <iostream>
#include "comp_decomp.h"

void syntax()
{
    std::cerr <<
        "SYNTAX: \n"
        "\n"
        "binary [c|d] <input file> <output file>\n"
        "\n"
        "c encodes a text file containing integer base 10 numbers into BIN! format\n"
        "d decodes the BIN! format file\n";
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
//    char input[20] = { "example01.txt"};
//    char output[20] = { "freq01.bin"};
    char input[20] = { "freq01.bin"};
    char output[20] = { "decode01.txt"};
    
//    if (argc != 4) {
//        syntax();
//    }

    std::string command = "d";
    if (command == "c") {
        compress(input, output);
    }
    else if (command == "d") {
        decompress(input, output);
    }
    else {
        syntax();
    }

    return EXIT_SUCCESS;
}

