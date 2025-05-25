#include <iostream>
#include <vector>
#include <random>

int compareCharWithPivot(const std::string &s, int d, char pivot) {
  if (static_cast<size_t>(d) >= s.length()) {
    return pivot == '\0' ? 0 : -1;
  }

  if (s[d] < pivot) {
    return -1;
  } else if (s[d] > pivot) {
    return 1;
  }

  return 0;
}

void swap(std::vector<std::string> &arr, int i, int j) {
  if (i != j) {
    std::string temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}

void stringQuickSort(std::vector<std::string> &arr, int lo, int hi, int d) {

  if (hi <= lo) {
    return;
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(lo, hi);
  int pivot_idx = dis(gen);

  swap(arr, lo, pivot_idx);
  char pivot = static_cast<size_t>(d) < arr[lo].length() ? arr[lo][d] : '\0';

  int lt = lo;
  int gt = hi;
  int i = lo + 1;

  while (i <= gt) {
    int cmp = compareCharWithPivot(arr[i], d, pivot);

    if (cmp < 0) {
      swap(arr, lt++, i++);
    } else if (cmp > 0) {
      swap(arr, i, gt--);
    } else {
      i++;
    }

  }

  stringQuickSort(arr, lo, lt - 1, d);
  if (pivot != '\0') {
    stringQuickSort(arr, lt, gt, d + 1);
  }
  stringQuickSort(arr, gt + 1, hi, d);

}

int main() {
  int n;
  std::cin >> n;
  std::cin.ignore();

  std::vector<std::string> strings(n);

  for (int i = 0; i < n; ++i) {
    std::getline(std::cin, strings[i]);
  }

  stringQuickSort(strings, 0, n - 1, 0);
  for (const auto &str : strings) {
    std::cout << str << std::endl;
  }

  return 0;
}


