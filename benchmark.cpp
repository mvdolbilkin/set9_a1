#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

static uint64_t global_char_comparisons = 0;

bool countingCompare(const string &a, const string &b) {
  size_t min_len = min(a.length(), b.length());
  for (size_t i = 0; i < min_len; ++i) {
    global_char_comparisons++;
    if (a[i] != b[i]) {
      return a[i] < b[i];
    }
  }
  global_char_comparisons++;
  return a.length() < b.length();
}

class StringGenerator {
private:
  mt19937 rng;
  string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012345"
                    "6789!@#%:;^&*()-";

  string generateRandomString(size_t min_len = 10, size_t max_len = 200) {
    uniform_int_distribution<size_t> len_dist(min_len, max_len);
    uniform_int_distribution<size_t> char_dist(0, alphabet.size() - 1);
    size_t len = len_dist(rng);
    string res;
    res.reserve(len);
    for (size_t i = 0; i < len; ++i)
      res.push_back(alphabet[char_dist(rng)]);
    return res;
  }

public:
  StringGenerator(uint32_t seed = 42) : rng(seed) {}

  vector<string> generateRandom(size_t size) {
    vector<string> res;
    res.reserve(size);
    vector<string> bases;
    for (int i = 0; i < 20; ++i)
      bases.push_back(generateRandomString(5, 30));
    uniform_int_distribution<size_t> base_dist(0, bases.size() - 1);
    uniform_int_distribution<size_t> suffix_dist(5, 170);
    uniform_int_distribution<size_t> char_dist(0, alphabet.size() - 1);

    for (size_t i = 0; i < size; ++i) {
      string s = bases[base_dist(rng)];
      size_t suffix_len = suffix_dist(rng);
      for (size_t j = 0; j < suffix_len; ++j)
        s.push_back(alphabet[char_dist(rng)]);
      res.push_back(s);
    }
    return res;
  }

  vector<string> generateReverseSorted(size_t size) {
    auto arr = generateRandom(size);
    sort(arr.begin(), arr.end(), greater<string>());
    return arr;
  }

  vector<string> generateNearlySorted(size_t size, size_t swaps = 10) {
    auto arr = generateRandom(size);
    sort(arr.begin(), arr.end());
    uniform_int_distribution<size_t> idx_dist(0, size - 1);
    for (size_t i = 0; i < swaps; ++i)
      swap(arr[idx_dist(rng)], arr[idx_dist(rng)]);
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
  static TestResult testAlgorithm(SortFunc sort_func,
                                  const vector<string> &data, int runs = 10) {
    uint64_t total_time_us = 0;
    uint64_t total_comps = 0;

    for (int i = 0; i < runs; ++i) {
      vector<string> data_copy = data;
      global_char_comparisons = 0;

      auto start = chrono::high_resolution_clock::now();
      sort_func(data_copy);
      auto end = chrono::high_resolution_clock::now();

      total_time_us +=
          chrono::duration_cast<chrono::microseconds>(end - start).count();
      total_comps += global_char_comparisons;
    }

    return {(total_time_us / 1000.0) / runs, total_comps / runs};
  }
};

void standardQuickSortImpl(vector<string> &arr, int lo, int hi) {
  if (lo >= hi)
    return;
  string pivot = arr[lo];
  int i = lo, j = hi + 1;
  while (true) {
    while (countingCompare(arr[++i], pivot))
      if (i == hi)
        break;
    while (countingCompare(pivot, arr[--j]))
      if (j == lo)
        break;
    if (i >= j)
      break;
    swap(arr[i], arr[j]);
  }
  swap(arr[lo], arr[j]);
  standardQuickSortImpl(arr, lo, j - 1);
  standardQuickSortImpl(arr, j + 1, hi);
}
void standardQuickSort(vector<string> &arr) {
  if (!arr.empty())
    standardQuickSortImpl(arr, 0, arr.size() - 1);
}

void standardMerge(vector<string> &arr, vector<string> &aux, int lo, int mid,
                   int hi) {
  for (int k = lo; k <= hi; ++k)
    aux[k] = arr[k];
  int i = lo, j = mid + 1;
  for (int k = lo; k <= hi; ++k) {
    if (i > mid)
      arr[k] = aux[j++];
    else if (j > hi)
      arr[k] = aux[i++];
    else if (countingCompare(aux[j], aux[i]))
      arr[k] = aux[j++];
    else
      arr[k] = aux[i++];
  }
}
void standardMergeSortImpl(vector<string> &arr, vector<string> &aux, int lo,
                           int hi) {
  if (hi <= lo)
    return;
  int mid = lo + (hi - lo) / 2;
  standardMergeSortImpl(arr, aux, lo, mid);
  standardMergeSortImpl(arr, aux, mid + 1, hi);
  standardMerge(arr, aux, lo, mid, hi);
}
void standardMergeSort(vector<string> &arr) {
  vector<string> aux(arr.size());
  if (!arr.empty())
    standardMergeSortImpl(arr, aux, 0, arr.size() - 1);
}

inline int charAt(const string &s, int d) {
  if (d < s.length()) {
    global_char_comparisons++;
    return (unsigned char)s[d];
  }
  return -1;
}

void stringQuickSortImpl(vector<string> &arr, int lo, int hi, int d) {
  if (hi <= lo)
    return;
  int lt = lo, gt = hi;
  int v = charAt(arr[lo], d);
  int i = lo + 1;
  while (i <= gt) {
    int t = charAt(arr[i], d);
    if (t < v)
      swap(arr[lt++], arr[i++]);
    else if (t > v)
      swap(arr[i], arr[gt--]);
    else
      i++;
  }
  stringQuickSortImpl(arr, lo, lt - 1, d);
  if (v >= 0)
    stringQuickSortImpl(arr, lt, gt, d + 1);
  stringQuickSortImpl(arr, gt + 1, hi, d);
}
void stringQuickSort(vector<string> &arr) {
  if (!arr.empty())
    stringQuickSortImpl(arr, 0, arr.size() - 1, 0);
}

struct StringItem {
  string s;
  int lcp;
};
struct LcpResult {
  char cmp;
  int lcp;
};

LcpResult lcpCompare(const string &a, const string &b, int k) {
  int n = min(a.length(), b.length());
  int i = k;
  while (i < n) {
    global_char_comparisons++;
    if (a[i] == b[i])
      i++;
    else
      break;
  }
  global_char_comparisons++;
  if (i == a.length() && i == b.length())
    return {'=', i};
  if (i == a.length())
    return {'<', i};
  if (i == b.length())
    return {'>', i};
  if (a[i] < b[i])
    return {'<', i};
  return {'>', i};
}

vector<StringItem> stringMergeSortImpl(const vector<StringItem> &arr) {
  int n = arr.size();
  if (n <= 1) {
    if (n == 1) {
      vector<StringItem> res = arr;
      res[0].lcp = 0;
      return res;
    }
    return {};
  }
  int mid = n / 2;
  vector<StringItem> left(arr.begin(), arr.begin() + mid);
  vector<StringItem> right(arr.begin() + mid, arr.end());
  left = stringMergeSortImpl(left);
  right = stringMergeSortImpl(right);

  vector<StringItem> res;
  res.reserve(n);
  int i = 0, j = 0;
  int m = left.size(), f = right.size();

  while (i < m && j < f) {
    if (left[i].lcp > right[j].lcp) {
      res.push_back(left[i]);
      i++;
    } else if (left[i].lcp < right[j].lcp) {
      res.push_back(right[j]);
      j++;
    } else {
      LcpResult cmpRes = lcpCompare(left[i].s, right[j].s, left[i].lcp);
      if (cmpRes.cmp == '<' || cmpRes.cmp == '=') {
        res.push_back(left[i]);
        i++;
        right[j].lcp = cmpRes.lcp;
      } else {
        res.push_back(right[j]);
        j++;
        left[i].lcp = cmpRes.lcp;
      }
    }
  }
  while (i < m) {
    res.push_back(left[i]);
    i++;
  }
  while (j < f) {
    res.push_back(right[j]);
    j++;
  }
  return res;
}

void stringMergeSort(vector<string> &arr) {
  if (arr.empty())
    return;
  vector<StringItem> items(arr.size());
  for (size_t i = 0; i < arr.size(); ++i) {
    items[i].s = arr[i];
    items[i].lcp = 0;
  }
  items = stringMergeSortImpl(items);
  for (size_t i = 0; i < arr.size(); ++i)
    arr[i] = items[i].s;
}

const int R = 256;
void msdRadixSortImpl(vector<string> &arr, vector<string> &aux, int lo, int hi,
                      int d) {
  if (hi <= lo)
    return;
  int count[R + 2] = {0};
  for (int i = lo; i <= hi; i++)
    count[charAt(arr[i], d) + 2]++;
  for (int r = 0; r < R + 1; r++)
    count[r + 1] += count[r];
  for (int i = lo; i <= hi; i++)
    aux[count[charAt(arr[i], d) + 1]++] = arr[i];
  for (int i = lo; i <= hi; i++)
    arr[i] = aux[i - lo];
  for (int r = 0; r < R; r++)
    msdRadixSortImpl(arr, aux, lo + count[r], lo + count[r + 1] - 1, d + 1);
}
void msdRadixSort(vector<string> &arr) {
  if (arr.size() <= 1)
    return;
  vector<string> aux(arr.size());
  msdRadixSortImpl(arr, aux, 0, arr.size() - 1, 0);
}

const int CUTOFF = 74;
void msdRadixQuickSortImpl(vector<string> &arr, vector<string> &aux, int lo,
                           int hi, int d) {
  if (hi <= lo)
    return;
  if (hi - lo + 1 < CUTOFF) {
    stringQuickSortImpl(arr, lo, hi, d);
    return;
  }
  int count[R + 2] = {0};
  for (int i = lo; i <= hi; i++)
    count[charAt(arr[i], d) + 2]++;
  for (int r = 0; r < R + 1; r++)
    count[r + 1] += count[r];
  for (int i = lo; i <= hi; i++)
    aux[count[charAt(arr[i], d) + 1]++] = arr[i];
  for (int i = lo; i <= hi; i++)
    arr[i] = aux[i - lo];
  for (int r = 0; r < R; r++)
    msdRadixQuickSortImpl(arr, aux, lo + count[r], lo + count[r + 1] - 1,
                          d + 1);
}
void msdRadixQuickSort(vector<string> &arr) {
  if (arr.size() <= 1)
    return;
  vector<string> aux(arr.size());
  msdRadixQuickSortImpl(arr, aux, 0, arr.size() - 1, 0);
}

int main() {
  ofstream out("results.csv");
  out << "ArrayType,Size,Algorithm,TimeMs,Comparisons\n";

  StringGenerator gen;
  vector<string> types = {"Random", "Reversed", "NearlySorted"};

  vector<pair<string, void (*)(vector<string> &)>> algorithms = {
      {"Standard_QuickSort", standardQuickSort},
      {"Standard_MergeSort", standardMergeSort},
      {"String_QuickSort", stringQuickSort},
      {"String_MergeSort", stringMergeSort},
      {"MSD_RadixSort", msdRadixSort},
      {"MSD_RadixQuickSort", msdRadixQuickSort}};

  cout << "Starting benchmarks...\n";

  for (const string &type : types) {
    for (int size = 100; size <= 3000; size += 100) {
      cout << "Testing " << type << " size " << size << "...\n";

      vector<string> data;
      if (type == "Random")
        data = gen.generateRandom(size);
      else if (type == "Reversed")
        data = gen.generateReverseSorted(size);
      else
        data = gen.generateNearlySorted(size, 10);

      for (const auto &algo : algorithms) {
        TestResult res = StringSortTester::testAlgorithm(algo.second, data, 10);
        out << type << "," << size << "," << algo.first << ","
            << res.avg_time_ms << "," << res.avg_comparisons << "\n";
      }
    }
  }

  out.close();
  cout << "Benchmarks finished! Results saved to results.csv\n";
  return 0;
}