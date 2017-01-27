#include "iostream"
#include "Input.h"
#include "bfs.h"
#include <vector>
#include <ctime>
#include <cmath>
#include <algorithm>
#include "Quadtree.h"

#define PI 3.14159265

std::vector<Point> setAnimals(int width, int meerkatsNumber, int crocodileNumber, int jumpRange, int crocodileLength, Quadtree* quadtree)
{
	srand(time(0));
	//vector for checking if crocodiles are crossing
	std::vector<Point> test;
	int length = meerkatsNumber > width ? meerkatsNumber : width;
	//setting meerkats
	for (size_t i = 0; i < length; i++)
	{
		test.push_back(Point(i, 0, i));
		quadtree->insert(Point(i, 0, i));
	}
	//setting crocodiles
	for (size_t i = 0; i < crocodileNumber; i++)
	{
		//std::cout << i << std::endl;
		//crocodile begin
		int x1 = 0;
		int y1 = 0;
		//crocodile end
		int x2 = 0;
		int y2 = 0;
		//crocodile angle
		int angle = 0;

		//if crocodile is in range of meerkat jump to the opposite shore of the river - true
		bool isInRange = false;

		//if crocodile is ok - true
		bool check = false;

		//if crocodiles are crossing - true
		bool flag = false;
		while (!check)
		{
			x1 = rand() % (length -1);
			y1 = 1 + rand() % (width - 2);
			angle = rand() % 360;
			double sinus = sin(angle*PI / 180);
			double cosinus = cos(angle*PI / 180);
			x2 = crocodileLength * cosinus + x1;
			y2 = crocodileLength * sinus + y1;
			//checking all crocodiles
			for (size_t k = 0; k < test.size()-length; k+=2)
			{
				flag = areCrossing(Point(x1, y1), Point(x2, y2), test[length+k], test[length+k+1]);
				if (flag)
					break;
			}
			//if position of this crocodile is ok - leave while
			if (!flag && x2>=0 && x2<=length-1 && y2>=1 && y2<=width-2)
				check = true;

		}
		if (width - 1 - y1 <= jumpRange || width - 1 - y2 <= jumpRange)
			isInRange = true;
		//insert crocodile in vector and quadtree
		test.push_back(Point(x1, y1, length + i, isInRange));
		test.push_back(Point(x2, y2, length + i, isInRange));
		quadtree->insert(Point(x1, y1, length + i, isInRange));
		quadtree->insert(Point(x2, y2, length + i, isInRange));
	}
	return test;
}

bool areCrossing(Point firstBegin, Point firstEnd, Point secondBegin, Point secondEnd)
{
	if (firstBegin == secondBegin || firstBegin == secondEnd || firstEnd == secondBegin || firstEnd == secondEnd)
		return true;
	float d1 = crossProduct(secondEnd - secondBegin, firstBegin - secondBegin);
	float d2 = crossProduct(secondEnd - secondBegin, firstEnd - secondBegin);
	float d3 = crossProduct(firstEnd - firstBegin, secondBegin - firstBegin);
	float d4 = crossProduct(firstEnd - firstBegin, secondEnd - firstBegin);

	float d12 = d1*d2;
	float d34 = d3*d4;

	if (d12 > 0 || d34 > 0)
		return false;

	if (d12 < 0 || d34 < 0)
		return true;

	return onRectangle(firstBegin, secondBegin, secondEnd) || onRectangle(firstEnd, secondBegin, secondEnd) 
		|| onRectangle(secondBegin, firstBegin, firstEnd) || onRectangle(secondEnd, firstBegin, firstEnd);
}

float crossProduct(Point left, Point right)
{
	float product = left.getX()*right.getY() - right.getX()*left.getY();
	return product;
}

bool onRectangle(Point q, Point p1, Point p2)
{
	return std::min(p1.getX(), p2.getX()) <= q.getX() && q.getX() <= std::max(p1.getX(), p2.getX()) 
		&& std::min(p1.getY(), p2.getY()) <= q.getY() && q.getY() <= std::max(p1.getY(), p2.getY());
}

void test1(int meerkatsNumber)
{
	double c = 0.03;
	double q = 0.0;
	unsigned int time = meerkatsNumber*meerkatsNumber*log(meerkatsNumber);// n^2*logn
	std::vector<Point> animals;
	int width = 6;
	int crocodilesNumber = 1;
	int jumpRange = 1;
	int crocodileLength = 3;

	AABB river(Point(0, 0), Point(meerkatsNumber > width ? (meerkatsNumber - 1) : (width - 1), width - 1));
	Quadtree* quadtree = new Quadtree(river);

	for (int i = 0; i < meerkatsNumber; i++)
	{
		animals.push_back(Point(i, 0, i));
		quadtree->insert(Point(i, 0, i));
	}
	animals.push_back(Point(0, 1, meerkatsNumber, true));
	animals.push_back(Point(0, 4, meerkatsNumber, true));
	quadtree->insert(Point(0, 1, meerkatsNumber, true));
	quadtree->insert(Point(0, 4, meerkatsNumber, true));

	unsigned int start_time = clock();
	bfs(quadtree, animals, jumpRange, meerkatsNumber);
	unsigned int end_time = clock();
	unsigned int search_time = end_time - start_time;
	q = search_time / (c*time);
	std::cout << meerkatsNumber << "\t" << search_time << "\t" << q << std::endl;
}

void test2(int crocodilesNumber)
{
	double c = 0.025;
	double q = 0.0;
	unsigned int time = 10000*log(100+crocodilesNumber*2);// O(100*(100)log(100+m*2))
	std::vector<Point> animals;
	int width = 6;
	int meerkatsNumber = 100;
	int jumpRange = 1;
	int crocodileLength = 3;

	AABB river(Point(0, 0), Point(meerkatsNumber > width ? (meerkatsNumber - 1) : (width - 1), width - 1));
	Quadtree* quadtree = new Quadtree(river);

	for (int i = 0; i < meerkatsNumber; i++)
	{
		animals.push_back(Point(i, 0, i));
		quadtree->insert(Point(i, 0, i));
	}
	animals.push_back(Point(0, 1, 100, true));
	animals.push_back(Point(0, 4, 100, true));
	quadtree->insert(Point(0, 1, 100, true));
	quadtree->insert(Point(0, 4, 100, true));
	for (int i = 0; i < crocodilesNumber; i++)
	{
		int x1 = i;
		int y1 = 2;
		int x2 = i;
		int y2 = 4;
		bool isInRange = false;
		if (width - 1 - y1 <= jumpRange || width - 1 - y2 <= jumpRange)
			isInRange = true;
		animals.push_back(Point(x1, y1, meerkatsNumber + 1 + i, isInRange));
		animals.push_back(Point(x2, y2, meerkatsNumber + 1 + i, isInRange));
		quadtree->insert(Point(x1, y1, meerkatsNumber + 1 + i, isInRange));
		quadtree->insert(Point(x2, y2, meerkatsNumber + 1 + i, isInRange));
	}

	unsigned int start_time = clock();
	bfs(quadtree, animals, jumpRange, meerkatsNumber);
	unsigned int end_time = clock();
	unsigned int search_time = end_time - start_time;
	q = search_time / (c*time);
	std::cout << crocodilesNumber << "\t" << search_time << "\t" << q << std::endl;
}