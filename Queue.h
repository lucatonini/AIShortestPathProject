#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include <algorithm> 

struct Cell{
	int x, y, cost, link;// x location & y location with cost and link to previous node 
    Cell():cost(0),link(-1){};
    Cell(int x, int y):x(x),y(y),cost(0),link(-1){};
    Cell(int x, int y, int cost, int link):x(x),y(y),cost(cost),link(link){};
};

class Queue {
public:

    //Queue(std::vector<Cell> nodes):nodes(nodes){};
    void setNodes(std::vector<Cell> nodes){
        this->nodes = nodes;
    }
    
    void path(Cell start, Cell end){
		close.clear();
		
        int index;
        Cell temp(start.x,start.y,0,-1), addTemp(0,0,0,-1);
        addOpen(start);
        addClose(addTemp);
        while(!((close.back().x == end.x) && (close.back().y == end.y))){
            temp = pop();
            addClose(temp);
            index = findInClose(temp);
            
            if(findInClose(move(temp,Right)) == -1){
                addTemp = move(temp,Right);
                addTemp.cost += temp.cost;
                addTemp.link = index;
                addOpen(addTemp);
            }
            if(findInClose(move(temp,Left)) == -1){
                addTemp = move(temp,Left);
                addTemp.cost += temp.cost;
                addTemp.link = index;
                addOpen(addTemp);
            }
            if(findInClose(move(temp,Up)) == -1){
                addTemp = move(temp,Up);
                addTemp.cost += temp.cost;
                addTemp.link = index;
                addOpen(addTemp);
            }
            if(findInClose(move(temp,Down)) == -1){
                addTemp = move(temp,Down);
                addTemp.cost += temp.cost;
                addTemp.link = index;
                addOpen(addTemp);
            }
        }
	
    }
    
    void printNodes(){
        std::cout << "Nodes are printed below" << std::endl;
        
        for (auto &l : nodes) {
            std::cout << "[" << l.x << "] [" << l.y << "] ---> cost = " << l.cost<<std::endl;
        }
    }
	
    void printOpenVec(){
        std::cout << "Open Cells are printed below" << std::endl;
        
        for (auto &l : open) {
            std::cout << "[" << l.x << "] [" << l.y << "] ---> cost = " << l.cost << " link = " << l.link<<std::endl;
        }
    }
	std::vector<Cell>GetOpen() {

		return open;
	}
	std::vector<Cell>GetClose() {
		return close;
	}
    void printCloseVec(){
        
        std::cout << "Closed Cells are printed below" << std::endl;
        int i = 0;
        for (auto &l : close) {
            std::cout << "(" << i <<"):-[" << l.x << "] [" << l.y << "] ---> cost = " << l.cost << " link = " << l.link << std::endl;
            i++;
        }
    }
	int GetTotalCost() {

		return close.back().cost;
	}
    std::vector<Cell> open;
    std::vector<Cell> close;
    std::vector<Cell> nodes;
    
private:
    
    Cell Right = *new Cell(1,0);
    Cell Left = *new Cell(-1,0);
    Cell Up = *new Cell(0,-1);
    Cell Down = *new Cell(0,1);
    
    void addClose(Cell element) {
        close.push_back(element);
    }
    
    void addOpen(Cell element) {
        open.push_back(element);
        sort(open);
    }

    int findInClose(Cell look) {
      /*  int index = 0;
        for(auto i: close){
            if(look.x == i.x && look.y == i.y)
                return index;
            index++;
        }*/
		//        std::sort(b.begin(), b.end(), [](const Cell &node1, const Cell &node2)->bool {

		auto index = std::find_if(close.begin(), close.end(), [&look](const Cell el) {
			return (el.x == look.x) && (el.y == look.y);
		});
		if (index != close.end())
			return std::distance(close.begin(), index);

        return -1;
    }
    
    int findInNodes(Cell look){
       /* int index = 0;
        for(auto i: nodes){
            if(look.x == i.x && look.y == i.y)
                return index;
            index++;
        }*/
		auto index = std::find_if(nodes.begin(), nodes.end(), [&look](const Cell el) {
			return (el.x == look.x) && (el.y == look.y);
		});
		if (index != nodes.end())
			return std::distance(nodes.begin(), index);
        return -1;
    }
    
    /*Function that returns the next cell in the direction
     * that you choose. If the cell cannot be accessed then the 
     * current Cell is returned.
     */
    Cell move(Cell current, Cell dir){
        Cell temp;
        temp.x = current.x + dir.x;
        temp.y = current.y + dir.y;
        temp.cost = 10000000;
        int i = findInNodes(temp);

        if( i == nodes.size() || i == -1 || nodes.at(i).cost == -1)
            return current;    
        
        return nodes.at(i);
    }

    void sort(std::vector<Cell> &b) {
        std::sort(b.begin(), b.end(), [](const Cell &node1, const Cell &node2)->bool {
            return node1.cost > node2.cost;
        });
    }

    Cell getLastInOpen() {
        return open.back();
    }

    Cell pop() {
        auto temp = open.back();
        open.pop_back();
        return temp;
    }
};
