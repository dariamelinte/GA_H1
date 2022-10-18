#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <thread>
#include <iomanip>

#include "utils.c++"

std::mt19937 random_gen(time(0));

double low = -5.12;
double high = 5.12;
double interval = high - low;


int precision = 5;
int number_of_dimensions = 4;


int bit_string_len = ceil(log2(pow(10, precision) * interval)); //lungimea unui sir de biti de precizie 'precision'
int len = number_of_dimensions * bit_string_len; //calculeaza lungimea unui sir de biti pe un anumit numar de dimensiuni 'dimensions'


double generate_random_number(b10_conversion_info info,  double low, double high) {
  return low + static_cast<double>(info.number) * static_cast<double>(high - low) / info.maximum_number;
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


int main () {
  srand(time(0));

  for (int i = 0; i < 100; i++) {
    std::vector<bool> bit_string = generate_bit_string(-5.12, 5.12);

    // for (bool element : bit_string) {
    //   std::cout << element << " ";
    // }

    // std::cout << std::endl << std::endl << bit_string.size() << " " << bit_string_len << std::endl << std::endl;

    b10_conversion_info info = convert_b2_to_b10(bit_string);

    // std::cout << info.number << " " << info.maximum_number << std::endl << std::endl;

    std::vector<double> b10_vector = convert_bit_string_to_b10_vector(bit_string);


    for (double element : b10_vector) {
      std::cout << std::setprecision(precision) << element << " ";
    }
  
    std::cout << std::endl << std::endl;
  }

  std::cout << std::endl;
}