#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

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
  while (i < n && a[i] == b[i]) {
    i++;
  }
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

vector<StringItem> stringMergeSort(const vector<StringItem> &arr) {
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
  left = stringMergeSort(left);
  right = stringMergeSort(right);
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

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  int n;
  if (!(cin >> n))
    return 0;
  vector<StringItem> arr(n);
  for (int i = 0; i < n; ++i) {
    cin >> arr[i].s;
    arr[i].lcp = 0;
  }
  vector<StringItem> sorted_arr = stringMergeSort(arr);
  for (int i = 0; i < n; ++i) {
    cout << sorted_arr[i].s << "\n";
  }
  return 0;
}