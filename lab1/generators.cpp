#include "Header.h"

std::vector<uint8_t> built_in(size_t num_bytes) {
    std::vector<uint8_t> sequence(num_bytes);
    for (size_t i = 0; i < num_bytes; ++i) {
        sequence[i] = std::rand() % 256;
    }
    return sequence;
}

std::vector<uint8_t> lehmer(size_t num_bytes, uint32_t start, bool is_high) {
    std::vector<uint8_t> sequence(num_bytes);
    uint32_t x = (start == 0) ? 1 : start;
    uint32_t a; //теоретично можна кинути буде одразу сюди якийсь приклад ай гесс
    uint32_t c; 
    for (size_t i = 0; i < num_bytes; ++i) {
        x = x * a + c;
        sequence[i] = is_high ? (x >> 24) : (x & 0xFF);
    }
    return sequence;
}

std::vector<uint8_t> lehmer_low(size_t num_bytes, uint32_t start) {
    return lehmer(num_bytes, start, false);
}

std::vector<uint8_t> lehmer_high(size_t num_bytes, uint32_t start) {
    return lehmer(num_bytes, start, true);
}

std::vector<uint8_t> l20(size_t num_bytes, uint32_t start) {
    std::vector<uint8_t> sequence(num_bytes, 0);
    std::vector<int> state(20, 0);
    
    for(int i = 0; i < 20; i++) {
        state[i] = (start >> i) & 1;
    }
    if (start == 0) state[0] = 1;

    for (size_t i = 0; i < num_bytes; ++i) {
        uint8_t current_byte = 0;
        for (int bit = 0; bit < 8; ++bit) {
            int next = state[17] ^ state[15] ^ state[11] ^ state[0]; //щас окажєцця я ні читать ні рахувать не вмію
            current_byte |= (next << bit);
            for (int j = 0; j < 19; ++j) {
                state[j] = state[j + 1];
            }
            state[19] = next;
        }
        sequence[i] = current_byte;
    }
    return sequence;
}

