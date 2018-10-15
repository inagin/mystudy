#include <iostream>
#include <string>
#include <vector>

using namespace std;

//Dancing link のノード
struct Node{
	Node* L;
	Node* R;
	Node* U;
	Node* D;
	
	Node* C; //列のヘッダーへのポインタ
	int size; //サイズ
	int label; //ヘッダの場合は列、ヘッダ以外の場合は行の番号
	int group; //グループ番号
};

//新たなノードを作成し、返却する
Node* initNode(){
	Node* node = new Node;
	
	//初期化
	node->L = node->R = node->U = node->D = node->C = nullptr;
	node->size = -1;
	node->label = -1;
	node->group = -1;

	return node; 
}

void dumpNode(Node* head){
	cout << "Dump Node: " << endl;
	Node* tmp = head->R;

	for(Node* tmp2 = tmp->D; tmp2->D == tmp; tmp2 = tmp2->D){
		cout << " " << tmp2->label << ":";
		for(Node* tmp3 = tmp2; tmp3->R == tmp2; tmp3 = tmp3->R){
			if(tmp3 != head)
					cout << " (" << tmp3->label << ")";
		}
		cout << endl;
	}
	cout << endl;
}

/*
//行のグループを示すリスト(要検討)
struct RNode{
	RNode* Prev;
	RNode* Next;

	vector<Node*> nodes; //同じグループのノード行のヘッダを示す
	int group; //グループのID
};


//新たな行ノードを返す
RNode* initRNode(){
	RNode* node = new RNode;

	node->Prev = nullptr;
	node->Next = nullptr;

	node->group = 0;

	return node;
}

//RNodeから特定のグループを探す
RNode* searchRNode(RNode* head, int g){
	for(RNode* tmp = head->Next; tmp != nullptr; tmp = tmp->Next){
		if(tmp->group == g)
			return tmp;
	}
	return nullptr;
}

void dumpRNode(RNode* head){
	cout << "Dump Test: " << endl;

	for(RNode* tmp = head->Next; tmp != nullptr; tmp = tmp->Next){
		cout << tmp->group << " :";
		for(auto x : tmp->nodes)cout << x->label << " ";
		cout << endl;
	}
	cout << endl;
}

//後ろにプッシュする
void pushRNode(RNode* head, RNode* x){
	RNode* tmp;

	for(tmp = head ; tmp->Next != nullptr; tmp = tmp->Next);

	tmp->Next = x;
	x->Prev = tmp;
}

*/