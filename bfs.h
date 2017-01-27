#include "Quadtree.h"
#include <vector>

void bfs(Quadtree*, std::vector<Point>, int, int);

bool isPointInsideCircle(Point, Point, int);

std::vector<Point> findPointInMeerkatRange(Point, int, Quadtree*);

std::vector<Point> findPointInCrocodileRange(std::vector<Point>, Point, Point, int);

int findCrocodileEnd(Point, int, std::vector<Point>);

AABB createCrocodileRange(Point, Point, int);

