#include "Header.h"
#include <ctime>
#include <iostream>
#include <random>


void run_all_tests(const std::string& gen_name, const std::vector<uint8_t>& sequence) {
    std::cout << "\n-----------------------\n";
    std::cout << "Генератор: " << gen_name << std::endl;
    std::cout << "-------------------------\n";
    
    if (sequence.empty()) {
        std::cout << "Послідовність порожня\n";
        return;
    }

    std::vector<double> alphas = {0.01, 0.05, 0.1};
    int r = 10; 

    for (double alpha : alphas) {
        equiviv(sequence, alpha);
        independence(sequence, alpha);
        odnor(sequence, alpha, r);
    }
}

int main() {
    std::srand(std::time(nullptr));
    
    size_t bits = 1000000;
    size_t bytes = bits / 8;
    
    std::cout << "Довжина послідовности: " << bits << " бітів (" << bytes << " байтів)\n";
    
    uint32_t start1 = 131313131;
    uint32_t start2 = 987654321;
    uint32_t start3 = 555555555;
    
    run_all_tests("вбудований", built_in(bytes));
    run_all_tests("Lehmer Low", lehmer_low(bytes, start1));
    run_all_tests("Lehmer High", lehmer_high(bytes, start1));
    run_all_tests("L20", l20(bytes, start1));

    std::mt19937 start_gen(2024);
    std::vector<uint8_t> start_89(12);
    for (auto& b : start_89) b = start_gen() & 0xFF;
    run_all_tests("L89", l89(bytes, start_89));

    run_all_tests("Geffe", geffe(bytes, start1, start2, start3));
    run_all_tests("Wolfram", wolfram(bytes, start1));
    run_all_tests("Librarian", librarian(bytes, "C:\\Users\\User\\Desktop\\studie\\cpp\\.vscode\\progs\\ascr\\lab1\\text.txt"));
    run_all_tests("BM", bm(bytes, start1));
    run_all_tests("BM_bytes", bm_bytes(bytes, start1));
    run_all_tests("BBS", bbs(bytes, start1));
    run_all_tests("BBS_bytes", bbs_bytes(bytes, start1));

    return 0;
}