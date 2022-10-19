#include <iostream>

struct b10_conversion_info {
  int number;
  int maximum_number;
};

enum functions {
  de_jong = 1,
  schwefels,
  rastrigins,
  michalewiczs
};

enum improvement_type {
  best = 1,
  first,
  worst
};

struct function_computation {
  double result;
  bool is_ok;
};