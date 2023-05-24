#include "Knapsack.h"

void findSelectedItens(float capacity, int size, vector<float> heights, vector<vector<float>> table, vector<int> &selectedItens) {
  int actualCapactiy = capacity;

  for (int i = size; i > 0 && actualCapactiy > 0; i--) {
    if (table[i][capacity] != table[i - 1][actualCapactiy]) {
      selectedItens.push_back(i - 1);
      actualCapactiy -= heights[i - 1];
    }
  }
}

float Knapsack::BasicKnapSack(float capacity, BasicKnapsack basic, vector<int> &selectedItens) {
  int size = basic.weights.size();
  vector<float> weights = basic.weights;

  vector<vector<float>> table(size + 1, vector<float>(capacity + 1, 0));

  for (int i = 1; i <= size; i++) {
    for (int j = 1; j <= capacity; j++) {
      if (weights[i - 1] <= j) {
        table[i][j] = max(weights[i - 1] + table[i - 1][j - weights[i - 1]],
                          table[i - 1][j]);
      } else {
        table[i][j] = table[i - 1][j];
      }
    }
  }

  findSelectedItens(capacity, size, weights, table, selectedItens);

  return table[size][capacity];
}

float Knapsack::ComplexKnapsack(float capacity, ValuedKnapsack knapsackValues, vector<int> &selectedItens) {
  vector<float> weights = knapsackValues.basic.weights;
  int size = weights.size();

  vector<vector<float>> table(size + 1, vector<float>(capacity + 1, 0));

  for (int i = 1; i <= size; i++) {
    for (int j = 1; j <= capacity; j++) {
      if (weights[i - 1] <= j) {
        table[i][j] = max(knapsackValues.values[i - 1] + table[i - 1][j - weights[i - 1]],
                          table[i - 1][j]);
      } else {
        table[i][j] = table[i - 1][j];
      }
    }
  }

  findSelectedItens(capacity, size, weights, table, selectedItens);

  return table[size][capacity];
}