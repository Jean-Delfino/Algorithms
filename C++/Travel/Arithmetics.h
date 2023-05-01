#ifndef ARITHMETICS_H
#define ARITHMETICS_H

#include <cmath>

struct Point {
  double x, y;

  Point(double a, double b) {
    x = a;
    y = b;
  }
  Point() {}

  bool operator==(const Point &other) const {
    return x == other.x && y == other.y;
  }
};

class Arithmetics {
public:
  double ManhattanDistance(Point p1, Point p2);
  double EuclidianDistance(Point p1, Point p2);

protected:
};

#endif
