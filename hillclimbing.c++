#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <thread>
#include <iomanip>
#include <limits>

#include "utils.c++"

std::mt19937 random_gen(time(0));


// de jong: -5.12; 5.12;  minimum point = 0
// schwefels: -500; 500;  minimum point = - number_of_dimensions * 418.9829;


double low = -500;
double high = 500;
double interval = high - low;


int precision = 5;
int number_of_dimensions = 1;


int bit_string_len = ceil(log2(pow(10, precision) * interval)); //lungimea unui sir de biti de precizie 'precision'
int len = number_of_dimensions * bit_string_len; //calculeaza lungimea unui sir de biti pe un anumit numar de dimensiuni 'dimensions'

double lowest_double = std::numeric_limits<double>::lowest();
double highest_double = std::numeric_limits<double>::max();

double de_jong_function (std::vector<double> x) {
  int dimension = x.size();
  double f_x = 0;
  for (int facet = 0; facet < dimension; facet++) {
    f_x += x[facet] * x[facet];
  }
  return f_x;
}

double schwefels_function (std::vector<double> x) {
  int dimension = x.size();
  double f_x = 0;
  for (int facet = 0; facet < dimension; facet++) {
    f_x -= x[facet] * sin(sqrt(abs(x[facet])));
  }
  return f_x;
}

double generate_random_number(b10_conversion_info info,  double low, double high) {
  return low + info.number * (high - low) / info.maximum_number;
}

bool generate_random_bit() {
  return random_gen() % 2;
}

std::vector<bool> generate_bit_string(double low, double high) {
  std::vector<bool> bit_string;

  for (int index = 0; index < len; index++) {
    bit_string.push_back(generate_random_bit());
  }

  return bit_string;
}

b10_conversion_info convert_b2_to_b10(std::vector<bool> bit_string) {
  // std::cout << "mini mini mini bit string: ";

  // for (bool element : bit_string) {
  //   std::cout << element << " ";
  // }
  // std::cout << std::endl << std::endl;

  b10_conversion_info info;

  long long int power = 1;

  info.number = 0;
  info.maximum_number = 0;

  for (int index = bit_string.size() - 1; index >= 0; index--) {
    int bit = static_cast<int>(bit_string[index]);

    info.number += bit * power;
    info.maximum_number += power;
    power *= 2;
  }

  // std::cout << "info: " << info.number << " " << info.maximum_number << std::endl << std::endl;

  return info;
}

std::vector<double> convert_bit_string_to_b10_vector(std::vector<bool> bit_string) {
  std::vector<double> b10_vector;
  std::vector<bool> mini_bit_string;

  // std::cout << std::endl << std::endl << "---------------" << std::endl << std::endl;

  for (int dimension = 0; dimension < number_of_dimensions; dimension++) {
    for (int bit_i = 0; bit_i < bit_string_len; bit_i++) {
      int index = bit_i + bit_string_len * dimension;
      bool bit = bit_string[index];
      mini_bit_string.push_back(bit);
    }

    double number = generate_random_number(convert_b2_to_b10(mini_bit_string), low, high);

    // std::cout << "mini bit string: ";

    // for (bool element : mini_bit_string) {
    //   std::cout << element << " ";
    // }

    // std::cout << std::endl << std::endl << number << std::endl << std::endl;

    b10_vector.push_back(number);
    mini_bit_string.clear();
  }

  return b10_vector;
}


void first_improvement(std::vector<bool>& bit_string) {
  std::vector<double> b10_vector = convert_bit_string_to_b10_vector(bit_string);
  // double de_jong_result = de_jong_function(b10_vector);
  double schwefels_result = schwefels_function(b10_vector);

  for (int index = 0; index < len; index++) {
    bit_string[index] = !bit_string[index];

    std::vector<double> new_b10_vector = convert_bit_string_to_b10_vector(bit_string);
    // double new_de_jong_result = de_jong_function(new_b10_vector);
    double new_schwefels_result = schwefels_function(new_b10_vector);

    // if (new_de_jong_result < de_jong_result) {
    if (new_schwefels_result < schwefels_result) {
      return;
    }

    bit_string[index] = !bit_string[index];
  }
}


void best_improvement(std::vector<bool>& bit_string) {
  std::vector<double> b10_vector = convert_bit_string_to_b10_vector(bit_string);
  // double de_jong_result = de_jong_function(b10_vector);
  double schwefels_result = schwefels_function(b10_vector);

  int best_index = -1;

  for (int index = 0; index < len; index++) {
    bit_string[index] = !bit_string[index];

    std::vector<double> new_b10_vector = convert_bit_string_to_b10_vector(bit_string);
    // double new_de_jong_result = de_jong_function(new_b10_vector);
    double new_schwefels_result = schwefels_function(new_b10_vector);

    // if (new_de_jong_result < de_jong_result) {
    if (new_schwefels_result < schwefels_result) {
      // de_jong_result = new_de_jong_result;
      schwefels_result = new_schwefels_result;
      best_index = index;
    }

    bit_string[index] = !bit_string[index];
  }

  if (best_index != -1) {
    bit_string[best_index] = !bit_string[best_index];
  }
}

void worst_improvement(std::vector<bool>& bit_string) {
  std::vector<double> b10_vector = convert_bit_string_to_b10_vector(bit_string);
  // double de_jong_result = de_jong_function(b10_vector);
  double schwefels_result = schwefels_function(b10_vector);

  double lowest = lowest_double;
  int best_index = -1;

  for (int index = 0; index < len; index++) {
    bit_string[index] = !bit_string[index];

    std::vector<double> new_b10_vector = convert_bit_string_to_b10_vector(bit_string);
    // double new_de_jong_result = de_jong_function(new_b10_vector);
    double new_schwefels_result = schwefels_function(new_b10_vector);

    // if (new_de_jong_result < de_jong_result && new_de_jong_result > lowest) {
    if (new_schwefels_result < schwefels_result && new_schwefels_result > lowest) {
      // lowest = new_de_jong_result;
      lowest = new_schwefels_result;
      best_index = index;
    }

    bit_string[index] = !bit_string[index];
  }

  if (best_index != -1) {
    bit_string[best_index] = !bit_string[best_index];
  }
}

void hillclimbing() {
  double best_number = highest_double;

  for (int t = 0; t < 1000; t++) {
    bool local = false;

    std::vector<bool> candidate_bit_string = generate_bit_string(low, high);
    std::vector<double> candidate_b10_vector = convert_bit_string_to_b10_vector(candidate_bit_string);
    // double candidate_result = de_jong_function(candidate_b10_vector);
    double candidate_result = schwefels_function(candidate_b10_vector);

    do {
      std::vector<bool> nbh_bit_string = candidate_bit_string;
      best_improvement(nbh_bit_string);

      std::vector<double> nbh_b10_vector = convert_bit_string_to_b10_vector(nbh_bit_string);
      // double nbh_result = de_jong_function(nbh_b10_vector);
      double nbh_result = schwefels_function(nbh_b10_vector);

      if (nbh_result < candidate_result) {
        candidate_result = nbh_result;
        candidate_bit_string = nbh_bit_string;
      } else {
        local = true;
      }
    } while (!local);

    if (candidate_result <  best_number) {
      best_number = candidate_result;
    }

    std::cout << "current best number ----------------------- " << best_number << std::endl << std::endl;
  }

  std::cout << "hillclimber result ----------------------- " << best_number << std::endl << std::endl;
}

int main () {
  srand(time(0));

  std::cout << std::setprecision(precision) << std::fixed;

  auto start = std::chrono::high_resolution_clock::now();

  hillclimbing();

  // for (int i = 0; i < 3; i++) {
  //   std::cout << i << " ----------- ";
  //   std::vector<bool> bit_string = generate_bit_string(-5.12, 5.12);

  //   for (bool element : bit_string) {
  //     std::cout << element << " ";
  //   }

  //   std::cout << std::endl << std::endl;

    // b10_conversion_info info = convert_b2_to_b10(bit_string);

    // std::cout << info.number << " " << info.maximum_number << std::endl << std::endl;

    // std::vector<double> b10_vector = convert_bit_string_to_b10_vector(bit_string);


    // for (double element : b10_vector) {
    //   std::cout << std::setprecision(precision) << element << " ";
    // }

    // first_improvement(bit_string);
    // best_improvement(bit_string);
  //   worst_improvement(bit_string);

  //   for (bool element : bit_string) {
  //     std::cout << element << " ";
  //   }
  
  //   std::cout << std::endl << std::endl;
  // }

  auto stop = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> time = stop - start;
  std::cout << " main ---- difference: " << time.count() / 1000;

  std::cout << std::endl;
}