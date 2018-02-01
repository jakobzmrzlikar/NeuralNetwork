#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <vector>

typedef std::vector<std::vector<double>> data;

class QuadraticCost {

public:
    static double cost(double a, double y);
    static double error_b(double a, double y);
    static double error_w(double a, double y, double x);

};

class Standardization {
public:
    static data normalize(data training_data);
    static data scale(data training_data);
};

#endif /* FUNCTIONS_H */