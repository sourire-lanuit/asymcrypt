#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <fstream>

std::vector<uint8_t> built_in(size_t num_bytes);
std::vector<uint8_t> lehmer(size_t num_bytes, uint32_t start, bool is_high);
std::vector<uint8_t> lehmer_low(size_t num_bytes, uint32_t start);
std::vector<uint8_t> lehmer_high(size_t num_bytes, uint32_t start);
std::vector<uint8_t> l20(size_t num_bytes, uint32_t start);
std::vector<uint8_t> l89(size_t num_bytes, std::vector<uint8_t> start);
std::vector<uint8_t> geffe(size_t num_bytes, uint32_t start_l11, uint32_t start_l9, uint32_t start_l10);
std::vector<uint8_t> wolfram(size_t num_bytes, uint32_t start);
std::vector<uint8_t> librarian(size_t num_bytes, const std::string& filename);
std::vector<uint8_t> bm(size_t num_bytes, uint32_t start);
std::vector<uint8_t> bm_bytes(size_t num_bytes, uint32_t start);
std::vector<uint8_t> bbs(size_t num_bytes, uint32_t start);
std::vector<uint8_t> bbs_bytes(size_t num_bytes, uint32_t start);

double khi_square_quantile(double alpha, int df);
void equiviv(const std::vector<uint8_t>& sequence, double alpha);
void independence(const std::vector<uint8_t>& sequence, double alpha);
void odnor(const std::vector<uint8_t>& sequence, double alpha, int r);

#endif