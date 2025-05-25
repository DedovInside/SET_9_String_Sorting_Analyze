#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

std::pair<int, int> lcpCompare(const std::string &a, const std::string &b, int k = 0) {
  int n = std::min(a.size(), b.size());
  int i = k;
  while (i < n && a[i] == b[i]) {
    ++i;
  }
  if (i == n) {
    if (a.size() < b.size()) return {-1, i};
    if (a.size() > b.size()) return {+1, i};
    return {0, i};
  }
  return {a[i] < b[i] ? -1 : +1, i};
}

void mergeWithLCP(std::vector<std::string> &arr,
                  std::vector<std::string> &tmp,
                  int left, int mid, int right) {
  int nL = mid - left + 1;
  int nR = right - mid;
  std::vector<int> lcpL(nL, 0), lcpR(nR, 0);

  int i = left, j = mid + 1, k = left;
  while (i <= mid && j <= right) {
    int idxL = i - left;
    int idxR = j - (mid + 1);
    int k0 = std::min(lcpL[idxL], lcpR[idxR]);

    auto [cmp, newLcp] = lcpCompare(arr[i], arr[j], k0);
    if (cmp <= 0) {
      tmp[k++] = std::move(arr[i++]);
      lcpR[idxR] = newLcp;
    } else {
      tmp[k++] = std::move(arr[j++]);
      lcpL[idxL] = newLcp;
    }
  }
  while (i <= mid)   tmp[k++] = std::move(arr[i++]);
  while (j <= right) tmp[k++] = std::move(arr[j++]);

  for (int x = left; x <= right; ++x) {
    arr[x] = std::move(tmp[x]);
  }
}


void stringMergeSort(std::vector<std::string> &arr,
                     std::vector<std::string> &tmp,
                     int left, int right) {
  if (left >= right) return;
  int mid = left + (right - left) / 2;
  stringMergeSort(arr, tmp, left, mid);
  stringMergeSort(arr, tmp, mid + 1, right);
  mergeWithLCP(arr, tmp, left, mid, right);
}

int main() {
  int n;
  std::cin >> n;
  std::cin.ignore();

  std::vector<std::string> a(n);
  for (int i = 0; i < n; ++i) {
    std::getline(std::cin, a[i]);
  }
  std::vector<std::string> tmp(n);
  stringMergeSort(a, tmp, 0, n - 1);

  for (auto &s : a) {
    std::cout << s << "\n";
  }
  return 0;
}
