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

std::mt19937 random_gen(time(0));

double highest_double = std::numeric_limits<double>::max();

functions FUNCTION;

double LOW;
double HIGH;

int BIT_STRING_LEN;
int LEN;

int PRECISION = 5;
int NUMBER_OF_DIMENSIONS;
double ALPHA;

void compute_filename(std::string& filename) {
    switch (FUNCTION) {
    case functions::de_jong:
        filename += "de-jong___";
        break;
    case functions::rastrigins:
        filename += "rastrigins___";
        break;
    case functions::schwefels:
        filename += "schwefels___";
        break;
    case functions::michalewiczs:
        filename += "michalewiczs___";
        break;
    default:
        break;
    }

    filename += "simulated-annealing-fi___dimensions-";
    filename += std::to_string(NUMBER_OF_DIMENSIONS);
    filename += ".txt";
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
void convert_bit_string_to_b10_vector(std::vector<bool>& bit_string, std::vector<double>& b10_vector) {
    int poz = 0;
    for (int i = 0; i < LEN; i += BIT_STRING_LEN) {
        int xi = 0LL;
        for (int j = i; j < i + BIT_STRING_LEN; ++j) {
            xi *= 2;
            xi += bit_string[j];
        }
        double value = (double)xi / (pow(2.000, BIT_STRING_LEN) - 1.0000);
        b10_vector.push_back(LOW + value * (HIGH - LOW));
    }
}

double run_given_function(std::vector<bool>& bit_string) {
    double result;

    std::vector<double> b10_vector;
    convert_bit_string_to_b10_vector(bit_string, b10_vector);

    switch (FUNCTION) {
    case functions::de_jong:
        result = de_jongs_function(b10_vector);
        break;
    case functions::schwefels:
        result = schwefels_function(b10_vector);
        break;
    case functions::rastrigins:
        result = rastrigins_function(b10_vector);
        break;
    case functions::michalewiczs:
        result = michalewiczs_function(b10_vector);
        break;
    }

    return result;
}


double simulated_annealing() {
    //std::cout << FUNCTION << " " << NUMBER_OF_DIMENSIONS << '\n';

    int maxi_r;
    if (NUMBER_OF_DIMENSIONS == 5) {
        // 5 dimensiuni
        maxi_r = 4000;
    }

    if (NUMBER_OF_DIMENSIONS == 10) {
        // 10 dimensiuni
        maxi_r = 1500;
    }

    if (NUMBER_OF_DIMENSIONS == 30) {
        // 30 dimensiuni
        maxi_r = 500;
    }

    double best_value = highest_double;

    for (int index = 0; index < maxi_r; index++) {
        std::cout << index << '\n';

        double temperature = 10000;

        std::vector<bool> candidate_bit_string;
        for (int index = 0; index < LEN; index++) {
            candidate_bit_string.push_back(random_gen() % 2);
        }

        double candidate_result_fn = run_given_function(candidate_bit_string);

        std::random_device rand_dev;
        std::mt19937 gen(rand_dev());
        std::uniform_real_distribution<> dis(0, 1);

        do {
            int else_enters = 0;
            bool no_downgrade = false;

            do {
                std::vector<bool> nbh_bit_string = candidate_bit_string;

                for (int index = 0; index < LEN; index++) {
                    nbh_bit_string[index] = !nbh_bit_string[index];

                    double nbh_result_fn = run_given_function(nbh_bit_string);

                    if (nbh_result_fn < candidate_result_fn) {
                        break;
                    }

                    nbh_bit_string[index] = !nbh_bit_string[index];
                }

                if (nbh_bit_string == candidate_bit_string) {
                    std::vector<bool> nbh2 = candidate_bit_string;
                    double dif = highest_double;
                    for (int i = 0; i < LEN; ++i)
                    {
                        nbh_bit_string[i] = !nbh_bit_string[i];
                        double fx_nbh = run_given_function(nbh_bit_string);
                        if (candidate_result_fn - fx_nbh < dif)
                        {
                            dif = candidate_result_fn - fx_nbh;
                            nbh2 = nbh_bit_string;
                        }
                        nbh_bit_string[i] = !nbh_bit_string[i];
                    }
                    nbh_bit_string = nbh2;
                }

                if (candidate_result_fn < best_value)
                    best_value = candidate_result_fn;

                double nbh_result_fn = run_given_function(nbh_bit_string);

                if (nbh_result_fn < candidate_result_fn) {
                    // std::cout << "FIRST IF" << std::endl;
                    candidate_result_fn = nbh_result_fn;
                    candidate_bit_string = nbh_bit_string;


                    if (candidate_result_fn < best_value)
                        best_value = candidate_result_fn;
                }
                else if (dis(gen) < std::exp(-std::abs(nbh_result_fn - candidate_result_fn) / temperature)) {
                    // std::cout << "ELSE IF" << std::endl;

                    if (candidate_result_fn < best_value)
                        best_value = candidate_result_fn;
                    candidate_result_fn = nbh_result_fn;
                    candidate_bit_string = nbh_bit_string;
                    else_enters++;
                }
                else {
                    // std::cout << "ELSE ELSE" << std::endl;
                    no_downgrade = true;
                }

                // std::cout << index << " " << nbh_result_fn << " " << candidate_result_fn << std::endl;

            } while (else_enters < 60 && !no_downgrade);

            temperature = temperature * ALPHA;

        } while (temperature > 0.0000019);
    }

    return best_value;
}


void compile(int max_ind) {
    std::string output_filename = "./rapoarte/";
    compute_filename(output_filename);

    set_bounds();
    BIT_STRING_LEN = ceil(log2(pow(10, PRECISION) * (HIGH - LOW)));
    LEN = NUMBER_OF_DIMENSIONS * BIT_STRING_LEN;

    for (int index = 1; index <= max_ind; index++) {
        std::ofstream out_file;
        out_file.open(output_filename, std::ios_base::app);

        out_file << std::setprecision(PRECISION) << std::fixed;

        auto start_sa = std::chrono::high_resolution_clock::now();

        auto best_sol = simulated_annealing();


        auto stop_sa = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> time_sa = stop_sa - start_sa;
        out_file << index << " -- solution: " << best_sol << " time: " << time_sa.count() / 1000;
        out_file << '\n' << '\n';

        out_file.close();
    }
}


int main() {
    srand(time(0));

    for (int i = 0; i < 6; i++) {
      NUMBER_OF_DIMENSIONS = 5;

      FUNCTION = functions::de_jong;
      ALPHA = 0.3;
      compile(5);

      FUNCTION = functions::schwefels;
      ALPHA = 0.65;
      compile(5);

      FUNCTION = functions::rastrigins;
      ALPHA = 0.3;
      compile(5);

      FUNCTION = functions::michalewiczs;
      ALPHA = 0.25;
      compile(5);

      NUMBER_OF_DIMENSIONS = 10;

      FUNCTION = functions::de_jong;
      ALPHA = 0.3;
      compile(5);

      FUNCTION = functions::schwefels;
      ALPHA = 0.65;
      compile(5);

      FUNCTION = functions::rastrigins;
      ALPHA = 0.3;
      compile(5);

      FUNCTION = functions::michalewiczs;
      ALPHA = 0.25;
      compile(5);

      NUMBER_OF_DIMENSIONS = 30;

      FUNCTION = functions::de_jong;
      ALPHA = 0.3;
      compile(5);

      FUNCTION = functions::schwefels;
      ALPHA = 0.65;
      compile(5);

      FUNCTION = functions::rastrigins;
      ALPHA = 0.3;
      compile(5);

      FUNCTION = functions::michalewiczs;
      ALPHA = 0.25;
      compile(5);
    }

    return 0;
}