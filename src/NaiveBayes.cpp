#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include "NaiveBayes.hpp"

// training_data vector consisting of pairs (x,y)
typedef std::vector<std::vector<double>> data;

NaiveBayes::NaiveBayes() {}

double NaiveBayes::h(std::vector<double>& x) {
  double label;
  double probability = 0;
  double data_probability = 0; // P(d)
  for (int i=0; i<probabilities.size(); i++) {
    double new_probability = probabilities[i].back(); // P(class==i)
    for (int j=0; j<probabilities[0].size()-1; j++) {
      if (x[j]) {
        new_probability *= probabilities[i][j]; // P(x[j]| class==i)
      } else {
        new_probability *= (1 - probabilities[i][j]); // P(!x[j] | class==i)
      }
    }

    data_probability += new_probability;
    if (new_probability > probability) {
      probability = new_probability;
      label = i;
    }
  }
  probability /= data_probability;
  // Calculate final probability, even though label is known before that.
  std::cout << "Probability: " << probability*100 << '%' << '\n';

  return label;
}

// train DOESN'T WORK for negative labels!
void NaiveBayes::train(data& training_data) {
  double label;
  for (int i=0; i<training_data.size(); i++) {
    label = training_data[i].back();
    if (label >= probabilities.size()) {
      for (int j=probabilities.size(); j<label+1; j++) {
        std::vector<double> v(training_data[0].size(), 0);
        probabilities.push_back(v);
      }
    }

    for (int j=0; j<training_data[0].size()-1; j++) {
      if (training_data[i][j]) {
        probabilities[label][j]++;
      }
    }
    probabilities[label].back()++;

  }

  for (int i=0; i<probabilities.size(); i++) {
    for (int j=0; j<probabilities[0].size()-1; j++) {
      probabilities[i][j] /= probabilities[i].back(); // P(x[j] | class == i)
    }
    probabilities[i].back() /= training_data.size(); // P(class == i)
  }

}

double NaiveBayes::cost(data& training_data, std::string mode) {
  int m = training_data.size();
  int correct_class = 0;
  for (int i=0; i<m; i++) {
      std::vector<double> x(training_data[i].begin(), training_data[i].end()-1);
      double y = training_data[i].back();
      double a = h(x);
      if (fabs(a-y)<0.5) correct_class++;
  }

  if (mode == "classfiaction") {
    return correct_class/m * 100;
  }
}

void NaiveBayes::save(std::string filename) {
    std::string name = "../data/saves/" + filename;
    std::ofstream file(name);

    for (int i=0; i<probabilities.size(); i++) {
      file << probabilities[i][0];
      for (int j=1; j<probabilities[i].size(); j++) {
        file << "," << probabilities[i][j];
      }
      file << '\n';
    }
    std::cout << "Model saved to path: " << name << '\n';
}

void NaiveBayes::load(std::string filename) {
    std::string name = "../data/saves/" + filename;
    std::ifstream file(name);
    std::string line;
    std::vector<double> vec;
    probabilities.clear();

    while(getline(file,line)) {
        std::stringstream linestream(line);
        std::string value;

        while(getline(linestream,value,',')) {
          vec.push_back(atof(value.c_str()));
        }
        probabilities.push_back(vec);
        vec.clear();
  }
}

std::ostream& operator<<(std::ostream& os, const NaiveBayes& m) {
  return os << "Naive Bayes";
}
