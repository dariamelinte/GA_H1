//
// Created by Melinte Daria on 18/10/22.
//

#include "de_jong.h"

de_jong::de_jong() {
    precision = 5;
    dimensions = 10;
}

double de_jong::get_high_bound() {
    return high;
}

double de_jong::get_low_bound() {
    return low;
}

int de_jong::get_precision() {
    return precision;
}

int de_jong::get_dimensions() {
    return dimensions;
}

void de_jong::set_precision(int precision) {
    this -> precision = precision;
}

void de_jong::set_dimensions(int dimensions) {
    this -> dimensions = dimensions;
}