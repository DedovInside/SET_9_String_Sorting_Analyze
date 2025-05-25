#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <fstream>

unsigned long long char_comparisons = 0;

int custom_compare(const std::string &a, const std::string &b) {
  for (size_t i = 0; i < a.size() && i < b.size(); ++i) {
    ++char_comparisons;
    if (a[i] != b[i]) {
      return a[i] < b[i] ? -1 : 1;
    }
  }
  if (a.size() < b.size()) return -1;
  if (a.size() > b.size()) return 1;
  return 0;
}

// Адаптер для custom_compare для использования с std::sort
bool custom_compare_adapter(const std::string &a, const std::string &b) {
  return custom_compare(a, b) < 0;  // Адаптация int к bool
}

class StringGenerator {
 private:
  std::mt19937 rng;
  const std::string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%:;^&*()-";
  const int charset_size = 74;

 public:
  StringGenerator() {
    rng.seed(std::random_device()());
  }

  std::string generate_random_string(int min_len, int max_len) {
    std::uniform_int_distribution<int> len_dist(min_len, max_len);
    std::uniform_int_distribution<int> char_dist(0, charset_size - 1);
    int len = len_dist(rng);
    std::string s;
    for (int i = 0; i < len; ++i) {
      s += charset[char_dist(rng)];
    }
    return s;
  }

  std::vector<std::string> generate_random_array(int size, int min_len, int max_len) {
    std::vector<std::string> arr(size);
    for (int i = 0; i < size; ++i) {
      arr[i] = generate_random_string(min_len, max_len);
    }
    return arr;
  }

  std::vector<std::string> generate_reverse_sorted_array(int size, int min_len, int max_len) {
    auto arr = generate_random_array(size, min_len, max_len);
    std::sort(arr.begin(), arr.end(), custom_compare_adapter);
    std::reverse(arr.begin(), arr.end());
    return arr;
  }

  std::vector<std::string> generate_almost_sorted_array(int size, int min_len, int max_len, int swaps = 50) {
    auto arr = generate_random_array(size, min_len, max_len);
    std::sort(arr.begin(), arr.end(), custom_compare_adapter);
    std::uniform_int_distribution<int> idx_dist(0, size - 1);
    for (int i = 0; i < swaps; ++i) {
      int idx1 = idx_dist(rng), idx2 = idx_dist(rng);
      std::swap(arr[idx1], arr[idx2]);
    }
    return arr;
  }

  std::vector<std::string> generate_common_prefix_array(int size, int min_len, int max_len, int prefix_len) {
    std::string prefix = generate_random_string(prefix_len, prefix_len);
    std::vector<std::string> arr(size);
    for (int i = 0; i < size; ++i) {
      arr[i] = prefix + generate_random_string(min_len - prefix_len, max_len - prefix_len);
    }
    return arr;
  }
};

// Standard Merge Sort
void standard_merge(std::vector<std::string> &arr, int left, int mid, int right) {
  std::vector<std::string> temp(right - left + 1);
  int i = left, j = mid + 1, k = 0;
  while (i <= mid && j <= right) {
    if (custom_compare(arr[i], arr[j]) < 0) {
      temp[k++] = arr[i++];
    } else {
      temp[k++] = arr[j++];
    }
  }
  while (i <= mid) temp[k++] = arr[i++];
  while (j <= right) temp[k++] = arr[j++];
  for (i = left, k = 0; i <= right; ++i, ++k) arr[i] = temp[k];
}

void standard_merge_sort(std::vector<std::string> &arr, int left, int right) {
  if (left < right) {
    int mid = left + (right - left) / 2;
    standard_merge_sort(arr, left, mid);
    standard_merge_sort(arr, mid + 1, right);
    standard_merge(arr, left, mid, right);
  }
}

// String Merge Sort с LCP
std::pair<int, int> lcpCompare(const std::string &a, const std::string &b, int k = 0) {
  int n = std::min(a.size(), b.size());
  int i = k;
  while (i < n && a[i] == b[i]) {
    char_comparisons++;
    ++i;
  }
  if (i == n) {
    if (a.size() < b.size()) return {-1, i};
    if (a.size() > b.size()) return {+1, i};
    return {0, i};
  }
  char_comparisons++;
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


// Standard Quick Sort с случайным выбором опорного элемента
void standard_quicksort(std::vector<std::string> &arr, int lo, int hi, std::mt19937 &gen) {
  if (lo >= hi) {
    return;
  }

  std::uniform_int_distribution<> dis(lo, hi);
  int pivot_idx = dis(gen);
  std::swap(arr[lo], arr[pivot_idx]);

  const std::string pivot = arr[lo];  // Использовать константную копию вместо ссылки

  int lt = lo;
  int i = lo + 1;
  int gt = hi;

  while (i <= gt) {
    int cmp = custom_compare(arr[i], pivot);
    if (cmp < 0) {
      std::swap(arr[lt++], arr[i++]);
    } else if (cmp > 0) {
      std::swap(arr[i], arr[gt--]);
    } else {
      i++;
    }
  }

  standard_quicksort(arr, lo, lt - 1, gen);
  standard_quicksort(arr, gt + 1, hi, gen);
}

// String Quick Sort
int compareCharWithPivot(const std::string &s, int d, char pivot) {
  if (static_cast<size_t>(d) >= s.length()) {
    ++char_comparisons;
    return pivot == '\0' ? 0 : -1;
  }

  ++char_comparisons;
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

void stringQuickSort(std::vector<std::string> &arr, int lo, int hi, int d, std::mt19937 &gen) {

  if (hi <= lo) {
    return;
  }

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

  stringQuickSort(arr, lo, lt - 1, d, gen);
  if (pivot != '\0') {
    stringQuickSort(arr, lt, gt, d + 1, gen);
  }
  stringQuickSort(arr, gt + 1, hi, d, gen);

}

// MSD Radix Sort
void msdRadixSort(std::vector<std::string> &arr,
                  int lo,
                  int hi,
                  int d,
                  std::vector<std::string> &aux,
                  std::mt19937 &gen,
                  bool useQuickSort) {

  if (useQuickSort && hi - lo <= 74) {
    stringQuickSort(arr, lo, hi, d, gen);
    return;
  }

  if (hi <= lo) {
    return;
  }

  std::vector<int> count(258, 0);
  for (int i = lo; i <= hi; ++i) {
    int c = (static_cast<size_t>(d) < arr[i].length()) ? (unsigned char) arr[i][d] + 1 : 0;
    ++char_comparisons;
    count[c + 1]++;
  }

  for (int r = 0; r < 256 + 1; ++r) {
    count[r + 1] += count[r];
  }

  for (int i = lo; i <= hi; ++i) {
    int c = (static_cast<size_t>(d) < arr[i].length()) ? (unsigned char) arr[i][d] + 1 : 0;
    aux[count[c]++] = arr[i];
  }

  for (int i = lo; i <= hi; ++i) {
    arr[i] = aux[i - lo];
  }

  for (int r = 0; r < 256; ++r) {
    msdRadixSort(arr, lo + count[r], lo + count[r + 1] - 1, d + 1, aux, gen, useQuickSort);
  }
}

class StringSortTester {
 private:
  StringGenerator generator;
  std::mt19937 rng;

  struct TestResult {
    double time_ms;
    unsigned long long comparisons;
  };

  TestResult run_test_merge(std::vector<std::string> arr, void (*sort_func)(std::vector<std::string> &, int, int)) {
    char_comparisons = 0;
    auto start = std::chrono::high_resolution_clock::now();
    sort_func(arr, 0, arr.size() - 1);
    auto end = std::chrono::high_resolution_clock::now();
    double time_ms = std::chrono::duration<double, std::milli>(end - start).count();
    return {time_ms, char_comparisons};
  }

  TestResult run_test_string_merge(std::vector<std::string> arr,
                                   void (*sort_func)(std::vector<std::string> &,
                                                     std::vector<std::string> &,
                                                     int,
                                                     int)) {
    char_comparisons = 0;
    std::vector<std::string> temp(arr.size());
    auto start = std::chrono::high_resolution_clock::now();
    sort_func(arr, temp, 0, arr.size() - 1);
    auto end = std::chrono::high_resolution_clock::now();
    double time_ms = std::chrono::duration<double, std::milli>(end - start).count();
    return {time_ms, char_comparisons};
  }

  TestResult run_test_quicksort(std::vector<std::string> arr) {
    char_comparisons = 0;
    auto start = std::chrono::high_resolution_clock::now();
    standard_quicksort(arr, 0, arr.size() - 1, rng);
    auto end = std::chrono::high_resolution_clock::now();
    double time_ms = std::chrono::duration<double, std::milli>(end - start).count();
    return {time_ms, char_comparisons};
  }

  TestResult run_test_string_quicksort(std::vector<std::string> arr) {
    char_comparisons = 0;
    auto start = std::chrono::high_resolution_clock::now();
    stringQuickSort(arr, 0, arr.size() - 1, 0, rng);
    auto end = std::chrono::high_resolution_clock::now();
    double time_ms = std::chrono::duration<double, std::milli>(end - start).count();
    return {time_ms, char_comparisons};
  }

  TestResult run_test_radix_sort(std::vector<std::string> arr, bool use_quicksort) {
    char_comparisons = 0;
    std::vector<std::string> aux(arr.size());
    auto start = std::chrono::high_resolution_clock::now();
    msdRadixSort(arr, 0, arr.size() - 1, 0, aux, rng, use_quicksort);
    auto end = std::chrono::high_resolution_clock::now();
    double time_ms = std::chrono::duration<double, std::milli>(end - start).count();
    return {time_ms, char_comparisons};
  }

 public:
  StringSortTester() : rng(std::random_device()()) {}

  void test_algorithms(const std::string &output_file) {
    std::ofstream out(output_file);
    out << "size,type,algorithm,time_ms,comparisons\n";

    for (int size = 100; size <= 3000; size += 100) {
      auto random_arr = generator.generate_random_array(3000, 10, 200);
      auto reverse_arr = generator.generate_reverse_sorted_array(3000, 10, 200);
      auto almost_arr = generator.generate_almost_sorted_array(3000, 10, 200);
      auto prefix_arr = generator.generate_common_prefix_array(3000, 10, 200, 50);

      std::vector<std::vector<std::string>> arrays = {
          std::vector<std::string>(random_arr.begin(), random_arr.begin() + size),
          std::vector<std::string>(reverse_arr.begin(), reverse_arr.begin() + size),
          std::vector<std::string>(almost_arr.begin(), almost_arr.begin() + size),
          std::vector<std::string>(prefix_arr.begin(), prefix_arr.begin() + size)
      };
      std::vector<std::string> types = {"random", "reverse", "almost", "prefix"};
      std::vector<std::string> algorithms =
          {"quick_sort", "merge_sort", "string_quicksort", "string_mergesort", "msd_radix_sort",
           "msd_radix_sort_switch"};

      for (size_t t = 0; t < arrays.size(); ++t) {
        for (const auto &algo : algorithms) {
          std::vector<TestResult> results(10);
          for (int i = 0; i < 10; ++i) {
            if (algo == "quick_sort") {
              results[i] = run_test_quicksort(arrays[t]);
            } else if (algo == "merge_sort") {
              results[i] = run_test_merge(arrays[t], standard_merge_sort);
            } else if (algo == "string_quicksort") {
              results[i] = run_test_string_quicksort(arrays[t]);
            } else if (algo == "string_mergesort") {
              results[i] = run_test_string_merge(arrays[t], stringMergeSort);
            } else if (algo == "msd_radix_sort") {
              results[i] = run_test_radix_sort(arrays[t], false);
            } else {
              results[i] = run_test_radix_sort(arrays[t], true);
            }
          }
          double avg_time = 0, avg_comps = 0;
          for (const auto &res : results) {
            avg_time += res.time_ms / 10.0;
            avg_comps += res.comparisons / 10.0;
          }
          out << size << "," << types[t] << "," << algo << "," << avg_time << "," << avg_comps << "\n";
        }
      }
    }
    out.close();
  }
};

int main() {
  StringSortTester tester;
  tester.test_algorithms("sorts_results.csv");
  return 0;
}