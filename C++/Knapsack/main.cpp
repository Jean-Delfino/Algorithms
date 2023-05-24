#include "Knapsack.h"
#include <iostream>

int main() {
  Knapsack solver;
  int i;
  float capacity = 10.0;
  vector<float> weights = {2.0, 3.0, 5.0, 7.0};
  vector<float> values = {10.0, 15.0, 20.0, 10.0};
  KnapsackResult res = solver.FindComplexKnapsack(capacity, weights, values);

  cout << res.capacityResult << endl;

  for (i = 0; i < res.selectedItems.size(); i++) {
    cout << res.selectedItems[i] << " ";
  }
  cout << endl;
}