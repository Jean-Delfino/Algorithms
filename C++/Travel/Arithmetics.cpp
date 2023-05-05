#include "Arithmetics.h"

double ManhattanDistance(Point p1, Point p2) {
  return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

double EuclidianDistance(Point p1, Point p2) {
  return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}
