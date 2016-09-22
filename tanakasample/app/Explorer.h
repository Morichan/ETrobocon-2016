#ifndef _INC_EXPLORER
#define _INC_EXPLORER


#include<bits/stdc++.h>
#include<vector>
#include"Node.h"
#include"ev3api.h"
#include"util.h"
#include"Block.h"

using namespace std;


class Explorer{
private:
	vector<Node*> nodeList;
	Node* startNode;
	Node* goalNode;
	vector<int> root;
	vector<Block*> blockList;

public:
	Explorer();
	void set(int start, int goal);
	void print();
	void search();
	void setBlocks(int p1, int p2, int p3, int p4);
    int getRoot(int i);
    size_t getSize();
	int goalExplore(int nowNodeNum, char* color);
    bool contains(int num);

};
#endif //INC_EXPLORER
