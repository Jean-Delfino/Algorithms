#include "Travel.h"

bool containPoint(TravelPoint travelPoint, Point p) {
  for (const Point &point : travelPoint.pointsList) {
    if (point == p) return true;
  }
  return false;
}

bool shouldAdd(int **graph, int actualLin, int actualCol, int maxLin,
               int maxCol) {
  if (actualLin < 0 || actualCol < 0) return false;
  if (actualLin > maxLin - 1 || actualCol > maxCol - 1) return false;
  if (graph[actualLin][actualCol] == INVALID_POINT) return false;

  return true;
}

queue<Point> findValidNeighbors(int **graph, int maxLin, int maxCol, int lin, int col) {
  queue<Point> validPoints;
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      int actualLin = lin + i;
      int actualCol = col + j;

      if (i == 0 && j == 0) continue;
      if (shouldAdd(graph, actualLin, actualCol, maxLin, maxCol)) validPoints.push(Point(actualCol, actualLin));

    }
  }
  return validPoints;
}

int Travel::TravelPointGraph(int **graph, int col, int lin, Point origin, Point dest, double (*distanceMethod)(Point, Point)) {

  if (dest.x > col || dest.y > lin || dest.x < 0 || dest.y < 0) return -1;
  int minDistance = -1;
  Point auxPoint;
  priority_queue<TravelPoint> travelPoints;
  queue<Point> neighboors;

  TravelPoint actualTravelPoint = TravelPoint((int)origin.x, (int)origin.y, 1, distanceMethod(origin, dest));

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
        (findValidNeighbors(graph, lin, col, actualTravelPoint.actualPoint.y, actualTravelPoint.actualPoint.x));

    while (!neighboors.empty()) {
      auxPoint = neighboors.front();
      neighboors.pop();

      if (containPoint(actualTravelPoint, auxPoint)) {
        continue;
      }

      travelPoints.push(TravelPoint(actualTravelPoint, auxPoint, 1, distanceMethod(auxPoint, dest)));
    }
  }
  return minDistance;
}

int countValidPoints(int **graph, vector<vector<int>> &matrix,vector<vector<int>> &memo, int lin, int col, int eY, int eX) {

  if (col == eX && lin == eY) {
    return memo[lin][col] = 1; // Reached the end point
  }

  // Check if current cell is out of bounds or an invalid path
  if (lin < 0 || lin > matrix.size() - 1 || col < 0 || col > matrix[0].size() - 1 || matrix[lin][col] == 0 || graph[lin][col] == INVALID_POINT) {
    return 0;
  }

  // Return memorized result if available
  if (memo[lin][col] != -1) return memo[lin][col]; 

  // Mark the current cell as visited
  matrix[lin][col] = 0;

  int paths = 0;
  // Explore all eight possible directions

  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      int actualLin = lin + i;
      int actualCol = col + j;

      if (i == 0 && j == 0) continue;
      paths += countValidPoints(graph, matrix, memo, actualLin, actualCol, eX, eY);
    }
  }

  // Mark the current cell as unvisited (optional for this problem)
  matrix[lin][col] = 1;
  memo[lin][col] = paths;
  return paths;
}

int Travel::FindNumberOfValidPathToPoint(int **graph, int lin, int col, Point origin, Point dest) {
  vector<vector<int>> matrix(lin, vector<int>(col, -1));
  vector<vector<int>> memo(lin, vector<int>(col, -1));
  return countValidPoints(graph, matrix, memo, origin.y, origin.x, dest.y, dest.x);
}
