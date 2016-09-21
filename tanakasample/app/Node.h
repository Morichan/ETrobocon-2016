#ifndef _INC_NODE
#define _INC_NODE

#include<cstdlib>
#include<vector>

using namespace std;

class Node{
private:
	int nodeNum;
	char state;
	vector<int> nextNode;
	int cost;

public:
	Node(int num);
	void setEdge(int nodeNum);
	void setState(char state);
	char getState();
	void distance(Node* goalNode);
	vector<int> getNextNode();
	int getNodeNum();
	int getCost();

};

#endif //INC_NODE
