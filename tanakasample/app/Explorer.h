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
	vector<int> search();
	void setBlocks(int p1, int p2, int p3, int p4);
	bool contains(int num);
	int goalExplore(int nowNodeNum, char* color);

};
#endif //INC_EXPLORER
