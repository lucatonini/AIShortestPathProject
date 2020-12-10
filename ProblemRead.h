#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <algorithm> 
struct Cell {
	int x, y, cost;// x location y location and initall check is false because we didnt start
	bool check = false;

};
struct movement {
	Cell m[4] = { {1,0},{-1,0},{0,-1},{0,1} };
	/*
	right +1,0
	left -1,0
	up 0,-1
	down 0,1
	*/
};
using CellList = std::vector<Cell>;
class Queue {
public:
	void AddQueue() {
		open.push_back({});
	}
	void AddElement(Cell element) {
		if (open.empty()) 
			open.push_back({});
		auto &b = open.back();
		int index = find(element, b);
		if (index!=-1) {
			if (element.cost < b[index].cost)
				b[index] = element;		
		}
		else 
			b.push_back(element);

		
		sort(b);		
	}
	
	int  find(Cell element,std::deque<Cell> &b) {

		auto index = std::find_if(b.begin(), b.end(), [&](const Cell node){
			//node is the element of queue
			return (node.x == element.x && node.y == element.y);

		});
		if (index != b.end()) {
			std::cout << "yes Found" << std::endl;
			return (std::distance(b.begin(), index));

		}
		
		else {
			std::cout << "can not found" << std::endl;

			return -1;
		}
	}
	void sort(std::deque<Cell> &b) {
		std::sort(b.begin(), b.end(), [](const Cell &node1, const Cell &node2)->bool {
			return node1.cost < node2.cost;
		});

	}
	Cell getlast() {
		auto &b = open.back();
		return b.back();
	}
	bool finish(Cell current,Cell finish) {
		if (current.x == finish.x && current.y == finish.y)
			return true;

		return false;
	}
	Cell Pop(int i) {
		return open[i].front();
	}

	void backtrack() {
		Cell last;
		path.clear();
		for (int i = open.size()-1; i >=0 ; i--) {
			if (i == open.size() - 1) {
				path.push_back(open[i].front());
				last = open[i].front();
			}
			int index = find(last, open[i]);
			if (index != -1) {
				if (last.cost != open[i][index].cost) {
					last = open[i].front();
					path.push_back(open[i].front());
				
				}
				
			}
			else {
				path.push_back(open[i].front());
				last = open[i].front();
			}
		}
		
	}
	void printpath() {

		for (auto &l : path) {
			std::cout << "[" << l.x << "] [" << l.y << "] --->";
		}
		std::cout<<std::endl;

	}
	
private:
	std::vector<std::deque<Cell>> open;
	std::deque<Cell> close;//close list only for getting cost end of the list;
	std::vector<Cell> path;


};
class Problem:public Queue {
public:
	Problem(int n):n(n) {
		std::cout << "Problem Created" << std::endl;




	}
	bool valid(int x, int y, Cell dir) {
		if (x + dir.x >= 0 && x + dir.x < n && y + dir.y >= 0 && y + dir.y < n )
			return true;
		else
			return false;

	}


	void ReadFile(){
        std::ifstream problem;
	problem.open("Resources/problem.txt");//Directory WILL CHANGE
	if (problem.is_open()) {

		std::string  path;
		int i = 0;
		list2.push_back({});

		list.push_back({});
		while (problem >> path) {
			auto &back = list.back();
			auto &listback = list2.back();
			std::string value(path.begin() + 1, path.end());
			if (path[0] != 'w') {
				if (path[0] == 'x')
					back.x = std::stoi(value);
				else if (path[0] == 'y')
					back.y = std::stoi(value);
				
			}
			else {
				back.cost = std::stoi(value);
				if (back.x != n-1) {
					listback.push_back(back);
				}
				else
				{
					listback.push_back(back);
					list2.push_back({});
				}
				list.push_back({});
			}
		}
	}
		
	else
		std::cout << "cannot open";

    };
	void search(Cell start,Cell end){
        movement direction;
	AddElement(start);
	std::cout << "Search unit test " << std::endl;
	if (list2.empty())
	{
		std::cout << "list is empty can not search anything" << std::endl;
		return;
	}
	for (int y = 0; y < list2.size(); y++) {
		for (int x = 0; x < list2[y].size(); x++) {
			// here we have to control valid or not valid movement for each 
			// then we have to push open list other wise we can not trace all path.

			
			std::cout << "[" << x << "] [" << y << "]  cost =" << list2[y][x].cost << std::endl;


		}
		std::cout << std::endl;

	}
    };
	void WriteFile();
	

private:
	CellList list;
	std::vector<CellList>list2;
	int n;// n for table size;
};

