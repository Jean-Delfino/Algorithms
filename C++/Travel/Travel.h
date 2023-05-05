#ifndef TRAVEL_H
#define TRAVEL_H

#include <iostream>
#include <queue>
#include <vector>

using namespace std;

#include "Arithmetics.h"

#define INVALID_POINT 0

struct TravelPoint {
  int actualPathSize;
  double totalDistance;
  Point actualPoint;
  vector<Point> pointsList;

  TravelPoint(int col, int lin, int path, double distanceToFinalPoint)
      : actualPoint(col, lin) {
    actualPathSize = path;
    totalDistance = distanceToFinalPoint;
  }

  TravelPoint(const TravelPoint &other, Point newPoint, int pathIncrease,
              double totalDistance)
      : TravelPoint(newPoint.x, newPoint.y, other.actualPathSize + pathIncrease,
                    totalDistance)

  {
    pointsList = other.pointsList;
    pointsList.push_back(actualPoint);
  }

  bool operator<(const TravelPoint &other) const {
    return totalDistance > other.totalDistance;
  }
};

class Travel {
public:
  int TravelPointGraph(int **graph, int col, int lin, Point origin, Point dest,
                       double (*distanceMethod)(Point, Point));

protected:
private:
};

#endif
