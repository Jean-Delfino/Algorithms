#ifndef TRAVEL_H
#define TRAVEL_H

#include <queue>

using namespace std;

#include "Arithmetics.h"

#define INVALID_POINT 0

struct TravelPoint {
  int actualPathSize;
  double totalDistance;
  Point actualPoint;

  TravelPoint(int row, int col, int path, double distanceToFinalPoint)
      : actualPoint(row, col) {
    actualPathSize = path;
    totalDistance = path + distanceToFinalPoint;
  }

  bool operator<(const TravelPoint &other) const {
    if (totalDistance < other.totalDistance)
      return true;
    return false;
  }
};

class Travel {
public:
  int TravelPointGraph(int **graph, int col, int lin, Point origin, Point dest,
                       double (*distanceMethod)(Point, Point));

protected:
};

#endif
