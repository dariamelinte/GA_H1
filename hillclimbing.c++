#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <thread>
#include <iomanip>
#include <limits>
#include <string>
#include <fstream>

#define M_PI 3.14159265358979323846

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
  bool is_ok = false;
  std::vector<double> vect;
};

std::mt19937 random_gen(time(0));

double lowest_double = std::numeric_limits<double>::lowest();
double highest_double = std::numeric_limits<double>::max();

functions FUNCTION;
improvement_type IMPROVEMENT;

double LOW;
double HIGH;

int BIT_STRING_LEN;
int LEN;

int PRECISION = 5;
int NUMBER_OF_DIMENSIONS;

void compute_filename(std::string& filename) {

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

  filename += "dimensions-";
  filename += std::to_string(NUMBER_OF_DIMENSIONS);
  filename += ".txt";
}

double de_jongs_function(std::vector<double>& x) {
    int dimension = x.size();
    double f_x = 10 * dimension;
    for (int facet = 0; facet < dimension; facet++) {
        double num = x[facet];
        f_x += num * num - 10 * cos(2 * M_PI * num);
    }
    return f_x;
}

double rastrigins_function(std::vector<double>& x) {
    int dimension = x.size();
    double f_x = 10 * dimension;
    for (int facet = 0; facet < dimension; facet++) {
        double num = x[facet];
        f_x += num * num - 10 * cos(2 * M_PI * num);
    }
    return f_x;
}

double schwefels_function(std::vector<double>& x) {
    int dimension = x.size();
    double f_x = 0;
    for (int facet = 0; facet < dimension; facet++) {
        double num = x[facet];
        f_x -= num * sin(sqrt(abs(num)));
    }
    return f_x;
}

double michalewiczs_function(std::vector<double>& x) {
    int dimension = x.size();
    double f_x = 0;
    for (int facet = 0; facet < dimension; facet++) {
        double num = x[facet];
        f_x -= sin(num) * (double)pow(sin((double)(facet + 1) * num * num / M_PI), 20);
    }

    return f_x;
}

void run_given_function(std::vector<double>& candidate, function_computation& comp ) {
  switch (FUNCTION) {
    case functions::de_jong:
      comp.result = de_jongs_function(candidate);
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
      comp.result = michalewiczs_function(candidate);
      comp.is_ok = true;
      break;
    default:
      break;
  }
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

double generate_random_number(b10_conversion_info& info) {
  return LOW + info.number * (HIGH - LOW) / info.maximum_number;
}

void generate_bit_string(std::vector<bool>& bit_string) {
  for (int index = 0; index < LEN; index++) {
    bit_string.push_back(random_gen() % 2);
  }
}

void convert_bit_string_to_b10_vector(std::vector<bool>& bit_string, std::vector<double>& b10_vector) {
  int poz = 0;
  for (int i = 0; i < LEN; i += BIT_STRING_LEN) {
    int x_i = 0;
    for (int j = i; j < i + BIT_STRING_LEN; ++j) {
      x_i *= 2;
      x_i += bit_string[j];
    }
    double value = (double)x_i / (pow(2.0, BIT_STRING_LEN) - 1.0);
    b10_vector.push_back(LOW + value * (HIGH - LOW));
  }
}


void first_improvement(std::vector<bool>& bit_string) {
  std::vector<double> b10_vector;
  convert_bit_string_to_b10_vector(bit_string, b10_vector);
  function_computation comp;
  comp.is_ok = false;
  run_given_function(b10_vector, comp);  

  if (!comp.is_ok) {
    // std::cout << "could not compute function: " << FUNCTION << '\n' << '\n';
    return;
  }

  for (int index = 0; index < LEN; index++) {
    bit_string[index] = !bit_string[index];

    std::vector<double> new_b10_vector;
    convert_bit_string_to_b10_vector(bit_string, new_b10_vector);
 
    function_computation new_comp;
    new_comp.is_ok = false;
    
    run_given_function(new_b10_vector, new_comp);  

    if (!new_comp.is_ok) {
      // std::cout << "could not compute new function: " << FUNCTION << '\n' << '\n';
      return;
    }

    if (new_comp.result < comp.result) {
      return;
    }

    bit_string[index] = !bit_string[index];
  }
}


void best_improvement(std::vector<bool>& bit_string) {
  std::vector<double> b10_vector;
  convert_bit_string_to_b10_vector(bit_string, b10_vector);
  function_computation comp;
  comp.is_ok = false;
  run_given_function(b10_vector, comp);  

  if (!comp.is_ok) {
    // std::cout << "could not compute function: " << FUNCTION << '\n' << '\n';
    return;
  }

  int best_index = -1;

  for (int index = 0; index < LEN; index++) {
    bit_string[index] = !bit_string[index];

    std::vector<double> new_b10_vector;
    convert_bit_string_to_b10_vector(bit_string, new_b10_vector);

    function_computation new_comp;  
    new_comp.is_ok = false;
    run_given_function(new_b10_vector, new_comp);  

    if (!new_comp.is_ok) {
      // std::cout << "could not compute new function: " << FUNCTION << '\n' << '\n';
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
  std::vector<double> b10_vector;
  convert_bit_string_to_b10_vector(bit_string, b10_vector);

  function_computation comp;
  comp.is_ok = false;
  run_given_function(b10_vector, comp);  

  if (!comp.is_ok) {
    // std::cout << "could not compute function: " << FUNCTION << '\n' << '\n';
    return;
  }

  double lowest = lowest_double;
  int best_index = -1;

  for (int index = 0; index < LEN; index++) {
    bit_string[index] = !bit_string[index];

    std::vector<double> new_b10_vector;
    convert_bit_string_to_b10_vector(bit_string, new_b10_vector);

    function_computation new_comp;
    new_comp.is_ok = false;
    run_given_function(new_b10_vector, new_comp);  

    if (!new_comp.is_ok) {
      // std::cout << "could not compute new function: " << FUNCTION << '\n' << '\n';
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

void hillclimbing(function_computation& comp) {
  int maxi_r;
  if (NUMBER_OF_DIMENSIONS == 5) {
      // 5 dimensiuni
      maxi_r = IMPROVEMENT == improvement_type::first ? 7500 : 2500;
  }

  if (NUMBER_OF_DIMENSIONS == 10) {
      // 10 dimensiuni
      maxi_r = IMPROVEMENT == improvement_type::first ? 5000 : 1000;
  }

  if (NUMBER_OF_DIMENSIONS == 30) {
      // 30 dimensiuni
      maxi_r = 300;
  }

  for (int t = 0; t < maxi_r; t++) {
    std::cout << t << std:: endl;
    bool local = false;

    std::vector<bool> candidate_bit_string;
    generate_bit_string(candidate_bit_string);
    std::vector<double> candidate_b10_vector;
    convert_bit_string_to_b10_vector(candidate_bit_string, candidate_b10_vector);

    function_computation candidate_comp;
    candidate_comp.is_ok = false;
    run_given_function(candidate_b10_vector, candidate_comp);
    candidate_comp.vect = candidate_b10_vector;

    if (!candidate_comp.is_ok) {
      // std::cout << "could not compute candidate function: " << FUNCTION << '\n' << '\n';
      return;
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

      std::vector<double> nbh_b10_vector;
      convert_bit_string_to_b10_vector(nbh_bit_string, nbh_b10_vector);

      function_computation nbh_comp;
      nbh_comp.is_ok = false;
      run_given_function(nbh_b10_vector, nbh_comp);
      nbh_comp.vect = nbh_b10_vector;
      
      if (!nbh_comp.is_ok) {
        // std::cout << "could not compute nbh function: " << FUNCTION << '\n' << '\n';
        return;
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
}

void compile (int max_ind) {
  std::string output_filename = "./rapoarte/";
  compute_filename(output_filename);

  // auto start = std::chrono::high_resolution_clock::now();


  set_bounds();
  BIT_STRING_LEN = ceil(log2(pow(10, PRECISION) * (HIGH - LOW)));
  LEN = NUMBER_OF_DIMENSIONS * BIT_STRING_LEN;
  // std::cout << std::setprecision(PRECISION) << std::fixed;

  for (int index = 1; index <= max_ind;) {
    std::ofstream out_file;
    out_file.open(output_filename, std::ios_base::app);
  
    out_file << std::setprecision(PRECISION) << std::fixed;
  
    auto start_hill = std::chrono::high_resolution_clock::now();

    function_computation comp;
    comp.result = highest_double;
    comp.is_ok = false;

    hillclimbing(comp);

    if (comp.is_ok) {
      out_file << index << " ------  [ ";
      // std::cout << index << " ------  [ ";

      for (double el : comp.vect) {
        out_file << el << " ";
        // std::cout << el << " "; 
      }

      out_file << " ]  ------ result: " << comp.result << " ----- ";
      // std::cout << " ]  ------ result: " << comp.result << " ----- ";

      auto stop_hill = std::chrono::high_resolution_clock::now();

      std::chrono::duration<double, std::milli> time_hill = stop_hill - start_hill;
      out_file << " time: " << time_hill.count() / 1000;
      // std::cout << " time: " << time_hill.count() / 1000;

      out_file << '\n' << '\n';
      // std::cout << '\n' << '\n';

      index++;
    }

    out_file.close();
  }
}

int main () {
  srand(time(0));

  NUMBER_OF_DIMENSIONS = 5;


  // -------------------
  IMPROVEMENT = improvement_type::first;
  // -------------------
  FUNCTION = functions::de_jong;
  compile(30);

  FUNCTION = functions::rastrigins;
  compile(30);

  FUNCTION = functions::schwefels;
  compile(30);



  // -------------------
  IMPROVEMENT = improvement_type::best;
  // -------------------
  FUNCTION = functions::de_jong;
  compile(30);

  FUNCTION = functions::rastrigins;
  compile(30);

  FUNCTION = functions::schwefels;
  compile(30);



  // -------------------
  IMPROVEMENT = improvement_type::worst;
  // -------------------
  FUNCTION = functions::de_jong;
  compile(30);

  FUNCTION = functions::rastrigins;
  compile(30);

  FUNCTION = functions::schwefels;
  compile(30);




  NUMBER_OF_DIMENSIONS = 10;


  // -------------------
  IMPROVEMENT = improvement_type::first;
  // -------------------
  FUNCTION = functions::de_jong;
  compile(30);

  FUNCTION = functions::rastrigins;
  compile(30);

  FUNCTION = functions::schwefels;
  compile(30);



  // -------------------
  IMPROVEMENT = improvement_type::best;
  // -------------------
  FUNCTION = functions::de_jong;
  compile(30);

  FUNCTION = functions::rastrigins;
  compile(30);

  FUNCTION = functions::schwefels;
  compile(30);



  // -------------------
  IMPROVEMENT = improvement_type::worst;
  // -------------------
  FUNCTION = functions::de_jong;
  compile(30);

  FUNCTION = functions::rastrigins;
  compile(30);

  FUNCTION = functions::schwefels;
  compile(30);




  NUMBER_OF_DIMENSIONS = 30;

  

  // -------------------
  IMPROVEMENT = improvement_type::first;
  // -------------------
  FUNCTION = functions::de_jong;
  compile(30);

  FUNCTION = functions::rastrigins;
  compile(30);

  FUNCTION = functions::schwefels;
  compile(30);



  // -------------------
  IMPROVEMENT = improvement_type::best;
  // -------------------
  FUNCTION = functions::de_jong;
  compile(30);

  FUNCTION = functions::rastrigins;
  compile(30);

  FUNCTION = functions::schwefels;
  compile(30);



  // -------------------
  IMPROVEMENT = improvement_type::worst;
  // -------------------
  FUNCTION = functions::de_jong;
  compile(30);

  FUNCTION = functions::rastrigins;
  compile(30);

  FUNCTION = functions::schwefels;
  compile(30);


  
  return 0;
}