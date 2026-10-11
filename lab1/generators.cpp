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

std::vector<uint8_t> l89(size_t num_bytes, std::vector<uint8_t> start) {
    std::vector<uint8_t> sequence(num_bytes, 0);
    std::vector<int> state(89, 0);
    
    int index = 0;
    for (uint8_t b : start) {
        for (int i = 0; i < 8 && index < 89; ++i) {
            state[index++] = (b >> i) & 1;
        }
    }
    state[0] = 1;

    for (size_t i = 0; i < num_bytes; ++i) {
        uint8_t current_byte = 0;
        for (int bit = 0; bit < 8; ++bit) {
            int next = state[51] ^ state[0]; //рекуренти ці ваші 
            current_byte |= (next << bit);
        
            for (int j = 0; j < 88; ++j) {
                state[j] = state[j + 1];
            }
            state[88] = next;
        }
        sequence[i] = current_byte;
    }
    return sequence;
}

//ходять чутки що я досі б'юсь головою об стіл після генератора джиффі з симкрипти і от він знову
std::vector<uint8_t> geffe(size_t num_bytes, uint32_t start_l11, uint32_t start_l9, uint32_t start_l10) {
    std::vector<uint8_t> sequence(num_bytes, 0);
    std::vector<int> L11(11), L9(9), L10(10);
    
    for (int i = 0; i < 11; i++) L11[i] = (start_l11 >> i) & 1;
    for (int i = 0; i < 9;  i++) L9[i]  = (start_l9  >> i) & 1;
    for (int i = 0; i < 10; i++) L10[i] = (start_l10 >> i) & 1;
    
    if (start_l11 == 0) L11[0] = 1;
    if (start_l9 == 0)  L9[0]  = 1;
    if (start_l10 == 0) L10[0] = 1;

    for (size_t i = 0; i < num_bytes; ++i) {
        uint8_t current = 0;
        for (int bit = 0; bit < 8; ++bit) {
            int next_l11 = L11[0] ^ L11[2];
            int x = L11[0];
            for (int j = 0; j < 10; j++) L11[j] = L11[j+1];
            L11[10] = next_l11;
            
            int next_l9 = L9[0] ^ L9[1] ^ L9[3] ^ L9[4];
            int y = L9[0];
            for (int j = 0; j < 8; j++) L9[j] = L9[j+1];
            L9[8] = next_l9;
            
            int next_l10 = L10[0] ^ L10[3];
            int s = L10[0];
            for (int j = 0; j < 9; j++) L10[j] = L10[j+1];
            L10[9] = next_l10;
    
            int z = (s & x) ^ ((1 ^ s) & y);
            current |= (z << bit);
        }
        sequence[i] = current;
    }
    return sequence;
}

uint32_t left32(uint32_t val, int shift) { 
    return (val << shift) | (val >> (32 - shift)); 
}
uint32_t right32(uint32_t val, int shift) { 
    return (val >> shift) | (val << (32 - shift)); 
}

std::vector<uint8_t> wolfram(size_t num_bytes, uint32_t start) {
    std::vector<uint8_t> sequence(num_bytes, 0);
    uint32_t r = (start == 0) ? 1 : start;
    
    for (size_t i = 0; i < num_bytes; ++i) {
        uint8_t val = 0;
        for (int bit = 0; bit < 8; ++bit) {
            val |= ((r & 1) << bit);
            r = left32(r, 1) ^ (r | right32(r, 1));
        }
        sequence[i] = val;
    }
    return sequence;
}

std::vector<uint8_t> librarian(size_t num_bytes, const std::string& filename) {
    std::vector<uint8_t> sequence;
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "pomylka vidkryttya\n"; // nishevo
        return sequence;
    }
    char byte;
    while (file.get(byte) && sequence.size() < num_bytes) {
        sequence.push_back(static_cast<uint8_t>(byte));
    }
    return sequence;
}

// все було так вдупляємо навіть якщо і реялізація виявиться неправильною
// а потім стались бм і ббс 

