#include "Quadtree.h"
#include <queue>
#include <vector>
#include "bfs.h"
#include "iostream"

void bfs(Quadtree* quadtree, std::vector<Point> animals, int jumpRange, int meerkatsNumber)
{
	std::vector<std::vector<int>> pathAnswer(animals.back().getNumber() + 1);
	for (size_t l = 0; l < meerkatsNumber; l++)
	{
		//if vertices were visited - true
		std::vector<bool> visited(animals.back().getNumber() + 1);
		//length ways vector
		std::vector<int> distance(animals.back().getNumber() + 1);
		std::queue<Point> q;
		std::vector<int> path(animals.back().getNumber() + 1);
		//vector with points that are in range of meerkat jump
		std::vector<Point> pointInRange;
		//if we found crocodile that is in range of meerkat jump to the opposite shore of the river - we leave while
		bool isFound = false;
		//number to restore path
		int numberOfLastCrocodile = 0;
		q.push(animals[l]);
		visited[animals[l].getNumber()] = true;
		distance[animals[l].getNumber()] = 0;
		path[animals[l].getNumber()] = -1;
		while (!q.empty())
		{
			//if we already have answer for this meerkat
			if (pathAnswer[l].size() != 0)
				break;
			Point v = q.front();
			q.pop();
			//if its meerkat
			if (v.getY() == 0)
			{
				pointInRange = findPointInMeerkatRange(v, jumpRange, quadtree);
			}
			//if its crocodile
			else
			{
				int number = findCrocodileEnd(v, meerkatsNumber, animals);
				AABB range = createCrocodileRange(v, animals[number], jumpRange);
				//finding points in rectangle range
				std::vector<Point> inRange = quadtree->queryRange(range);
				pointInRange = findPointInCrocodileRange(inRange, v, animals[number], jumpRange);
			}
			for (size_t i = 0; i < pointInRange.size(); i++)
			{
				if (!visited[pointInRange[i].getNumber()])
				{
					visited[pointInRange[i].getNumber()] = true;
					distance[pointInRange[i].getNumber()] = distance[v.getNumber()] + 1;
					//if this crocodile is in range of meerkat jump to the opposite shore of the river - we leave while
					if (pointInRange[i].getIsInRange() == true)
					{
						numberOfLastCrocodile = pointInRange[i].getNumber();
						isFound = true;
					}
					q.push(pointInRange[i]);
					path[pointInRange[i].getNumber()] = v.getNumber();
				}
			}
			//if we found answer we must reverse path and leave while
			if (isFound == true)
			{
				for (size_t i = numberOfLastCrocodile; i != -1; i = path[i])
				{
					pathAnswer[l].push_back(i);
				}
				reverse(pathAnswer[l].begin(), pathAnswer[l].end());
				break;
			}
		}
		//std::cout << l << " ---> " << pathAnswer[l].size() << " : ";
		//print path and if we have meerkats in the path set path to them
		for (size_t i = 0; i < pathAnswer[l].size(); i++)
		{
			//if we have meerkat and its path isnt set
			if (pathAnswer[pathAnswer[l][i]].size() == 0 && pathAnswer[l][i] < meerkatsNumber)
			{
				//set this path to meerkat and erase all meerkats before it from this path
				pathAnswer[pathAnswer[l][i]] = pathAnswer[l];
				for (size_t j = 0; j < i; j++)
				{
					pathAnswer[pathAnswer[l][i]].erase(pathAnswer[pathAnswer[l][i]].begin());
					std::vector<int>(pathAnswer[pathAnswer[l][i]]).swap(pathAnswer[pathAnswer[l][i]]);
				}
			}
			//std::cout << pathAnswer[l][i] << " ";
		}
		//std::cout << std::endl;
	}
}

bool isPointInsideCircle(Point center, Point point, int radius)
{
	return sqrt((center.getX() - point.getX())*(center.getX() - point.getX()) + 
		(center.getY() - point.getY())*(center.getY() - point.getY())) <= radius;
}

std::vector<Point> findPointInMeerkatRange(Point point, int jumpRange, Quadtree* quadtree)
{
	AABB range(Point(point.getX() - jumpRange, point.getY() - jumpRange), Point(point.getX() + jumpRange, point.getY() + jumpRange));
	std::vector<Point> pointInRange = quadtree->queryRange(range);
	std::vector<Point> pointInCircle;
	//checking all points in range and erase point if it isnt in circle
	for (size_t i = 0; i < pointInRange.size(); i++)
	{
		if (isPointInsideCircle(point, pointInRange[i], jumpRange))
		{
			pointInCircle.push_back(pointInRange[i]);
		}
	}
	return pointInCircle;
}

int findCrocodileEnd(Point point, int meerkatsNumber, std::vector<Point> animals)
{
	for (size_t i = meerkatsNumber; i < animals.size(); i++)
	{
		if (point.getNumber() == animals[i].getNumber() && point != animals[i])
		{
			return i;
		}
	}
}

AABB createCrocodileRange(Point first, Point second, int jumpRange)
{
	Point third;
	Point fourth;
	//sorting first and second points
	if (first.getX() >= second.getX())
	{
		Point tmp = first;
		first = second;
		second = tmp;
	}
	third = Point(first.getX(), second.getY());
	fourth = Point(second.getX(), first.getY());
	if (first.getY() <= third.getY())
		return AABB(Point(first.getX() - jumpRange, first.getY() - jumpRange), Point(second.getX() + jumpRange, second.getY() + jumpRange));
	else
		return AABB(Point(third.getX() - jumpRange, third.getY() - jumpRange), Point(fourth.getX() + jumpRange, fourth.getY() + jumpRange));
}

std::vector<Point> findPointInCrocodileRange(std::vector<Point> points, Point first, Point second, int jumpRange)
{
	std::vector<Point> tmp;
	for (size_t i = 0; i < points.size(); i++)
	{
		float a = points[i].getX() - first.getX();
		float b = points[i].getY() - first.getY();
		float c = second.getX() - first.getX();
		float d = second.getY() - first.getY();

		float dot = a * c + b * d;
		float len_sq = c * c + d * d;
		float param = -1;
		if (len_sq != 0) //in case of 0 length line
			param = dot / len_sq;

		float xx, yy;

		if (param < 0) {
			xx = first.getX();
			yy = first.getY();
		}
		else if (param > 1) {
			xx = second.getX();
			yy = second.getY();
		}
		else {
			xx = first.getX() + param * c;
			yy = first.getY() + param * d;
		}

		float dx = points[i].getX() - xx;
		float dy = points[i].getY() - yy;
		float dest = sqrt(dx * dx + dy * dy);
		if (dest <= jumpRange)
		{
			tmp.push_back(points[i]);
			//points.erase(points.begin() + i);
			//std::vector<Point>(points).swap(points);
		}
	}
	return tmp;
}
