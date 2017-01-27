#include "iostream"
#include "Input.h"
#include "bfs.h"
#include <cstdlib>
#include <ctime>
#include <fstream>

int main(int argc, char* argv[])
{
	int executionMode = 0;
	std::cout << "Choose execution mode: " << std::endl;
	std::cout << "1) Your own values" << std::endl;
	std::cout << "2) Read values from file" << std::endl;
	std::cout << "3) Generate random values and time check " << std::endl;
	std::cin >> executionMode;
	//vector that contains animals
	std::vector<Point> animals;
	int width = 0;
	int meerkatsNumber = 0;
	int crocodilesNumber = 0;
	int jumpRange = 0;
	int crocodileLength = 0;

	if (executionMode == 1)
	{
		std::cout << "Choose number of meerkats: " << std::endl;
		std::cin >> meerkatsNumber;
		std::cout << "Choose number of crocodiles: " << std::endl;
		std::cin >> crocodilesNumber;
		std::cout << "Choose width of the river: " << std::endl;
		std::cin >> width;
		std::cout << "Choose range of the meerkat jump: " << std::endl;
		std::cin >> jumpRange;
		std::cout << "Choose crocodile length: " << std::endl;
		std::cin >> crocodileLength;

		AABB river(Point(0, 0), Point(meerkatsNumber > width ? (meerkatsNumber - 1) : (width - 1), width - 1));
		Quadtree* quadtree = new Quadtree(river);
		for (int i = 0; i < meerkatsNumber; i++)
		{
			animals.push_back(Point(i, 0, i));
			quadtree->insert(Point(i, 0, i));
		}
		std::cout << "Lets set crocodiles: " << std::endl;
		for(int i = 0; i < crocodilesNumber; i++)
		{
			int x1 = 0;
			int y1 = 0;
			int x2 = 0;
			int y2 = 0;
			bool isInRange = false;
			std::cout << "Give two numbers(x1 , y1) for crocodile nr." << i + 1 <<" begin:" << std::endl;
			std::cin >> x1 >> y1;
			std::cout << "Give two numbers(x2 , y2) for crocodile nr." << i + 1 << " end:" << std::endl;
			std::cin >> x2 >> y2;
			if (width - 1 - y1 <= jumpRange || width - 1 - y2 <= jumpRange)
				isInRange = true;
			animals.push_back(Point(x1, y1, meerkatsNumber + i, isInRange));
			animals.push_back(Point(x2, y2, meerkatsNumber + i, isInRange));
			quadtree->insert(Point(x1, y1, meerkatsNumber + i, isInRange));
			quadtree->insert(Point(x2, y2, meerkatsNumber + i, isInRange));
		}
		unsigned int start_time = clock();
		bfs(quadtree, animals, jumpRange, meerkatsNumber);
		unsigned int end_time = clock();
		unsigned int search_time = end_time - start_time;
		std::cout << search_time << std::endl;

	}
	else if (executionMode == 2)
	{
		int width = 0;
		int meerkatsNumber = 0;
		int crocodilesNumber = 0;
		int jumpRange = 0;
		int crocodileLength = 0;
		
		int firstX = 0;
		int firstY = 0;
		int secondX = 0;
		int secondY = 0;
		std::vector<Point> crocodiles;
		bool data = true;

		std::ifstream file("test.txt");
		//std::ifstream file("test2.txt");

		while (!file.eof()) {
			if (data) 
			{
				file >> width >> meerkatsNumber >> crocodilesNumber >> jumpRange >> crocodileLength;
				data = false;
			}
			else 
			{
				file >> firstX >> firstY >> secondX >> secondY;
				crocodiles.push_back(Point(firstX, firstY));
				crocodiles.push_back(Point(secondX, secondY));
			}
		}
		file.close();
		AABB river(Point(0, 0), Point(meerkatsNumber > width ? (meerkatsNumber - 1) : (width - 1), width - 1));
		Quadtree* quadtree = new Quadtree(river);
		for (int i = 0; i < meerkatsNumber; i++)
		{
			animals.push_back(Point(i, 0, i));
			quadtree->insert(Point(i, 0, i));
		}
		for (int i = 0; i < crocodiles.size(); i+=2) {
			bool isInRange = false;
			if (width - 1 - crocodiles[i].getY() <= jumpRange || width - 1 - crocodiles[i+1].getY() <= jumpRange)
				isInRange = true;
			animals.push_back(Point(crocodiles[i].getX(), crocodiles[i].getY(), meerkatsNumber + i/2, isInRange));
			quadtree->insert(Point(crocodiles[i].getX(), crocodiles[i].getY(), meerkatsNumber + i/2, isInRange));
			animals.push_back(Point(crocodiles[i+1].getX(), crocodiles[i+1].getY(), meerkatsNumber + i/2, isInRange));
			quadtree->insert(Point(crocodiles[i+1].getX(), crocodiles[i+1].getY(), meerkatsNumber + i/2, isInRange));
		}
		/*for (size_t i = 0; i < animals.size(); i++)
		{
			std::cout << animals[i].getX() << " " << animals[i].getY() << "    " << animals[i].getNumber() << " " << animals[i].getIsInRange() << std::endl;
		}*/
		bfs(quadtree, animals, jumpRange, meerkatsNumber);

	}
	else if (executionMode == 3)
	{
		int i = 0;
		std::cout << "Choose test:" << std::endl;
		std::cout << "1) meerkats [10,1000] & crocodile 1" << std::endl;
		std::cout << "2) meerkats 100 & crocodile [1,100]" << std::endl;
		std::cin >> i;
		if (i == 1) {
			std::cout << "Meerkat" << "\t" << "t[ms]" << "\t" << "q" << std::endl;
			test1(10);
			test1(25);
			test1(50);
			test1(75);
			test1(100);
			test1(250);
			test1(500);
			test1(1000);
		}
		else {
			std::cout << "Crocod" << "\t" << "t[ms]" << "\t" << "q" << std::endl;
			test2(1);
			test2(5);
			test2(10);
			test2(15);
			test2(20);
			test2(25);
			test2(30);
			test2(35);
			test2(40);
			test2(45);
			test2(50);
			test2(55);
			test2(60);
			test2(65);
			test2(70);
			test2(75);
			test2(80);
			test2(85);
			test2(90);
			test2(95);
			test2(100);
		}
	}

	/*for (size_t i = 0; i < animals.size(); i++)
	{
		std::cout << animals[i].getX() << " " << animals[i].getY() << "    " << animals[i].getNumber() << " " << animals[i].getIsInRange() << std::endl;
	}
	std::cout << "Setting animals --- OK" << std::endl;*/

	system("pause");
	return 0;
}