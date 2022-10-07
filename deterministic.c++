#include <iostream>
#include <vector>
#include <random>

double epsilon = 0.00001;

double de_jong_function (std::vector<double> x) {
  int dimension = x.size();
  double f_x = 0;
  std::cout << "dimension " << dimension << std::endl;
  for (int facet = 1; facet <= dimension; facet++) {
    f_x += x[facet] * x[facet]; 
    std::cout << f_x << " ";
  }
  std::cout << std::endl;
  return f_x;
}

void print_de_jong_function_result (std::vector<double> vect) {
  std::cout << "valoarea functiei pentru vectorul (";
  for (double x : vect) {
    std::cout << x << " ";
  }

  std::cout << ") este " << de_jong_function(vect) << std::endl;
}

void recursive_for (int n) {
  if (n == 0) {
    // tb sa fac ceva nu stiu inca ce
  } else {
    n--;
    for (double index = -5.12; index <= 5.12; index += epsilon) {
      recursive_for(n);
    }
  }
}


double generate_random_number(double high, double low) {
  double r = low + static_cast<float>(rand()) * static_cast<float>(high - low) / RAND_MAX;

  if (r < -5.12 || r > 5.12) {
    return generate_random_number(high, low);
  }
  return r;
}

std::vector<double> generate_vector (int dimenstion, double high, double low) {
  std::vector<double> vect;

  for (int index = 1; index <= dimenstion; index++) {
    double random_num = generate_random_number(high, low);
    vect.push_back(random_num);
  }

  return vect;
}
int main () {
  std::vector<double> vect = generate_vector(2, -5.12, 5.12);
  print_de_jong_function_result(vect);

  // std::cout << std::endl;

  return 0;
}
