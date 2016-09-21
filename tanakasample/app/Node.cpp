
#include"Node.h"

//コンストラクタ
Node::Node(int num){
	state = '*';
	nodeNum = num;
}

//周囲のノードの設定
void Node::setEdge(int nodeNum){
	int target = nodeNum-5;

	for(int i=0; i<11; i++){
		if((i !=0 && i%4 == 3) || target+i == nodeNum){
			continue;
		}
		nextNode.push_back(target+i);
	}

	//隣り合わないノードの排除
	if(nextNode[0]%4 == 3 || nextNode[3]==-1 || nextNode[0] == -1){
		nextNode[0] = nextNode[3] = nextNode[5] = -1;
	}

	if(nextNode[2]%4 == 0){
		nextNode[2] = nextNode[4] = nextNode[7] = -1;
	}
	
	int count = 0;
	while(count < (int )nextNode.size()){
		if(nextNode[count] < 0 || nextNode[count] > 15){
			nextNode.erase(nextNode.begin()+count);
			count = count-1;
		}
		count++;
	}

}

void Node::setState(char blockState){
	state = blockState;
}

char Node::getState(){
	return state;
}

//ゴールまでの距離の計算
void Node::distance(Node* goalNode){
	int x = (nodeNum%4 - goalNode->nodeNum%4)*(nodeNum%4 - goalNode->nodeNum%4);
	int y = (nodeNum/4 - goalNode->nodeNum/4)*(nodeNum/4 - goalNode->nodeNum/4);
	cost = x+y;

}

vector<int> Node::getNextNode(){
	return nextNode;
}

int Node::getNodeNum(){
	return nodeNum;
}

int Node::getCost(){
	return cost;
}
