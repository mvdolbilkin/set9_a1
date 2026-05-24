#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int R = 256;

inline int charAt(const string &s, int d) {
  if (d < s.length()) {
    return (unsigned char)s[d];
  }
  return -1;
}

void msdRadixSortImpl(vector<string> &arr, vector<string> &aux, int lo, int hi,
                      int d) {
  if (hi <= lo)
    return;
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

void msdRadixSort(vector<string> &arr) {
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
  msdRadixSort(arr);
  for (int i = 0; i < n; ++i) {
    cout << arr[i] << "\n";
  }
  return 0;
}