#include <iostream>
#include "Problem.h"
#include <algorithm>
bool check(const int x,const int y,const int n, std::vector<Cell> gridPoints) {//check input
	if (x > n || y > n)
		return false;
	auto index = std::find_if(gridPoints.begin(), gridPoints.end(), [&x,&y](const Cell el) {
		return (el.x == x) && (el.y == y)&& el.cost!=-1;
	});

	if (index != gridPoints.end())
		return true;
	
	return false;
}
int main(int argc, char **argv) {
	std::cout << "Main of the project will go here." << std::endl;
	int boardsize = 8;
	std::cout << "enter board size = ";
	//std::cin >> boardsize;
	Problem problem(boardsize);
	std::cout << "Created" << std::endl;
	problem.Initialize();
	problem.readFormattedFile();
	std::cout << "\nFile Read In" << std::endl;
	problem.PrintAsGrid();
	std::cout << "\nPrinted grid" << std::endl;
	std::cout << std::endl << std::endl;


	bool inter = false;
	/*while (!inter) {
		std::cout << "enter Start x point = ";
		std::cin >> start.x;
		std::cout << "enter Start y point = ";
		std::cin >> start.y;
		std::cout << "enter end x point = ";
		std::cin >> end.x;
		std::cout << "enter end y point = ";
		std::cin >> end.y;
		 inter = check(start.x, start.y, 12, problem.GetGrid()) && check(end.x, end.y, problem.GetGridSize(), problem.GetGrid());
			if(!inter)
				std::cout<<"Enter x and y again !!!"<<std::endl;

	}   */
	auto starts = problem.GetStarts();
	auto ends = problem.GetEnds();
	for (int i = 0; i < starts.size(); i++) {
		if (check(starts[i].x, starts[i].y, problem.GetGridSize(), problem.GetGrid()) && check(ends[i].x, ends[i].y, problem.GetGridSize(), problem.GetGrid())) {
			problem.search(starts[i],ends[i]);
			problem.backtracking();
			problem.pushexport(problem.GetShortestPath());
            problem.resetPaths();
		}
		else{
			std::cout<<i+1 << ". error x = " << starts[i].x << " y = " << starts[i].y << " end x = "<< ends[i].x <<  " "  << ends[i].y << " size = "<< problem.GetGridSize() << std::endl;
        }
	}

   /* problem.search(start,end);
	problem.backtracking();
	problem.printpath();
	*/
	problem.Export();
	problem.PrintBlock();
    std::cout<< "\nYay, I worked!" << std::endl;
	return 0;
}
