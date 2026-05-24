#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int R = 256;
const int CUTOFF = 74;

inline int charAt(const string &s, int d) {
  if (d < s.length()) {
    return (unsigned char)s[d];
  }
  return -1;
}

void stringQuickSortImpl(vector<string> &arr, int lo, int hi, int d) {
  if (hi <= lo)
    return;
  int lt = lo;
  int gt = hi;
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
  if (v >= 0) {
    stringQuickSortImpl(arr, lt, gt, d + 1);
  }
  stringQuickSortImpl(arr, gt + 1, hi, d);
}

void msdRadixSortImpl(vector<string> &arr, vector<string> &aux, int lo, int hi,
                      int d) {
  if (hi <= lo)
    return;

  if (hi - lo + 1 < CUTOFF) {
    stringQuickSortImpl(arr, lo, hi, d);
    return;
  }
  int count[R + 2] = {0};
  for (int i = lo; i <= hi; i++) {
    int c = charAt(arr[i], d);
    count[c + 2]++;
  }
  for (int r = 0; r < R + 1; r++) {
    count[r + 1] += count[r];
  }
  for (int i = lo; i <= hi; i++) {
    int c = charAt(arr[i], d);
    aux[count[c + 1]++] = arr[i];
  }
  for (int i = lo; i <= hi; i++) {
    arr[i] = aux[i - lo];
  }
  for (int r = 0; r < R; r++) {
    msdRadixSortImpl(arr, aux, lo + count[r], lo + count[r + 1] - 1, d + 1);
  }
}

void msdRadixQuickSort(vector<string> &arr) {
  int n = arr.size();
  if (n <= 1)
    return;
  vector<string> aux(n);
  msdRadixSortImpl(arr, aux, 0, n - 1, 0);
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int n;
  if (!(cin >> n))
    return 0;
  vector<string> arr(n);
  for (int i = 0; i < n; ++i) {
    cin >> arr[i];
  }
  msdRadixQuickSort(arr);
  for (int i = 0; i < n; ++i) {
    cout << arr[i] << "\n";
  }
  return 0;
}