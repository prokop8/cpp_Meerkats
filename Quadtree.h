#ifndef _QUADTREE_H_
#define _QUADTREE_H_
#include <vector>

class Point
{
	float x;
	float y;
	//ID of this animal
	int number;
	//if its crocodile and its in range of meerkat jump - true
	bool isInRange;
public:
	Point(float x = 0, float y = 0, int number = 0, bool isInRange = false) :x(x), y(y), number(number), isInRange(isInRange) {};
	float getX() const;
	float getY() const;
	int getNumber() const;
	bool getIsInRange() const;
	friend const Point operator+(const Point& left, const Point& right);
	friend const Point operator-(const Point& left, const Point& right);
	friend bool operator==(const Point& left, const Point& right);
	friend bool operator!=(const Point& left, const Point& right);
};

struct AABB
{
	//down left
	Point first;
	//up rigth
	Point second;

	AABB(Point first = Point(), Point second = Point()) :first(first), second(second) {};
	bool containsPoint(const Point& a) const;
	bool intersects(const AABB& other) const;
};

class Quadtree
{
	Quadtree* northWest;
	Quadtree* northEast;
	Quadtree* southWest;
	Quadtree* southEast;

	AABB boundary;
	int CAPACITY;
	std::vector<Point> animals;

public:
	Quadtree();
	Quadtree(AABB boundary);
	~Quadtree();
	bool insert(Point point);
	void subdivide();
	std::vector<Point> queryRange(AABB range);
};
#endif
