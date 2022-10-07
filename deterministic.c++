#include <iostream>
#include <vector>

double epsilon = 0.00001;

double de_jong_function (std::vector<double> x) {
  int dimension = x.size();
  double f_x = 0;
  for (int facet = 1; facet <= dimension; facet++) {
    f_x += x[facet] * x[facet]; 
  }

  return f_x;
}

void print_de_jong_function_result (std::vector<double> vect) {
  std::cout << "valoarea functiei pentru vectorul (";
  int size = vect.size();
  for (int i = 1; i <= size; i++) {
    std::cout << vect[i];

    if (i != size) {
      std::cout << ", ";
    }
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

int main () {
  std::vector<double> vect;
  vect.push_back(3.2);
  vect.push_back(1.23);
  vect.push_back(-2.89);

  print_de_jong_function_result(vect);

  return 0;
}
