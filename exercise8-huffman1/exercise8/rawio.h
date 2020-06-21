#pragma once

#include <istream>
#include <ostream>

template <typename T>
std::ostream& raw_write(std::ostream& os, T& val, size_t size = sizeof(T))
{
	return os.write(reinterpret_cast<char*>(&val), size);
}

template <typename T>
std::istream& raw_read(std::istream& is, T& val, size_t size = sizeof(T))
{
	return is.read(reinterpret_cast<char*>(&val), size);
}
