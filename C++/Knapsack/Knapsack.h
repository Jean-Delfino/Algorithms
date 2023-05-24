#ifndef KNAPSACK_H
#define KNAPSACK_H

#include <iostream>
#include <vector>

using namespace std;

struct KnapsackResult {
  float capacityResult;
  vector<int> selectedItems;
};

class Knapsack {
private:
  struct BasicKnapsack {
    vector<float> &weights;
  };

  struct ValuedKnapsack {
    BasicKnapsack basic;
    vector<float> &values;
  };

public:
  KnapsackResult FindBasicKnapsack(float capacity, vector<float> &heights) {
    if (capacity < 1) return {0, {}};

    BasicKnapsack basicKnapsack{heights};
    KnapsackResult result;
    result.capacityResult = BasicKnapSack(capacity, basicKnapsack, result.selectedItems);

    return result;
  }

  KnapsackResult FindComplexKnapsack(float capacity, vector<float> &heights, vector<float> &values) {
    if (capacity < 1) return {0, {}};

    BasicKnapsack basicKnapsack{heights};
    ValuedKnapsack valuedKnapsack{basicKnapsack, values};
    KnapsackResult result;
    result.capacityResult = ComplexKnapsack(capacity, valuedKnapsack, result.selectedItems);

    return result;
  }

private:
  float BasicKnapSack(float capacity, BasicKnapsack basic, vector<int> &selectedItens);
  float ComplexKnapsack(float capacity, ValuedKnapsack knapsackValues, vector<int> &selectedItens);
};

#endif