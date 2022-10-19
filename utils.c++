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

struct function_computation {
  double result;
  bool is_ok;
};