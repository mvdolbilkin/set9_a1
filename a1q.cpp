#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int charAt(const std::string &s, size_t d) {
  if (d < s.length()) {
    return s[d];
  }
  return -1;
}

void stringQuickSortImpl(std::vector<std::string> &arr, int lo, int hi, int d) {
  if (hi <= lo)
    return;
  int lt = lo;
  int gt = hi;
  int v = charAt(arr[lo], d);
  int i = lo + 1;
  while (i <= gt) {
    int t = charAt(arr[i], d);
    if (t < v) {
      std::swap(arr[lt++], arr[i++]);
    } else if (t > v) {
      std::swap(arr[i], arr[gt--]);
    } else {
      i++;
    }
  }
  stringQuickSortImpl(arr, lo, lt - 1, d);
  if (v >= 0) {
    stringQuickSortImpl(arr, lt, gt, d + 1);
  }
  stringQuickSortImpl(arr, gt + 1, hi, d);
}

void stringQuickSort(std::vector<std::string> &arr) {
  if (!arr.empty()) {
    stringQuickSortImpl(arr, 0, arr.size() - 1, 0);
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);
  int n;
  if (!(std::cin >> n))
    return 0;
  std::vector<std::string> arr(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> arr[i];
  }
  stringQuickSort(arr);
  for (int i = 0; i < n; ++i) {
    std::cout << arr[i] << "\n";
  }

  return 0;
}