#ifndef LOGISTICREGRESSION_H
#define LOGISTICREGRESSION_H

#include <vector>
#include <string>

// training_data vector consisting of pairs (x,y)
typedef std::vector<std::vector<double>> data;

class LogisticRegression {

public:

    // vector of weights
    std::vector<double> w;

    LogisticRegression(int features);

    // call function
    double h(std::vector<double> x);

    double cost(data training_data, int m);

    // train returns vector of costs at each epoch
    std::vector<double> train(data training_data, double learning_rate, int epochs);

    void save(std::string filename);

    void load(std::string filename);

private:

    void gradient_descent(data training_data, double learning_rate, int m);

};

#endif /* LOGISTICREGRESSION_H */