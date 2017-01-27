#include "Quadtree.h"
#include "iostream"

float Point::getX() const
{
	return x;
}

float Point::getY() const
{
	return y;
}

int Point::getNumber() const
{
	return number;
}

bool Point::getIsInRange() const
{
	return isInRange;
}

const Point operator+(const Point& left, const Point& right) 
{
	return Point(left.x + right.x, left.y + right.y);
}

const Point operator-(const Point& left, const Point& right)
{
	return Point(left.x - right.x, left.y - right.y);
}

bool operator==(const Point& left, const Point& right)
{
	return (left.x == right.x && left.y == right.y);
}

bool operator!=(const Point& left, const Point& right)
{
	return (left.x != right.x || left.y != right.y);
}

bool AABB::containsPoint(const Point& a) const
{
	return !((a.getX() < first.getX()) || (a.getY() < first.getY()) ||
		(a.getX() > second.getX()) || (a.getY() > second.getY()));
}

bool AABB::intersects(const AABB& other) const
{
	return !((first.getX() > other.second.getX()) || (first.getY() > other.second.getY()) ||
		(second.getX() < other.first.getX()) || (second.getY() < other.first.getY()));
}

Quadtree::Quadtree()
{
	CAPACITY = 100;
	northWest = nullptr;
	northEast = nullptr;
	southWest = nullptr;
	southEast = nullptr;
	boundary = AABB();
	animals = std::vector<Point>();
}

Quadtree::Quadtree(AABB boundary)
{
	CAPACITY = 100;
	animals = std::vector<Point>();
	northWest = nullptr;
	northEast = nullptr;
	southWest = nullptr;
	southEast = nullptr;
	this->boundary = boundary;
}

Quadtree::~Quadtree()
{
	delete northWest;
	delete northEast;
	delete southWest;
	delete southEast;
}

void Quadtree::subdivide()
{
	float midX = (boundary.first.getX() + boundary.second.getX()) / 2.0f;
	float midY = (boundary.first.getY() + boundary.second.getY()) / 2.0f;

	Point first = Point(boundary.first.getX(), boundary.first.getY());
	Point second = Point(midX, midY);
	northWest = new Quadtree(AABB(first, second));

	first = Point(midX, boundary.first.getY());
	second = Point(boundary.second.getX(), midY);
	northEast = new Quadtree(AABB(first, second));

	first = Point(boundary.first.getX(), midY);
	second = Point(midX, boundary.second.getY());
	southWest = new Quadtree(AABB(first, second));

	first = Point(midX, midY);
	second = Point(boundary.second.getX(), boundary.second.getY());
	southEast = new Quadtree(AABB(first, second));

	while (!animals.empty())
	{
		if(northWest->insert(animals.back())){}
		else if(northEast->insert(animals.back())){}
		else if(southWest->insert(animals.back())){}
		else if(southEast->insert(animals.back())){}
		animals.pop_back();
	}
}

bool Quadtree::insert(Point point)
{
	if (!boundary.containsPoint(point))
		return false;

	if (northWest != nullptr)
	{
		if (northWest->insert(point))
			return true;
		else if (northEast->insert(point))
			return true;
		else if (southWest->insert(point))
			return true;
		else if (southEast->insert(point))
			return true;
	}

	if (animals.size() < CAPACITY)
	{
		animals.push_back(point);
		return true;
	}

	if (northWest == nullptr)
		subdivide();

	if (northWest->insert(point))
		return true;

	if (northEast->insert(point))
		return true;

	if (southWest->insert(point))
		return true;
	
	if (southEast->insert(point))
		return true;

	return false;
}

std::vector<Point> Quadtree::queryRange(AABB range)
{
	std::vector<Point> pointInRange;
	if (!boundary.intersects(range))
		return pointInRange;
	for (int i = 0; i < int(animals.size()); i++)
	{
		if (range.containsPoint(animals[i]))
			pointInRange.push_back(animals[i]);
	}
	if (northWest == nullptr)
		return pointInRange;

	std::vector<Point> tmp = northWest->queryRange(range);
	pointInRange.insert(pointInRange.end(), tmp.begin(), tmp.end());

	tmp = northEast->queryRange(range);
	pointInRange.insert(pointInRange.end(), tmp.begin(), tmp.end());

	tmp = southWest->queryRange(range);
	pointInRange.insert(pointInRange.end(), tmp.begin(), tmp.end());

	tmp = southEast->queryRange(range);
	pointInRange.insert(pointInRange.end(), tmp.begin(), tmp.end());

	return pointInRange;
}
