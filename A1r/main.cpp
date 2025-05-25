#include <iostream>
#include <vector>

const int R = 256;

void msdRadixSort(std::vector<std::string> &arr, int lo, int hi, int d, std::vector<std::string> &aux) {
  if (hi <= lo) {
    return;
  }

  std::vector<int> count(R + 2, 0);
  for (int i = lo; i <= hi; ++i) {
    int c = (static_cast<size_t>(d) < arr[i].length()) ? (unsigned char) arr[i][d] + 1 : 0;
    count[c + 1]++;
  }

  for (int r = 0; r < R + 1; ++r) {
    count[r + 1] += count[r];
  }

  for (int i = lo; i <= hi; ++i) {
    int c = (static_cast<size_t>(d) < arr[i].length()) ? (unsigned char) arr[i][d] + 1 : 0;
    aux[count[c]++] = arr[i];
  }

  for (int i = lo; i <= hi; ++i) {
    arr[i] = aux[i - lo];
  }

  for (int r = 0; r < R; ++r) {
    msdRadixSort(arr, lo + count[r], lo + count[r + 1] - 1, d + 1, aux);
  }
}

int main() {
  int n;
  std::cin >> n;
  std::cin.ignore();

  std::vector<std::string> strings(n);

  for (int i = 0; i < n; ++i) {
    std::getline(std::cin, strings[i]);
  }

  std::vector<std::string> aux(strings.size());
  msdRadixSort(strings, 0, n - 1, 0, aux);

  for (const auto &str : strings) {
    std::cout << str << std::endl;
  }

  return 0;
}