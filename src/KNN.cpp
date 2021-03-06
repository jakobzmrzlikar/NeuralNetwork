#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "functions.hpp"
#include "cost.hpp"
#include "KNN.hpp"

// training_data vector consisting of pairs (x,y)
typedef std::vector<std::vector<double>> data;

KNN::KNN() {}

double KNN::h(std::vector<double>& x, int k, std::string mode="classification") {
  std::vector<double> neighbours = kNeighbours(x, k);
  if (mode == "classification") {
    double label = neighbours[0];
    int occurances = std::count(neighbours.begin(), neighbours.end(), label);
    for (int i=1; i<k; i++) {
      if (neighbours[i] != label) {
        int new_occurances = std::count(neighbours.begin(), neighbours.end(), neighbours[i]);
        if (new_occurances > occurances) {
          label = neighbours[i];
          occurances = new_occurances;
        }
      }
    }
    return label;
  } else if (mode == "regression") {
    double result = 0.0;
    for (int i=0; i<k; i++) {
      result += neighbours[i];
    }
    return result/k;
  } else {
    std::cout << "Invalid mode: " << mode << '\n';
  }
}

void KNN::train(data& training_data) {
  space = training_data;
}

double KNN::cost(data& training_data, std::string mode) {
  int m = training_data.size();
  double cost = 0.0;
  double correct_class = 0.0;
  for (int i=0; i<m; i++) {
      std::vector<double> x(training_data[i].begin(), training_data[i].end()-1);
      double y = training_data[i].back();
      double a = h(x, K, mode);
      cost += QuadraticCost::cost(a, y);
      if (fabs(a-y)<0.5) correct_class++;
  }

  if (mode == "regression") {
    return cost/m;
  } else if (mode == "classification") {
    return correct_class/m * 100;
  }
}

std::vector<double> KNN::kNeighbours(std::vector<double>& x, int k) {
  std::vector<std::pair<double, double>> d; // (distance, label)
  for (int i=0; i<space.size(); i++) {
    double label = space[i].back();
    space[i].pop_back();
    d.push_back(std::make_pair((distance(x, space[i])), label));
    space[i].push_back(label);
  }

  std::sort(d.begin(), d.end());

  std::vector<double> result;
  for (int i=0; i<k; i++) {
    result.push_back(d[i].second);
  }

  return result;
}

void KNN::save(std::string filename) {
    std::string name = "../data/saves/" + filename;
    std::ofstream file(name);

    for (int i=0; i<space.size(); i++) {
      file << space[i][0];
      for (int j=1; j<space[i].size(); j++) {
        file << "," << space[i][j];
      }
      file << '\n';
    }
    std::cout << "Model saved to path: " << name << '\n';
}

void KNN::load(std::string filename) {
    std::string name = "../data/saves/" + filename;
    std::ifstream file(name);
    std::string line;
    std::vector<double> vec;
    space.clear();

    while(getline(file,line)) {
        std::stringstream linestream(line);
        std::string value;

        while(getline(linestream,value,',')) {
          vec.push_back(atof(value.c_str()));
        }
        space.push_back(vec);
        vec.clear();
  }
}

std::ostream& operator<<(std::ostream& os, const KNN& m) {
  return os << "KNN";
}
