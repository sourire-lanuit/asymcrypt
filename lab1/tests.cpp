#include "Header.h"
#include <cmath>

double z_alpha(double alpha) {
    if (alpha == 0.01) return 2.326;
    if (alpha == 0.05) return 1.645;
    if (alpha == 0.10) return 1.282;
    return 1.645; 
}

double khi_square_quantile(double alpha, int df) {
    double z = z_alpha(alpha);
    return std::sqrt(2.0 * df) * z + df;
}

void equiviv(const std::vector<uint8_t>& sequence, double alpha) {
    size_t m = sequence.size();
    std::vector<size_t> v(256, 0);
    
    for (uint8_t byte : sequence) {
        v[byte]++;
    }
    
    double n_j = static_cast<double>(m) / 256.0;
    double khi_square = 0.0;
    
    for (int j = 0; j < 256; ++j) {
        khi_square += std::pow(v[j] - n_j, 2) / n_j;
    }
    
    double khi_theory = khi_square_quantile(alpha, 255);
    
    std::cout << "\nТест на рівноімовірність (alpha = " << alpha << ")\n";
    std::cout << "Обчислений x^2: " << khi_square << "\n";
    std::cout << "Теоретичний x^2: " << khi_theory << "\n";
    if (khi_square <= khi_theory) {
        std::cout << "=> гіпотеза H_0 не відкидається => тест пройдено\n";
    } else {
        std::cout << "=> гіпотеза H_0 відкидається => тест не пройдено\n";
    }
}

void independence(const std::vector<uint8_t>& sequence, double alpha) {
    size_t n = sequence.size() / 2;
    std::vector<std::vector<size_t>> v_ij(256, std::vector<size_t>(256, 0));
    std::vector<size_t> v_i(256, 0);
    std::vector<size_t> a_j(256, 0);
    
    for (size_t k = 0; k < n; ++k) {
        uint8_t byte1 = sequence[2 * k];
        uint8_t byte2 = sequence[2 * k + 1];
        v_ij[byte1][byte2]++;
        v_i[byte1]++;
        a_j[byte2]++;
    }
    
    double khi_square = 0.0;
    for (int i = 0; i < 256; ++i) {
        for (int j = 0; j < 256; ++j) {
            if (v_i[i] > 0 && a_j[j] > 0) {
                khi_square += std::pow(v_ij[i][j], 2) / (static_cast<double>(v_i[i]) * a_j[j]);
            }
        }
    }
    khi_square = n * (khi_square - 1);
    
    double khi_theory = khi_square_quantile(alpha, 255 * 255);
    
    std::cout << "\nТест на незалежність (alpha = " << alpha << ")\n";
    std::cout << "Обчислений x^2: " << khi_square << "\n";
    std::cout << "Теоретичний x^2: " << khi_theory << "\n";
    if (khi_square <= khi_theory) {
        std::cout << "=> гіпотеза H_0 не відкидається => тест пройдено\n";
    } else {
        std::cout << "=> гіпотеза H_0 відкидається => тест не пройдено\n";
    }
}

void odnor(const std::vector<uint8_t>& sequence, double alpha, int r) {
    size_t m = sequence.size();
    size_t m_prime = m / r; 
    size_t n = m_prime * r;
    
    std::vector<std::vector<size_t>> v_ij(256, std::vector<size_t>(r, 0));
    std::vector<size_t> v_i(256, 0);
    
    for (int j = 0; j < r; ++j) {
        for (size_t k = 0; k < m_prime; ++k) {
            uint8_t byte = sequence[j * m_prime + k];
            v_ij[byte][j]++;
            v_i[byte]++;
        }
    }
    
    double khi_square = 0.0;
    for (int i = 0; i < 256; ++i) {
        for (int j = 0; j < r; ++j) {
            if (v_i[i] > 0 && m_prime > 0) {
                khi_square += std::pow(v_ij[i][j], 2) / (static_cast<double>(v_i[i]) * m_prime);
            }
        }
    }
    khi_square = n * (khi_square - 1.0);
    
    int df = 255 * (r - 1);
    double khi_theory = khi_square_quantile(alpha, df);
    
    std::cout << "\nТест на однорідність (alpha = " << alpha << ", r = " << r << ")\n";
    std::cout << "Обчислений x^2: " << khi_square << "\n";
    std::cout << "Теоретичний x^2: " << khi_theory << "\n";
    if (khi_square <= khi_theory) {
        std::cout << "=> гіпотеза H0 не відкидається => тест пройдено\n";
    } else {
        std::cout << "=> гіпотеза H0 відкидається => тест не пройдено\n";
    }
}