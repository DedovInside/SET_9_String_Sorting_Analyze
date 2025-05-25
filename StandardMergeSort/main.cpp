#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <string>

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

// Генератор случайных строк
std::string random_string(std::mt19937& gen, int min_len, int max_len) {
  std::uniform_int_distribution<> len_dist(min_len, max_len);
  std::uniform_int_distribution<> char_dist(97, 122); // a-z

  int length = len_dist(gen);
  std::string result;
  result.reserve(length);

  for (int i = 0; i < length; ++i) {
    result.push_back(static_cast<char>(char_dist(gen)));
  }

  return result;
}

// Генерация тестовых массивов
std::vector<std::string> generate_random_array(std::mt19937& gen, int size, int min_len, int max_len) {
  std::vector<std::string> arr(size);
  for (int i = 0; i < size; ++i) {
    arr[i] = random_string(gen, min_len, max_len);
  }
  return arr;
}

std::vector<std::string> generate_sorted_array(std::mt19937& gen, int size, int min_len, int max_len) {
  auto arr = generate_random_array(gen, size, min_len, max_len);
  std::sort(arr.begin(), arr.end());
  return arr;
}

std::vector<std::string> generate_reverse_array(std::mt19937& gen, int size, int min_len, int max_len) {
  auto arr = generate_sorted_array(gen, size, min_len, max_len);
  std::reverse(arr.begin(), arr.end());
  return arr;
}

int main() {
  std::random_device rd;
  std::mt19937 gen(rd());
  const int NUM_TESTS = 50;
  const int ARRAY_SIZE = 100;
  const int MIN_LEN = 5;
  const int MAX_LEN = 20;

  bool all_correct = true;

  // Тестирование на случайных массивах
  for (int i = 0; i < NUM_TESTS; ++i) {
    auto test_array = generate_random_array(gen, ARRAY_SIZE, MIN_LEN, MAX_LEN);
    auto std_array = test_array;

    std::sort(std_array.begin(), std_array.end());
    standard_merge_sort(test_array, 0, test_array.size() - 1);

    if (test_array != std_array) {
      all_correct = false;
      break;
    }
  }

  // Тестирование на отсортированных массивах
  for (int i = 0; i < NUM_TESTS && all_correct; ++i) {
    auto test_array = generate_sorted_array(gen, ARRAY_SIZE, MIN_LEN, MAX_LEN);
    auto std_array = test_array;

    standard_merge_sort(test_array, 0, test_array.size() - 1);

    if (test_array != std_array) {
      all_correct = false;
      break;
    }
  }

  // Тестирование на обратно отсортированных массивах
  for (int i = 0; i < NUM_TESTS && all_correct; ++i) {
    auto test_array = generate_reverse_array(gen, ARRAY_SIZE, MIN_LEN, MAX_LEN);
    auto std_array = test_array;

    std::sort(std_array.begin(), std_array.end());
    standard_merge_sort(test_array, 0, test_array.size() - 1);

    if (test_array != std_array) {
      all_correct = false;
      break;
    }
  }

  std::cout << (all_correct ? "true" : "false") << std::endl;

  return 0;
}