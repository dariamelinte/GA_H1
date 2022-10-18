//
// Created by Melinte Daria on 18/10/22.
//

#ifndef CLASS_BASED_PROJECT_DE_JONG_H
#define CLASS_BASED_PROJECT_DE_JONG_H


class de_jong {
    double low = -5.12;
    double high = 5.12;

    int precision;
    int dimensions;

    int bit_string_len;
    int len;

public:
    de_jong();
    double get_low_bound();
    double get_high_bound();
    int get_precision();
    int get_dimensions();

    void set_precision(int precision);
    void set_dimensions(int dimensions);

    void set_bit_string
};


#endif //CLASS_BASED_PROJECT_DE_JONG_H
