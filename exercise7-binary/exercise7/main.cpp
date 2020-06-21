//
//  main.cpp
//  exercise7
//
//  Created by Hoàng Minh Trị on 6/15/20.
//  Copyright © 2020 Hoang Minh Tri. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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
void error(const std::string& s)
{
    std::cerr << s << "\n";
    exit(EXIT_FAILURE);
}

template <typename T>
std::ostream& raw_write(std::ostream& os, const T& val, size_t size = sizeof(T))
{
    return os.write(reinterpret_cast<const char*>(&val), size);
}

uint32_t numbits(int32_t min, int32_t max)
{
    uint32_t range = max - min + 1;
    uint32_t maxnum = range - 1;
    uint32_t n = 0;
    while (maxnum > 0) {
        maxnum >>= 1; // maxnum /= 2;
        ++n;
    }
    return n;
}

class bitwriter {
    std::ostream& os_;
    uint8_t buf_, n_ = 0;

    void writebit(uint32_t u)
    {
        buf_ = (buf_ << 1) | (u & 1);
        if (++n_ == 8) {
            raw_write(os_, buf_);
            n_ = 0;
        }
    }
public:
    bitwriter(std::ostream& os) : os_(os) {}
    ~bitwriter() { flush(); }

    std::ostream& operator()(uint32_t u, uint8_t n) {
        while (n --> 0) {
            writebit(u >> n);
        }
        return os_;
    }

    void flush(uint32_t u = 0) {
        while (n_ > 0) {
            writebit(u);
        }
    }
};

void compress(const std::string& input, const std::string& output)
{
    std::ifstream is(input);
    if (!is) {
        error("Cannot open input file");
    }

    int32_t min = std::numeric_limits<int32_t>::max();
    int32_t max = std::numeric_limits<int32_t>::min();
    std::vector<int32_t> v;
    while (true) {
        int val;
        is >> val;
        if (!is) {
            break;
        }
        v.push_back(val);

        if (min > val) {
            min = val;
        }
        if (max < val) {
            max = val;
        }
    }


    uint32_t n = numbits(min, max);

    std::ofstream os(output, std::ios::binary);
    if (!os) {
        error("Cannot open output file");
    }

    bitwriter bw(os);

    os << "BIN!";
    bw(min, 32); // raw_write(os, _byteswap_ulong(min));
    bw(max, 32); // raw_write(os, _byteswap_ulong(max));
    bw(v.size(), 32); // raw_write(os, _byteswap_ulong(v.size()));

    for (const auto& x : v) {
        uint32_t num = x - min;
        bw(num, n);
    }
}

template <typename T>
std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T))
{
    return is.read(reinterpret_cast<char*>(&val), size);
}

class bitreader {
    std::istream& is_;
    uint8_t buffer_;
    uint8_t n_ = 0;

    uint32_t read_bit() {
        if (n_ == 0) {
            is_.read(reinterpret_cast<char*>(&buffer_), sizeof(uint8_t));
            n_ = 8;
        }
        return (buffer_ >> --n_) & 1;
    }

public:
    bitreader(std::istream& is) : is_(is) {}

    bitreader& operator()(uint32_t& u, uint8_t n) {
        u = 0;
        while (n-- > 0) {
            u = (u << 1) | read_bit();
        }
        return *this;
    }

    explicit operator bool() {
        return !is_.fail();
    }
};

void decompress(const std::string& input, const std::string& output)
{
    std::ifstream is(input, std::ios::binary);
    if (!is) {
        error("Cannot open input file");
    }

    std::string magic(4, ' ');
    raw_read(is, magic[0], 4);
    /*magic += is.get();
    magic += is.get();
    magic += is.get();
    magic += is.get();*/

    bitreader br(is);
    int32_t min, max;
    uint32_t num_values;
    uint32_t tmp;
    br(tmp, 32);
    min = tmp;
    br(tmp, 32);
    max = tmp;
    br(num_values, 32);

    uint32_t n = numbits(min, max);

    std::ofstream os(output, std::ios::binary);
    if (!os) {
        error("Cannot open output file");
    }

    for (uint32_t i = 0; i < num_values; ++i) {
        br(tmp, n);
        os << int(tmp) + min << "\n";
    }
}

int main(int argc, char *argv[])
{
    char input[20] = { "freq.txt"};
    char output[20] = { "freq01.txt"};
    
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

