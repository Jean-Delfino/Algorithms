#include "Travel.h"

bool shouldAdd(int **graph, int actualRow, int actualCol, int rows, int col) {
  if (actualRow < 0 || actualCol < 0)
    return false;
  if (actualRow > rows - 1 || actualCol > col - 1)
    return false;
  if (graph[actualRow][actualCol] == INVALID_POINT)
    return false;

  return true;
}

queue<Point> findValidNeighbors(int **graph, int row, int col, int rows, int cols) {
  queue<Point> validPoints;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      int actualRow = row + i;
      int actualCol = col + j;

      if (i == 0 && j == 0)continue;
      if (shouldAdd(graph, actualRow, actualCol, rows, cols)) {
        validPoints.push(Point(actualRow, actualCol));
      }
    }
  }
  return validPoints;
}

int Travel::TravelPointGraph(int **graph, int col, int row, Point origin,
                             Point dest,
                             double (*distanceMethod)(Point, Point)) {
  priority_queue<TravelPoint> travelPoints;
  queue<Point> neighboors;

  TravelPoint actualTravelPoint = TravelPoint((int)origin.x, 
  											  (int)origin.y, 
											  0,
                                              distanceMethod(origin, dest));
  Point auxPoint;

  travelPoints.push(actualTravelPoint);
  int minDistance = -1;

  while (!travelPoints.empty()) {
    actualTravelPoint = travelPoints.top();
    travelPoints.pop();

    if (actualTravelPoint.actualPoint == dest) {
      if (minDistance == -1 || actualTravelPoint.actualPathSize < minDistance) {
        minDistance = actualTravelPoint.actualPathSize;
      }

      if (travelPoints.top().actualPathSize > actualTravelPoint.actualPathSize - 1) {
        break;
      }
    }

    neighboors = findValidNeighbors(graph, actualTravelPoint.actualPoint.x,actualTravelPoint.actualPoint.y, row, col);

    while (!neighboors.empty()) {
      auxPoint = neighboors.front();
      neighboors.pop();

      TravelPoint((int)auxPoint.x, 
	  			  (int)auxPoint.y,
                  actualTravelPoint.actualPathSize + 1,
                  distanceMethod(auxPoint, dest));
    }
  }

  return minDistance;
}
