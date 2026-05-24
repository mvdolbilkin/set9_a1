#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>

static uint64_t global_char_comparisons = 0;

class StringGenerator {
private:
    std::mt19937 rng;
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%:;^&*()-";

    std::string generateRandomString(size_t min_len = 10, size_t max_len = 200) {
        std::uniform_int_distribution<size_t> len_dist(min_len, max_len);
        std::uniform_int_distribution<size_t> char_dist(0, alphabet.size() - 1);
        
        size_t len = len_dist(rng);
        std::string res;
        res.reserve(len);
        for (size_t i = 0; i < len; ++i) {
            res.push_back(alphabet[char_dist(rng)]);
        }
        return res;
    }

public:
    StringGenerator(uint32_t seed = 42) : rng(seed) {}

    std::vector<std::string> generateRandom(size_t size) {
        std::vector<std::string> res;
        res.reserve(size);
        
        std::vector<std::string> bases;
        for (int i = 0; i < 20; ++i) {
            bases.push_back(generateRandomString(5, 30));
        }
        
        std::uniform_int_distribution<size_t> base_dist(0, bases.size() - 1);
        std::uniform_int_distribution<size_t> suffix_dist(5, 170);
        std::uniform_int_distribution<size_t> char_dist(0, alphabet.size() - 1);

        for (size_t i = 0; i < size; ++i) {
            std::string s = bases[base_dist(rng)];
            size_t suffix_len = suffix_dist(rng);
            for(size_t j = 0; j < suffix_len; ++j) {
                s.push_back(alphabet[char_dist(rng)]);
            }
            res.push_back(s);
        }
        return res;
    }

    std::vector<std::string> generateReverseSorted(size_t size) {
        auto arr = generateRandom(size);
        std::sort(arr.begin(), arr.end(), std::greater<std::string>());
        return arr;
    }

    std::vector<std::string> generateNearlySorted(size_t size, size_t swaps = 10) {
        auto arr = generateRandom(size);
        std::sort(arr.begin(), arr.end());
        
        std::uniform_int_distribution<size_t> idx_dist(0, size - 1);
        for (size_t i = 0; i < swaps; ++i) {
            std::swap(arr[idx_dist(rng)], arr[idx_dist(rng)]);
        }
        return arr;
    }
};

struct TestResult {
    double avg_time_ms;
    uint64_t avg_comparisons;
};

class StringSortTester {
public:
    template <typename SortFunc>
    static TestResult testAlgorithm(SortFunc sort_func, const std::vector<std::string>& data, int runs = 10) {
        uint64_t total_time_us = 0;
        uint64_t total_comps = 0;

        for (int i = 0; i < runs; ++i) {
            std::vector<std::string> data_copy = data;
            
            global_char_comparisons = 0;

            auto start = std::chrono::high_resolution_clock::now();
            sort_func(data_copy);
            auto end = std::chrono::high_resolution_clock::now();

            total_time_us += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            total_comps += global_char_comparisons;
        }

        TestResult result;
        result.avg_time_ms = (total_time_us / 1000.0) / runs;
        result.avg_comparisons = total_comps / runs;
        
        return result;
    }
};