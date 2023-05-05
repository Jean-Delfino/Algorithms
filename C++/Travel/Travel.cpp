#include "Travel.h"

bool ContainPoint(TravelPoint travelPoint, Point p) {
  for (const Point &point : travelPoint.pointsList) {
    if (point == p) {
      return true;
    }
  }
  return false;
}

bool shouldAdd(int **graph, int actualLin, int actualCol, int maxLin,
               int maxCol) {
  if (actualLin < 0 || actualCol < 0)
    return false;
  if (actualLin > maxLin - 1 || actualCol > maxCol - 1)
    return false;
  if (graph[actualLin][actualCol] == INVALID_POINT)
    return false;

  return true;
}

queue<Point> findValidNeighbors(int **graph, int maxLin, int maxCol, int lin,
                                int col) {
  queue<Point> validPoints;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      int actualLin = lin + i;
      int actualCol = col + j;

      if (i == 0 && j == 0)
        continue;
      if (shouldAdd(graph, actualLin, actualCol, maxLin, maxCol)) {
        validPoints.push(Point(actualCol, actualLin));
      }
    }
  }
  return validPoints;
}

int Travel::TravelPointGraph(int **graph, int col, int lin, Point origin,
                             Point dest,
                             double (*distanceMethod)(Point, Point)) {

  if (dest.x > col || dest.y > lin || dest.x < 0 || dest.y < 0)
    return -1;
  int minDistance = -1;
  Point auxPoint;
  priority_queue<TravelPoint> travelPoints;
  queue<Point> neighboors;

  TravelPoint actualTravelPoint = TravelPoint((int)origin.x, (int)origin.y, 1,
                                              distanceMethod(origin, dest));
  actualTravelPoint.pointsList.push_back(origin);
  travelPoints.push(actualTravelPoint);

  while (!travelPoints.empty()) {
    actualTravelPoint = travelPoints.top();
    travelPoints.pop();

    if (actualTravelPoint.actualPoint == dest) {
      if (minDistance == -1 || actualTravelPoint.actualPathSize < minDistance) {
        minDistance = actualTravelPoint.actualPathSize;
      }

      if (travelPoints.top().actualPathSize >
          actualTravelPoint.actualPathSize - 1) {
        break;
      }
    }

    neighboors =
        (findValidNeighbors(graph, lin, col, actualTravelPoint.actualPoint.y,
                            actualTravelPoint.actualPoint.x));

    while (!neighboors.empty()) {
      auxPoint = neighboors.front();
      neighboors.pop();

      if (ContainPoint(actualTravelPoint, auxPoint)) {
        continue;
      }

      travelPoints.push(TravelPoint(actualTravelPoint, auxPoint, 1,
                                    distanceMethod(auxPoint, dest)));
    }
  }
  return minDistance;
}
