#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <thread>
#include <iomanip>
#include <limits>
#include <string>
#include <fstream>
#define _USE_MATH_DEFINES

#include "utils.c++"

std::mt19937 random_gen(time(0));

double lowest_double = std::numeric_limits<double>::lowest();
double highest_double = std::numeric_limits<double>::max();

functions FUNCTION;
improvement_type IMPROVEMENT;

double LOW;
double HIGH;

int BIT_STRING_LEN;
int LEN;

int PRECISION;
int NUMBER_OF_DIMENSIONS;

std::string compute_filename() {
  std::string filename = "./rapoarte/";

  switch (FUNCTION) {
    case functions::de_jong:
      filename += "de_jong___";
      break;
    case functions::schwefels:
      filename += "schwefels___";
      break;
    case functions::rastrigins:
      filename += "rastrigins___";
      break;
    case functions::michalewiczs:
      filename += "michalewiczs___";
      // TODO: CREATE FUNCTION
      break;
    default:
      break;
  }

  switch (IMPROVEMENT) {
    case improvement_type::best:
      filename += "best-improvement___";
      break;
    case improvement_type::first:
      filename += "first-improvement__";
      break;
    case improvement_type::worst:
      filename += "worst-improvement__";
      break;
    default:
      break;
  }

  filename += "precision-";
  filename += std::to_string(PRECISION);
  filename += "___";

  filename += "dimensions-";
  filename += std::to_string(NUMBER_OF_DIMENSIONS);
  filename += "___";

  int64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

  filename += std::to_string(timestamp);
  filename += ".txt";

  return filename;
}

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

double rastrigins_function (std::vector<double> x) {
  int dimension = x.size();
  double f_x = 10 * dimension;
  for (int facet = 0; facet < dimension; facet++) {
    f_x += x[facet] * x[facet] - 10 * cos(2 * M_PI * x[facet]);
  }
  return f_x;
}

function_computation run_given_function(std::vector<double> candidate) {
  function_computation comp;

  comp.is_ok = false;

  switch (FUNCTION) {
    case functions::de_jong:
      comp.result = de_jong_function(candidate);
      comp.is_ok = true;
      break;
    case functions::schwefels:
      comp.result = schwefels_function(candidate);
      comp.is_ok = true;
      break;
    case functions::rastrigins:
      comp.result = rastrigins_function(candidate);
      comp.is_ok = true;
      break;
    case functions::michalewiczs:
      // TODO: CREATE FUNCTION
      break;
    default:
      break;
  }

  return comp;
}

void set_bounds() {
  switch (FUNCTION) {
    case functions::de_jong:
      LOW = -5.12;
      HIGH = 5.12;
      break;
    case functions::schwefels:
      LOW = -500;
      HIGH = 500;
      break;
    case functions::rastrigins:
      LOW = -5.12;
      HIGH = 5.12;
      break;
    case functions::michalewiczs:
      LOW = 0;
      HIGH = M_PI;
      break;
    default:
      break;
  }
}

double generate_random_number(b10_conversion_info info) {
  return LOW + info.number * (HIGH - LOW) / info.maximum_number;
}

std::vector<bool> generate_bit_string() {
  std::vector<bool> bit_string;

  for (int index = 0; index < LEN; index++) {
    bit_string.push_back(random_gen() % 2);
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

std::vector<double> convert_bit_string_to_b10_vector(std::vector<bool> bit_string) {
  std::vector<double> b10_vector;
  std::vector<bool> mini_bit_string;

  for (int dimension = 0; dimension < NUMBER_OF_DIMENSIONS; dimension++) {
    for (int bit_i = 0; bit_i < BIT_STRING_LEN; bit_i++) {
      int index = bit_i + BIT_STRING_LEN * dimension;
      bool bit = bit_string[index];
      mini_bit_string.push_back(bit);
    }

    double number = generate_random_number(convert_b2_to_b10(mini_bit_string));

    b10_vector.push_back(number);
    mini_bit_string.clear();
  }

  return b10_vector;
}


void first_improvement(std::vector<bool>& bit_string) {
  std::vector<double> b10_vector = convert_bit_string_to_b10_vector(bit_string);
  function_computation comp = run_given_function(b10_vector);  

  if (!comp.is_ok) {
    std::cout << "could not compute function: " << FUNCTION << '\n' << '\n';
    return;
  }

  for (int index = 0; index < LEN; index++) {
    bit_string[index] = !bit_string[index];

    std::vector<double> new_b10_vector = convert_bit_string_to_b10_vector(bit_string);
 
    function_computation new_comp = run_given_function(new_b10_vector);  

    if (!new_comp.is_ok) {
      std::cout << "could not compute new function: " << FUNCTION << '\n' << '\n';
      return;
    }

    if (new_comp.result < comp.result) {
      return;
    }

    bit_string[index] = !bit_string[index];
  }
}


void best_improvement(std::vector<bool>& bit_string) {
  std::vector<double> b10_vector = convert_bit_string_to_b10_vector(bit_string);
  function_computation comp = run_given_function(b10_vector);  

  if (!comp.is_ok) {
    std::cout << "could not compute function: " << FUNCTION << '\n' << '\n';
    return;
  }

  int best_index = -1;

  for (int index = 0; index < LEN; index++) {
    bit_string[index] = !bit_string[index];

    std::vector<double> new_b10_vector = convert_bit_string_to_b10_vector(bit_string);

    function_computation new_comp = run_given_function(new_b10_vector);  

    if (!new_comp.is_ok) {
      std::cout << "could not compute new function: " << FUNCTION << '\n' << '\n';
      return;
    }

    if (new_comp.result < comp.result) {
      comp.result = new_comp.result;
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

  function_computation comp = run_given_function(b10_vector);  

  if (!comp.is_ok) {
    std::cout << "could not compute function: " << FUNCTION << '\n' << '\n';
    return;
  }

  double lowest = lowest_double;
  int best_index = -1;

  for (int index = 0; index < LEN; index++) {
    bit_string[index] = !bit_string[index];

    std::vector<double> new_b10_vector = convert_bit_string_to_b10_vector(bit_string);

    function_computation new_comp = run_given_function(new_b10_vector);  

    if (!new_comp.is_ok) {
      std::cout << "could not compute new function: " << FUNCTION << '\n' << '\n';
      return;
    }

    if (new_comp.result < comp.result && new_comp.result > lowest) {
      lowest = new_comp.result;
      best_index = index;
    }

    bit_string[index] = !bit_string[index];
  }

  if (best_index != -1) {
    bit_string[best_index] = !bit_string[best_index];
  }
}

function_computation hillclimbing() {
  function_computation comp;

  comp.result = highest_double;
  comp.is_ok = false;


  for (int t = 0; t < 1500; t++) {
    bool local = false;

    std::vector<bool> candidate_bit_string = generate_bit_string();
    std::vector<double> candidate_b10_vector = convert_bit_string_to_b10_vector(candidate_bit_string);

    function_computation candidate_comp = run_given_function(candidate_b10_vector);
    candidate_comp.vect = candidate_b10_vector;

    if (!candidate_comp.is_ok) {
      std::cout << "could not compute candidate function: " << FUNCTION << '\n' << '\n';
      return comp;
    }

    do {
      std::vector<bool> nbh_bit_string = candidate_bit_string;

      if (IMPROVEMENT == improvement_type::best) {
        best_improvement(nbh_bit_string);
      } else if (IMPROVEMENT == improvement_type::first) {
        first_improvement(nbh_bit_string);
      } else if (IMPROVEMENT == improvement_type::worst) {
        worst_improvement(nbh_bit_string);
      }

      std::vector<double> nbh_b10_vector = convert_bit_string_to_b10_vector(nbh_bit_string);

      function_computation nbh_comp = run_given_function(nbh_b10_vector);
      nbh_comp.vect = nbh_b10_vector;
      
      if (!nbh_comp.is_ok) {
        std::cout << "could not compute nbh function: " << FUNCTION << '\n' << '\n';
        return comp;
      }

      if (nbh_comp.result < candidate_comp.result) {
        candidate_comp.result = nbh_comp.result;
        candidate_comp.vect = nbh_comp.vect;
        candidate_bit_string = nbh_bit_string;
      } else {
        local = true;
      }
    } while (!local);

    if (candidate_comp.result < comp.result) {
      comp.result = candidate_comp.result;
      comp.vect = candidate_comp.vect;
    }
  }

  comp.is_ok = true;

  return comp;
}

int main () {
  srand(time(0));

  std::cout << "introduceti numarul functiei pe care vreti sa o rulati" << '\n';
  std::cout << "1 - de jong, 2 - schwefel's, 3 - rastrigins, 4 - michalewiczs" << '\n';

  int func_num;
  std::cin >> func_num;
  std::cout << '\n';
  
  FUNCTION = (functions)(func_num);

  std::cout << "introduceti numarul improvementului pe care vreti sa il folositi" << '\n';
  std::cout << "1 - best, 2 - first, 3 - worst" << '\n';

  int improvement_num;
  std::cin >> improvement_num;
  std::cout << '\n';
  
  IMPROVEMENT = (improvement_type)(improvement_num);


  std::cout << "Introduceti numarul de decimale pe care il va folosi functia pentru precizie: ";
  std::cin >> PRECISION;
  std::cout << '\n';
  

  std::cout << "Introduceti numarul de dimensiuni pe care va fi creata functia: ";
  std::cin >> NUMBER_OF_DIMENSIONS;
  std::cout << '\n';
  
  std::string output_filename = compute_filename();

  auto start = std::chrono::high_resolution_clock::now();


  std::ofstream out_file(output_filename);

  set_bounds();
  BIT_STRING_LEN = ceil(log2(pow(10, PRECISION) * (HIGH - LOW)));
  LEN = NUMBER_OF_DIMENSIONS * BIT_STRING_LEN;

  out_file << std::setprecision(PRECISION) << std::fixed;
  std::cout << std::setprecision(PRECISION) << std::fixed;

  for (int index = 1; index <= 30;) {
    function_computation comp = hillclimbing();

    auto start_hill = std::chrono::high_resolution_clock::now();

    if (comp.is_ok) {
      out_file << index << " ------  [ ";
      std::cout << index << " ------  [ ";

      for (double el : comp.vect) {
        out_file << el << " ";
        std::cout << el << " "; 
      }

      out_file << " ]  ------ result: " << comp.result << " ----- ";
      std::cout << " ]  ------ result: " << comp.result << " ----- ";

      auto stop_hill = std::chrono::high_resolution_clock::now();

      std::chrono::duration<double, std::milli> time_hill = stop_hill - start_hill;
      out_file << " time: " << time_hill.count() / 1000;
      std::cout << " time: " << time_hill.count() / 1000;

      out_file << '\n' << '\n';
      std::cout << '\n' << '\n';

      index++;
    }
  }

  auto stop = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> time = stop - start;
  
  out_file << '\n' << '\n';
  std::cout << '\n' << '\n';
  out_file << "total time: " << time.count() / 1000;
  std::cout << "total time: " << time.count() / 1000;

  out_file << '\n';
  std::cout << std::endl;

  out_file.close();
  return 0;
}