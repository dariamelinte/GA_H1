#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <thread>
#include <iomanip>

std::mt19937 random_gen(time(0));
double generate_random_number(b10_conversion_info info,  double low, double high) {
  return low + static_cast<double>(info.number) * static_cast<double>(high - low) / info.maximum_number;
}

bool generate_random_bit() {
  return random_gen() % 2;
}

std::vector<bool> generate_bit_string(double low, double high, int precision, int number_of_dimensions) {
  double interval = high - low;
  long long int power = 1;

  std::vector<bool> bit_string;

  for (int index = 0; index < precision; index++) {
    power *= 10;
  }

  int bit_string_len = ceil(log2(power * interval)); //lungimea unui sir de biti de precizie 'precision'
  int len = number_of_dimensions * bit_string_len; //calculeaza lungimea unui sir de biti pe un anumit numar de dimensiuni 'dimensions'

  for (int index = 0; index < len; index++) {
    bit_string.push_back(generate_random_bit());
  }

  return bit_string;
}

b10_conversion_info convert_b2_to_b10(std::vector<bool> bit_string) {

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

  return info;
}


int main () {
  srand(time(0));

  for (int i = 0; i < 100; i++) {
    std::vector<bool> vect = generate_bit_string(-5.12, 5.12, 5, 1);

    // for (bool element : vect) {
    //   std::cout << element << " ";
    // }

    // std::cout << std::endl << std::endl << vect.size() << std::endl << std::endl;

    b10_conversion_info info = convert_b2_to_b10(vect);

    // std::cout << info.number << " " << info.maximum_number << std::endl << std::endl;


    std::cout << std::setprecision(5) << generate_random_number(info, -5.12, 5.12) << " ";
  }

  std::cout << std::endl;
}