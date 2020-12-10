#pragma once

#include "Queue.h"
#include <sstream>
#include <string>
using multidimensional = std::vector<Cell>;
class Problem {
public:
    Problem(int n):n(n) {
        std::cout << "Base Created" << std::endl;
    }
	
    void PrintAsGrid(){
        for (int y = 0; y < n; y++) {
            for (int x = 0; x < n; x++) {
                
                std::cout << gridPoints[y*n+x].cost << "\t";
            }
            std::cout << std::endl;
        }
    }
	int find(Cell c)
	 {
		auto index = std::find_if(gridPoints.begin(), gridPoints.end(), [&c](const Cell el) {
			return (el.x == c.x) && (el.y == c.y);
		});
		if (index != gridPoints.end())
			return std::distance(gridPoints.begin(), index);
		else
			return -1;
	}
	void readFormattedFile() {
		//initialize();
		std::ifstream problem;
		problem.open("/home/luca/projects/AIShortestPath/Resources/problem.txt");//Directory WILL CHANGE
		if (problem.is_open()) {
			std::string path, readed;
			std::cout << "File Open" << std::endl;
			Cell back;
			int count = 0;
			while (problem >> readed) {
				if (count == 0) {
					path = readed;
					std::cout << path << std::endl;
				}
				else if (count == 1) {
					workingtime = std::stoi(readed);
					std::cout << "working times = " << workingtime << std::endl;
				}
				else {
					/*
					Cell x;
					std::getline(problem, v);
					StartEndPoints.push_back({ readed[0],readed[1],-1 ,-1});
					std::cout << count - 1 << ". Start End " << readed<<" "<< v << std::endl;
					*/
					std::string v, line;
					std::getline(problem, line);
					std::stringstream check(line);
					int elimination = 0;
					Cell starts;
					Cell ends;
					while (std::getline(check, v, ' ')) {
						if (elimination == 1)
							starts.x = std::stoi(v);
						else if (elimination == 2)
							starts.y = std::stoi(v);
						else if (elimination == 3)
							ends.x = std::stoi(v);
						else if (elimination == 4)
							ends.y = std::stoi(v);
						elimination++;
					}

					Start.push_back(starts);
					End.push_back(ends);
				
				}
				count++;

			}
			std::cout << "start End test : " << std::endl;
			for (int i = 0; i < Start.size(); i++) {
				std::cout << "x = " << Start[i].x << " y = " << Start[i].y << std::endl;
			}
			for (int i = 0; i <End.size(); i++) {
				std::cout << "x = " << End[i].x << " y = " << End[i].y << std::endl;
			}
			// we need parcing
			int check = 0;//check=0 open,1close,2comma,3 ;
			std::string num;

			for (int i = 0; i < path.size();) {
				if (path[i] == '(') {
					check = 0;
					i++;
				}
				else if (path[i] == ')') {
					check = 1;
					std::cout << back.x << " , " << back.y << " " << back.cost << std::endl;
					int index = find(back);
					if (index != -1)
						gridPoints[index] = back;
					num.clear();
					check = 0;
					i++;


				}
				else if (path[i] == ',') {
					check = 2;
					i++;
				}
				else if (path[i] == ';') {
					check = 3;
					i++;
				}
				else {
					int oldcheck = check;
					check = 4;
					num.push_back(path[i]);
					int j = i + 1;
					while (path[j] != ')'&&path[j] != '('&&path[j] != ','&&path[j] != ';') {
						num.push_back(path[j]);
						j++;
					}
					if (oldcheck == 0) {
						back.x = std::stoi(num);
						num.clear();

					}
					else if (oldcheck == 2) {
						back.y = std::stoi(num);
						num.clear();

					}
					else if (oldcheck == 3) {
						back.cost = std::stoi(num);
						num.clear();

					}
					else {
						num.clear();
					}
					
					i++;


				}




			}
			std::cout << "Grid Created!" << std::endl;
			for (auto &x : gridPoints) {
				std::cout << "[" << x.x << "] [" << x.y << "] cost = " << x.cost << std::endl;

			}
			setPath();
			std::cout << "Path Created!" << std::endl;
		}
		else
			std::cout << "cannot open";

	}
	void Initialize() {

		for (int y = 0; y < n; y++) {
			for (int x = 0; x < n; x++)
			{
				Cell  np;
				np.x = x;
				np.y = y;
				np.cost = -1;
				gridPoints.push_back(np);
			}
			
		}
	}
	void ReadFile(){
        std::ifstream problem;
        problem.open("/home/luca/projects/AIShortestPath/Resources/problem.txt");//Directory WILL CHANGE
        if (problem.is_open()) {
            std::string path;
            std::cout<< "File Open" << std::endl;
			Cell back;

            while (problem >> path) {
				//std::cout << path << std::endl;
               std::string value(path.begin() + 1, path.end());
            
                if (path[0] != 'w') {
                    if (path[0] == 'x')
                        back.x = std::stoi(value);
                    else if (path[0] == 'y')
                        back.y = std::stoi(value);
                }
                else {
                    back.cost = std::stoi(value);
                    gridPoints.push_back(back);
                }
				
            }
			// we need parcing
			int check=0;//check has to be 3 for end points each end points check will 0
		
            std::cout << "Grid Created!" << std::endl;
            setPath();
            std::cout << "Path Created!" << std::endl;
        }
        else
            std::cout << "cannot open";
    }
	void PrintBlock() {
		std::ofstream map;
		map.open("blockmap.txt");
		for (auto &p : gridPoints) {
			if(p.cost==-1)
				map << '{' << p.x << ',' << p.y << '}' << ',';
		}
		map.close();

	}
	int cost() {
		return shortestPath.back().cost;
	}
    void search(const Cell start, const Cell end){
		shortestPath.clear();
		
        grid.path(start,end);
        grid.printOpenVec();
        grid.printCloseVec();
    }
	void backtracking() {
		shortestPath.clear();
		auto open = grid.GetClose();
		int i = open.size() - 1;
		
		while (open[i].link != -1) {
			shortestPath.push_back(open[i]);
			i = open[i].link;
		}
		shortestPath.push_back(open[i]);//for start from the starting point
		std::reverse(shortestPath.begin(), shortestPath.end());//because our vectoro is working like stack our starting point end of the vector.
		totalcosts.push_back(cost());


	}
	std::vector<Cell>GetShortestPath() {
		return shortestPath;
	}
	void printpath() {
		auto shortestpath = GetShortestPath();
		auto closelist = grid.GetClose();
		std::cout << "\n \n shortest path : " << std::endl;
		for (auto &l : shortestPath) {
			std::cout << "[" << l.x << "] [" << l.y << " ]" << " link = " << l.link << std::endl;
		}
		std::cout << "Cost is = " << closelist.back().cost << std::endl;

	}
	void pushcost(int c) {
		totalcosts.push_back(c);
	}
	int gettotalcost( int index) {
		return totalcosts.at(index);
	}
	void Export() {

		std::ofstream path;
		std::ofstream formatted;
		path.open("shortestpath.txt");
		formatted.open("formatted.txt");
		if (!path.is_open()&&!formatted.is_open())
			throw;
		//for (auto &p : shortestPath) {
		for (int i = 0; i < exportpath.size(); i++) {
			path << i + 1 << "." << " ";
			formatted << i + 1 << "." << " " << exportpath[i].size() - 1 << ", ";
			for (int j = 0; j < exportpath[i].size(); j++) {
				path << '{' << exportpath[i][j].x << ',' << exportpath[i][j].y << '}';
				formatted << '(' << exportpath[i][j].x << ',' << exportpath[i][j].y << ';' << exportpath[i][j].cost << ')';
				if (j != exportpath[i].size() - 1)
					path << ',';
				
			}
			path << std::endl;
			formatted << std::endl;
			//totalcosts.push_back(grid.GetTotalCost());
			
		}
		path << std::endl;
		for (int i = 0; i < totalcosts.size(); i++) {
			path << i+1 << ". " << totalcosts[i] << std::endl;
		}


	/*	for(int i=0;i<shortestPath.size();i++){
			path << '{' << shortestPath[i].x << ',' << shortestPath[i].y << '}';
			if (i != shortestPath.size() - 1)
				path << ',';
		}*/
		//path << std::endl << grid.GetTotalCost();

		path.close();
		formatted.close();
	}
    
	int GetGridSize() {
		return n;
	}
	std::vector<Cell>GetGrid() {
		return gridPoints;
	}
	int GetStartinputSize() {
		return Start.size();
	}
	int GetEndinputSize() {
		return End.size();
	}
	void pushexport(std::vector<Cell> path) {
		exportpath.push_back(path);
	}
	std::vector<Cell> GetStarts() {
		return Start;
	}
	std::vector<Cell> GetEnds() {
		return End;
	}
	int getworking() {
		return workingtime;
	}
	
	void resetPaths(){
        grid.open.clear();
        grid.close.clear();
    }
private:
    std::vector<Cell> gridPoints;
    std::vector<Cell> shortestPath;
	std::vector<multidimensional> exportpath;
	std::vector<int>totalcosts;
	int workingtime;
	std::vector<Cell>Start;
	std::vector<Cell>End;
    int n;// n for table size;
    Queue grid;
    
    void setPath(){
        grid.setNodes(gridPoints);
    }
    
    

};
