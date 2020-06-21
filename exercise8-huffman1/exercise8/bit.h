#pragma once

#include "rawio.h"

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
        while (n-- > 0) {
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

class bitreader {
    std::istream& is_;
    uint8_t buffer_;
    uint8_t n_ = 0;

public:
    uint32_t read_bit() {
        if (n_ == 0) {
            raw_read(is_, buffer_);
            n_ = 8;
        }
        return (buffer_ >> --n_) & 1;
    }

    bitreader& read(uint32_t& u, uint8_t n) {
        u = 0;
        while (n-- > 0) {
            u = (u << 1) | read_bit();
        }
        return *this;
    }

    bitreader(std::istream& is) : is_(is) {}

    bitreader& operator()(uint32_t& u, uint8_t n) {
        return read(u, n);
    }
    bitreader& operator()(int32_t& u, uint8_t n) {
        return read(reinterpret_cast<uint32_t&>(u), n);
    }

    bitreader& operator()(uint8_t& u, uint8_t n) {
        assert(n <= 8);
        uint32_t tmp;
        read(tmp, n);
        u = static_cast<uint8_t>(tmp);
        return *this;
    }

    explicit operator bool() {
        return !is_.fail();
    }
};
