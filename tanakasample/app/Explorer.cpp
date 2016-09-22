
#include"Explorer.h"

//コンストラクタ
Explorer::Explorer(){
	for(int i=0; i<16; i++){
	nodeList.push_back(new Node(i));
	nodeList[i]->setEdge(i);
	}
}

//スタート＆ゴール位置設定
void Explorer::set(int start, int goal){
	nodeList[start]->setState('s');
	nodeList[goal]->setState('g');

	for(int i=0; i<16; i++){
		nodeList[i]->setCost(nodeList[i]->distance(nodeList[goal]));
	}

	startNode = nodeList[start];
	goalNode = nodeList[goal];
}


vector<int> Explorer::search(){
	Node* target = startNode;
	Node* next = startNode;
	root.push_back(target->getNodeNum());

	while(next != goalNode){
		int nodeCost = 100;
		root.push_back(target->getNodeNum());
		for(int i=0; i<(int )target->getNextNode().size(); i++){
			if(nodeCost > nodeList[target->getNextNode()[i]]->getCost() && nodeList[target->getNextNode()[i]]->getState() != 'b' && !contains((nodeList[target->getNextNode()[i]])->getNodeNum())){
				root.pop_back();
				nodeCost = nodeList[target->getNextNode()[i]]->getCost();
				next = nodeList[target->getNextNode()[i]];
				root.push_back(next->getNodeNum());
			}
		}
		target = next;
	}

	FILE* fp = fopen("count.txt", "w");

	for(int i=0; i<(int )root.size(); i++){
		fprintf(fp, "%d ", root[i]);
	}
	fclose(fp);
	
	return root;
}

void Explorer::setBlocks(int p1, int p2, int p3, int p4){
	nodeList[p1]->setState('b');
	nodeList[p2]->setState('b');
	nodeList[p3]->setState('b');
	nodeList[p4]->setState('b');
}

//スタート地点から一番近い指定の色のノードを探す
int Explorer::goalExplore(int nowNodeNum, char* color){
	int goalCost=100;
	Node* target;
	int goal=0;

	for(int i=0; i<(int)nodeList.size(); i++){
		target = nodeList[i];
		if(target->distance(nodeList[nowNodeNum]) < goalCost && strcmp(color, target->getNodeColor()) == 0 && target->getState() != 'b'){
			goal = target->getNodeNum();
			goalCost = target->getCost();
		}
	}

	return goal;
}

bool Explorer::contains(int num){
	for(int i=0; i<(int)root.size(); i++){
		if(root[i] == num){
			return true;
		}
	}
	return false;
}
