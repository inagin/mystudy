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
	int label; //行番号 or 列番号
	int group; //グループ番号
	string name; //名前
};

//新たなノードを作成し、返却する
Node* initNode(){
	Node* node = new Node;
	
	//初期化
	node->L = node->R = node->U = node->D = node->C = nullptr;
	node->size = 0;
	node->label = 0;
	node->group = 0;
	node->name = "";

	return node; 
}

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