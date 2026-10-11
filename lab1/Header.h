#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <fstream>

std::vector<uint8_t> built_in(size_t num_bytes);
std::vector<uint8_t> lehmer(size_t num_bytes, uint32_t seed, bool is_high);
std::vector<uint8_t> lehmer_low(size_t num_bytes, uint32_t start);
std::vector<uint8_t> lehmer_high(size_t num_bytes, uint32_t start);
std::vector<uint8_t> l20(size_t num_bytes, uint32_t start);
std::vector<uint8_t> l89(size_t num_bytes, std::vector<uint8_t> start);
std::vector<uint8_t> geffe(size_t num_bytes, uint32_t start_l11, uint32_t start_l9, uint32_t start_l10);
std::vector<uint8_t> wolfram(size_t num_bytes, uint32_t start);
std::vector<uint8_t> librarian(size_t num_bytes, const std::string& filename);

#endif