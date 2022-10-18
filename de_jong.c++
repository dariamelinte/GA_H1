#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <thread>
#include <iomanip>

double de_jong_low = -5.12;
double de_jong_high = 5.12;

int de_jong_precision = 5;
int de_jong_number_of_dimensions = 3;

double de_jong_interval = de_jong_high - de_jong_low;

long long int de_jong_power = 1;

for (int index = 0; index < de_jong_precision; index++) {
  de_jong_power *= 10;
}

int de_jong_bit_string_len = ceil(log2(de_jong_power * de_jong_interval)); //lungimea unui sir de biti de precizie 'precision'
int de_jong_len = de_jong_number_of_dimensions * de_jong_bit_string_len; //calculeaza lungimea unui sir de biti pe un anumit numar de dimensiuni 'dimensions'


double de_jong_function (std::vector<double> x) {
  int dimension = x.size();
  double f_x = 0;
  for (int facet = 0; facet < dimension; facet++) {
    f_x += x[facet] * x[facet];
  }
  return f_x;
}

void print_de_jong_function_result (std::vector<double> vect) {
  std::cout << "De Jong ---- v ( ";
  for (double x : vect) {
    std::cout << x << " ";
  }

  std::cout << ") = " << de_jong_function(vect) << std::endl;
}

class DeJongFunc() {
  double low = -5.12;
  double high = 5.12;

  int precision = 5;
  int number_of_dimensions = 3;

public:
}

