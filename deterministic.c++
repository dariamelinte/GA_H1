
double epsilon = 0.00001;

void hillclimbing(std::vector<double> vect) {
  auto start = std::chrono::high_resolution_clock::now();
  int index = 0;

  while (index <1000000) {
    index++;
  }

  auto stop = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> time = stop - start;
  std::cout << " hillclimbing ---- difference: " << time.count() / 1000;
};


// void HillClimbing(function<double(vector<double>)> mathFunction, function<void(vector<bool>&, function<double(vector<double>)>)> findFunction)
// {
    
//     double best, tmpBest, bestMinimum = numeric_limits<double>::max();
//     int testsNumber = 1000;
//     bool local;
    
//     do {
//         vector<bool> vc(L), vn(L);
//         local = false;
//         bitStringGenerator(vc);
//         double tmpResult = mathFunction(bitStringToDoubleValue(vc));
//         double neighbourResult;
//         do {
//             //bitStringGenerator(vn);
//             //vn = findFunction(vc, mathFunction);
//             vn = vc;
//             findFunction(vn, mathFunction);
//             neighbourResult = mathFunction(bitStringToDoubleValue(vn));

//             if (neighbourResult < tmpResult)
//             {
//                 vc = vn;
//                 tmpResult = neighbourResult;
//             }
//             else
//                 local = true;

//         } while (!local);

//         if (tmpResult < bestMinimum)
//             bestMinimum = tmpResult;

//         testsNumber--;

//     } while (testsNumber != 0);

//     auto stop = high_resolution_clock::now();

//     cout << fixed << setprecision(5) << bestMinimum << '\n';

//     duration<double, milli> time = stop - start;
//     rtime.push_back(time.count() / 1000);
//     rvalues.push_back(bestMinimum);
// }

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

void print_schwefels_function_result (std::vector<double> vect) {
  std::cout << "Schwefel's ---- v ( ";
  for (double x : vect) {
    std::cout << x << " ";
  }

  std::cout << ") = " << schwefels_function(vect) << std::endl;

  hillclimbing(vect);
}

void print_rastrigins_function_result (std::vector<double> vect) {
  std::cout << "Rastrigin's ---- v ( ";
  for (double x : vect) {
    std::cout << x << " ";
  }

  std::cout << ") = " << rastrigins_function(vect) << std::endl;

  hillclimbing(vect);
}

double generate_random_number(double low, double high) {
  return low + static_cast<float>(random_gen()) * static_cast<float>(high - low) / random_gen.max();
}

std::vector<double> generate_vector (int dimenstion, double high, double low) {
  std::vector<double> vect;

  for (int index = 0; index < dimenstion; index++) {
    double random_num = generate_random_number(low, high);
    vect.push_back(random_num);
  }

  return vect;
}

void run_schwefels (int n) {
  for (int i = 0; i < n; i++) {
    std::cout << i << " --> ";
    std::vector<double> vect = generate_vector(4, -500, 500);
    print_schwefels_function_result(vect);
    std::cout << std::endl;
  }
}

void run_rastrigins (int n) {
  for (int i = 0; i < n; i++) {
    std::cout << i << " --> ";
    std::vector<double> vect = generate_vector(4, -5.12, 5.12);
    print_rastrigins_function_result(vect);
    std::cout << std::endl;
  }
}

int main () {
  auto start = std::chrono::high_resolution_clock::now();
  int index = 0;
  srand(time(0));
  run_de_jong(10000);
  run_schwefels(10000);
  run_rastrigins(10000);
  auto stop = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> time = stop - start;
  std::cout << " main ---- difference: " << time.count() / 1000;
  return 0;
}
