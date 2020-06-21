#include "comp_decomp.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <array>
#include <cmath>

#include "bit.h"

static void error(const std::string& s)
{
	std::cerr << s << "\n";
	exit(EXIT_FAILURE);
}

class freq{
    uint32_t counters_[256] = {0};
    uint32_t total_ = 0;
public:
    void operator()(uint8_t val){
        ++counters_[val];
        ++total_;
    }
    uint32_t total() const {return total_;}
    uint32_t operator[](size_t pos){
        return counters_[pos];
    }
    double entropy(){
        double total = 0;
        for (size_t i = 0; i < 256; i++){
            total += counters_[i];
        }
        double h = 0;
        for (size_t i = 0; i < 256; i++){
            double p = counters_[i] / total;
            if(p>0){
                h += p*log2(p);
            }
        }
        return -h;
    }
};

struct node{
    uint8_t sym_;
    uint32_t p_;
    uint32_t code_ = 0;
    uint8_t len_ = 0;
    node *left_ = nullptr;
    node *right_ = nullptr;
    
    node(uint8_t sym, uint32_t p) : sym_(sym), p_(p) {}
    node(node* left, node* right) :
        p_(left->p_ + right->p_), left_(left), right_(right) {}
    
    bool leaf() const { return left_ == nullptr; }
};

void print_bin(uint32_t code, uint8_t len){
    while (len --> 0) {
        std::cout << ((code >> len) & 1);
    }
}

void print_tree(const node *n, size_t tabs = 0) {
    std::cout << std::string(tabs, '\t');
    if (n->leaf()) {
        std::cout << n->sym_ << ", " << n->p_ << ", ";
        print_bin(n->code_, n->len_);
        std::cout << '\n';
    }
    else {
        std::cout << ".\n";
        print_tree(n->left_, tabs + 1);
        print_tree(n->right_, tabs + 1);
    }
}

void create_codes(node *n, uint32_t code = 0, uint8_t len = 0) {
    if (n->leaf()) {
        n->code_ = code;
        n->len_ = len;
    } else {
        code <<= 1;
        create_codes(n->left_, code, len + 1);
        code += 1;
        create_codes(n->right_, code, len + 1);
    }
}

void compress(const std::string& input, const std::string& output)
{
	std::ifstream is(input);
	if (!is) {
		error("Cannot open input file");
	}
    
    freq f;
    uint8_t x;
    while(raw_read(is, x)){
        f(x);
    }
    
    std::vector<node*> list;
    std::array<node*, 256> table = {0};
    uint8_t nentries = 0;
    for(size_t i = 0; i < 256; i++){
        if(f[i] > 0){
            auto n = new node(static_cast<uint8_t>(i), f[i]);
            list.push_back(n);
            table[i] = n;
            ++nentries;
        }
    }
    
    while(list.size() > 1){
        sort(list.begin(), list.end(), [](const node* a, const node* b){
            return a->p_ > b->p_;
        });
        
        node *n1 = list.back();
        list.pop_back();
        node *n2 = list.back();
        list.pop_back();
        
        list.push_back(new node(n1, n2));
    }
    node *root = list.back();
    list.pop_back();
    
    create_codes(root);

    print_tree(root);
    
    is.clear();
    is.seekg(0);
    
	std::ofstream os(output, std::ios::binary);
	if (!os) {
		error("Cannot open output file");
	}

	bitwriter bw(os);

	os << "HUFFMAN1";
	bw(nentries, 8); // raw_write(os, _byteswap_ulong(min));
    for(size_t i = 0; i < 256; i++){
        if(table[i] != nullptr){
            bw(table[i]->sym_, 8);
            bw(table[i]->len_, 5);
            bw(table[i]->code_, table[i]->len_);
        }
    }
    bw(f.total(), 32);
    while (raw_read(is, x)) {
        bw(table[x]->code_, table[x]->len_);
    }
}

struct huff_entry {
    uint8_t sym_;
    uint32_t code_;
    uint8_t len_;
    
    bool operator<(const huff_entry& other) {
        return len_ < other.len_;
    }
};
        
bool mycompare(huff_entry* a, huff_entry* b){
    return (a->len_ < b->len_);
}

void decompress(const std::string& input, const std::string& output)
{
	std::ifstream is(input, std::ios::binary);
	if (!is) {
		error("Cannot open input file");
	}

	std::string magic(8, ' ');
	raw_read(is, magic[0], 8);
	if (magic != "HUFFMAN1") {
		error("Wrong format for input file");
	}

	bitreader br(is);
    uint32_t nentries = 0;
	br(nentries, 8);
    if (nentries == 0){
        nentries = 256;
    }
    
    std::vector<huff_entry> table(nentries);
    for (size_t i = 0; i < nentries; i++){
        br(table[i].sym_, 8);
        br(table[i].len_, 5);
        br(table[i].code_, table[i].len_);
    }
    
    sort(table.begin(), table.end(), mycompare);
    std::ofstream os(output, std::ios::binary);
    if (!os) {
        error("Cannot open output file");
    }
//
//    uint32_t nsym;
//    br(nsym, 32);
//    for (size_t i = 0; i < nsym; ++i) {
//        uint32_t code = 0;
//        uint8_t len = 0;
//        size_t pos = 0;
//        while (true) {
//            if (pos == nentries) {
//                error("Code not found!");
//            }
//            while (len < table[pos].len_) {
//                code = (code << 1) | br.read_bit();
//                ++len;
//            }
//            if (!br) {
//                error("Stream is too short!");
//            }
//            if (code == table[pos].code_) {
//                break;
//            }
//            ++pos;
//        }
//
//        raw_write(os, table[pos].sym_);
//    }
}
