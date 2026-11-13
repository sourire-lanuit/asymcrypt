#include "Header.h"
#include <cstdlib>

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
    uint32_t a = 65537; //теоретично можна кинути буде одразу сюди якийсь приклад ай гесс апд який приклад я слєпошара
    uint32_t c = 199; //апдейт я таки читати не вмію 
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

// наш герой повернувся, але світло до нього - ні... 
// ніхто не святой і я теж і закину все за раз походу

namespace {
const int Big_int = 5;
int parse_hex(const std::string& hex, uint64_t* out) {
    for (int i = 0; i < Big_int; ++i) out[i] = 0;
    int digits = (int)hex.size();
    for (int i = 0; i < digits; ++i) {
        char ch = hex[digits - 1 - i];
        uint64_t v = (ch >= '0' && ch <= '9') ? (ch - '0') : ((ch | 0x20) - 'a' + 10);
        out[i / 16] |= v << (4 * (i % 16));
    }
    return (digits + 15) / 16;
}
 
bool geq(const uint64_t* a, const uint64_t* b, int L) {
    for (int i = L - 1; i >= 0; --i) {
        if (a[i] != b[i]) return a[i] > b[i];
    }
    return true;
}
 
void subtract(uint64_t* a, const uint64_t* b, int L) {
    uint64_t borrow = 0;
    for (int i = 0; i < L; ++i) {
        unsigned __int128 d = (unsigned __int128)a[i] - b[i] - borrow;
        a[i] = (uint64_t)d;
        borrow = (uint64_t)(d >> 64) & 1;
    }
}
 

struct Montgomery {
    int L;
    uint64_t n[Big_int];
    uint64_t ninv;        
    uint64_t one[Big_int];   
    uint64_t rr[Big_int];    
    uint64_t plain1[Big_int];
 
    void double_mod(uint64_t* x) const { 
        uint64_t carry = x[L - 1] >> 63;
        for (int i = L - 1; i > 0; --i) x[i] = (x[i] << 1) | (x[i - 1] >> 63);
        x[0] <<= 1;
        if (carry || geq(x, n, L)) subtract(x, n, L);
    }
 
    void init(const uint64_t* mod, int len) {
        L = len;
        for (int i = 0; i < Big_int; ++i) { n[i] = mod[i]; one[i] = rr[i] = plain1[i] = 0; }
        plain1[0] = 1;
        uint64_t inv = n[0];
        for (int i = 0; i < 5; ++i) inv *= 2 - n[0] * inv;
        ninv = (uint64_t)0 - inv;
 
        uint64_t x[Big_int] = {0};
        x[0] = 1;
        for (int i = 0; i < 64 * L; ++i) double_mod(x);
        for (int i = 0; i < L; ++i) one[i] = x[i];
        for (int i = 0; i < 64 * L; ++i) double_mod(x);
        for (int i = 0; i < L; ++i) rr[i] = x[i];
    }
 
    void multipl(const uint64_t* a, const uint64_t* b, uint64_t* res) const {
        uint64_t t[Big_int + 2] = {0};
        for (int i = 0; i < L; ++i) {
            unsigned __int128 c = 0;
            for (int j = 0; j < L; ++j) {
                unsigned __int128 cur = (unsigned __int128)a[j] * b[i] + t[j] + c;
                t[j] = (uint64_t)cur;
                c = cur >> 64;
            }
            unsigned __int128 cur = (unsigned __int128)t[L] + c;
            t[L] = (uint64_t)cur;
            t[L + 1] = (uint64_t)(cur >> 64);
 
            uint64_t m = t[0] * ninv;
            cur = (unsigned __int128)m * n[0] + t[0];
            c = cur >> 64;
            for (int j = 1; j < L; ++j) {
                cur = (unsigned __int128)m * n[j] + t[j] + c;
                t[j - 1] = (uint64_t)cur;
                c = cur >> 64;
            }
            cur = (unsigned __int128)t[L] + c;
            t[L - 1] = (uint64_t)cur;
            t[L] = t[L + 1] + (uint64_t)(cur >> 64);
        }
        if (t[L] || geq(t, n, L)) subtract(t, n, L);
        for (int i = 0; i < L; ++i) res[i] = t[i];
    }
};
 
} 

static std::vector<uint8_t> bm_generic(size_t num_bytes, uint32_t start, bool bytes_mode) {
    std::vector<uint8_t> sequence(num_bytes, 0);
 
    uint64_t p[Big_int], a[Big_int];
    int L = parse_hex("CEA42B987C44FA642D80AD9F51F10457690DEF10C83D0BC1BCEE12FC3B6093E3", p);
    parse_hex("5B88C41246790891C095E2878880342E88C79974303BD0400B090FE38A688356", a);
 
    Montgomery M;
    M.init(p, L);
 
    const int proces = 64;
    std::vector<uint64_t> tbl((size_t)proces * 16 * L);
    auto entry = [&](int w, int d) { return &tbl[((size_t)w * 16 + d) * L]; };
 
    uint64_t base[Big_int];
    M.multipl(a, M.rr, base);
    for (int w = 0; w < proces; ++w) {
        for (int i = 0; i < L; ++i) entry(w, 0)[i] = M.one[i];
        for (int d = 1; d < 16; ++d) M.multipl(entry(w, d - 1), base, entry(w, d));
        M.multipl(entry(w, 15), base, base); 
    }
 
    uint64_t pm1[Big_int + 1] = {0}, half[Big_int + 1] = {0};
    for (int i = 0; i < L; ++i) pm1[i] = p[i];
    pm1[0] -= 1;
    for (int i = 0; i < L; ++i) half[i] = (pm1[i] >> 1) | (i + 1 < L ? pm1[i + 1] << 63 : 0);

    std::vector<uint64_t> kp;
    if (bytes_mode) {
        kp.assign(256 * (L + 1), 0);
        for (int k = 1; k < 256; ++k) {
            uint64_t carry = 0;
            for (int i = 0; i <= L; ++i) {
                unsigned __int128 s = (unsigned __int128)kp[(k - 1) * (L + 1) + i] + pm1[i] + carry;
                kp[k * (L + 1) + i] = (uint64_t)s;
                carry = (uint64_t)(s >> 64);
            }
        }
    }
 
    uint64_t T[Big_int] = {0};
    T[0] = (start == 0) ? 12345 : start;
 
    auto next_state = [&]() {
        uint64_t res[Big_int];
        for (int i = 0; i < L; ++i) res[i] = M.one[i];
        for (int w = 0; w < proces; ++w) {
            unsigned d = (unsigned)((T[w / 16] >> (4 * (w % 16))) & 15);
            if (d) M.multipl(res, entry(w, d), res);
        }
        M.multipl(res, M.plain1, T);
    };
 
    for (size_t i = 0; i < num_bytes; ++i) {
        if (!bytes_mode) {
            uint8_t current_byte = 0;
            for (int bit = 0; bit < 8; ++bit) {
                next_state();
                int out_bit = !geq(T, half, L) ? 1 : 0;
                current_byte |= (out_bit << bit);
            }
            sequence[i] = current_byte;
        } else {
            next_state();
            uint64_t v[Big_int + 1];
            for (int j = 0; j < L; ++j) v[j] = (T[j] << 8) | (j ? T[j - 1] >> 56 : 0);
            v[L] = T[L - 1] >> 56;
            int low = 0, high = 255;
            while (low < high) {
                int mid = (low + high + 1) / 2;
                if (!geq(&kp[mid * (L + 1)], v, L + 1)) low = mid;
                else high = mid - 1;
            }
            sequence[i] = (uint8_t)low;
        }
    }
    return sequence;
}
 
std::vector<uint8_t> bm(size_t num_bytes, uint32_t start) {
    return bm_generic(num_bytes, start, false);
}
 
std::vector<uint8_t> bm_bytes(size_t num_bytes, uint32_t start) {
    return bm_generic(num_bytes, start, true);
}

static std::vector<uint8_t> bbs_generic(size_t num_bytes, uint32_t start, bool bytes_mode) {
    std::vector<uint8_t> sequence(num_bytes, 0);
 
    uint64_t p[Big_int], q[Big_int], n[Big_int] = {0};
    int lp = parse_hex("D5BBB96D30086EC484EBA3D7F9CAEB07", p);
    int lq = parse_hex("425D2B9BFDB25B9CF6C416CC6E37B59C1F", q);
    for (int i = 0; i < lp; ++i) {
        uint64_t carry = 0;
        for (int j = 0; j < lq; ++j) {
            unsigned __int128 cur = (unsigned __int128)p[i] * q[j] + n[i + j] + carry;
            n[i + j] = (uint64_t)cur;
            carry = (uint64_t)(cur >> 64);
        }
        n[i + lq] = carry;
    }
 
    Montgomery M;
    M.init(n, lp + lq);
 
    uint64_t r0[Big_int] = {0}, r[Big_int], plain[Big_int];
    r0[0] = start < 2 ? 2 : start;
    M.multipl(r0, M.rr, r);
 
    for (size_t i = 0; i < num_bytes; ++i) {
        if (!bytes_mode) {
            uint8_t current_byte = 0;
            for (int bit = 0; bit < 8; ++bit) {
                M.multipl(r, r, r);
                M.multipl(r, M.plain1, plain);
                current_byte |= ((plain[0] & 1) << bit);
            }
            sequence[i] = current_byte;
        } else {
            M.multipl(r, r, r);
            M.multipl(r, M.plain1, plain);
            sequence[i] = (uint8_t)(plain[0] & 0xFF);
        }
    }
    return sequence;
}
 
std::vector<uint8_t> bbs(size_t num_bytes, uint32_t start) {
    return bbs_generic(num_bytes, start, false);
}
 
std::vector<uint8_t> bbs_bytes(size_t num_bytes, uint32_t start) {
    return bbs_generic(num_bytes, start, true);
}