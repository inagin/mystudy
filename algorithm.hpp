#include <map>
#include <queue>
#include <algorithm>
#include "node.hpp"

/* Dancing-Links を使って Exact Cover Problem を解く *///左にあるノード、右にあるノードの順に指定
void connectLR(Node* A, Node* B){
	A->R = B;
	B->L = A;
}

//上にあるノード、下にあるノードの順に指定
void connectUD(Node* A, Node* B){
	A->D = B;
	B->U = A;
}

//0-1二次元配列から Dancing-Linkを構築
//各行のノードには name を設定する
Node* constructDL(int** instance, int nrow, int ncolumn){
	Node* head = initNode();
	//列ごとのヘッダノードを作成、LRポインタで繋げる
	Node* tmp = head;

	//後で使用。新たに作るノードに対し、上にあるノードを見つける
	Node** tmpnodes_col = new Node*[ncolumn]; //列の一番上にあるノード
	Node** tmpnodes = new Node*[ncolumn];
	
	for(int i = 0; i < ncolumn; i++){
		Node* hc = initNode();
		hc->label = i;
		hc->name += (char)(i + 0x41);
		connectLR(tmp, hc);
		tmp = hc;
		tmpnodes_col[i] = hc;
		tmpnodes[i] = hc;

	}
	connectLR(tmp, head);

	//column objectのサイズ計算用
	unsigned int* size_col = new unsigned int[ncolumn];
	for(int i = 0; i < ncolumn; i++)size_col[i] = 0;

	//1列ずつノードを作っていく
	for(int i = 0; i < nrow; i++){
		Node* tmp_row = nullptr; // 行の一番左にあるノードを保持
		tmp = nullptr;

		//i行目のノード構築
		for(int j = 0; j < ncolumn; j++){
			if(instance[i][j] == 1){
				Node* node = initNode();
				node->C = tmpnodes_col[j];

				//nullptr は行の最初のノードの構築時のみ
				if(tmp != nullptr){
					connectLR(tmp, node);
				}

				if(tmp_row == nullptr){
					tmp_row = node;
				}

				connectUD(tmpnodes[j] ,node);
				tmp = node;
				tmpnodes[j] = node;

				size_col[j]++;

				stringstream ss;
				ss << i;
				node->label = i;
				node->name = ss.str();
			}

		}

		//行に一つもノードが追加されなかった場合にifを満たす
		if(tmp != nullptr)
			connectLR(tmp, tmp_row);
	}

	//列の一番下のノードと列のヘッダーノードを接続する
	for(int i = 0; i < ncolumn; i++){
		//列の一番上のノードと最後に作業したノードが一致。空列が無い限りはない。
		if(tmpnodes[i] != tmpnodes_col[i]){
			connectUD(tmpnodes[i], tmpnodes_col[i]);
		}
	}

	//列のヘッダーノードのsizeを更新
	for(int i = 0; i < ncolumn; i++){
		tmpnodes_col[i]->size = size_col[i];
	}

	delete[] tmpnodes_col;
	delete[] tmpnodes;
	delete[] size_col;

	return head;
}

//グループ情報をくっつけたDancing Linkを作成。
void constructDLForGrouping(Node*& head, multimap<int, Node*>& groupList, int** instance, int* rowGroup, int nrow, int ncolumn){
	head = initNode();

	//列ごとのヘッダノードを作成、LRポインタで繋げる
	Node* tmp = head;

	//後で使用。新たに作るノードに対し、上にあるノードを見つける
	Node** tmpnodes_col = new Node*[ncolumn]; //列の一番上にあるノード
	Node** tmpnodes = new Node*[ncolumn];
	
	for(int i = 0; i < ncolumn; i++){
		Node* hc = initNode();
		hc->label = i;
		hc->name += (char)(i + 0x41);
		connectLR(tmp, hc);
		tmp = hc;
		tmpnodes_col[i] = hc;
		tmpnodes[i] = hc;

	}
	connectLR(tmp, head);

	//column objectのサイズ計算用
	unsigned int* size_col = new unsigned int[ncolumn];
	for(int i = 0; i < ncolumn; i++)size_col[i] = 0;

	//1行ずつノードを作っていく
	for(int i = 0; i < nrow; i++){
		Node* tmp_row = nullptr; // 行の一番左にあるノードを保持
		tmp = nullptr;

		//i行目のノード構築
		for(int j = 0; j < ncolumn; j++){
			if(instance[i][j] == 1){
				Node* node = initNode();
				node->C = tmpnodes_col[j];

				//nullptr は行の最初のノードの構築時のみ
				if(tmp != nullptr){
					connectLR(tmp, node);
				}

				if(tmp_row == nullptr){
					tmp_row = node;
				}

				connectUD(tmpnodes[j] ,node);
				tmp = node;
				tmpnodes[j] = node;

				size_col[j]++;

				stringstream ss;
				ss << i;
				node->label = i;
				node->name = ss.str();
				node->group = rowGroup[i];
			}

		}

		//行に一つもノードが追加されなかった場合にifを満たす
		//空行があるインスタンスは考えないでよし。

		if(tmp != nullptr){
			//rowGroupの情報を参照して、各グループに所属している行の左端のノードを連想配列に格納する
			groupList.insert(pair<int, Node*>(rowGroup[i], tmp_row));
			connectLR(tmp, tmp_row);


		}
	}

	//列の一番下のノードと列のヘッダーノードを接続する
	for(int i = 0; i < ncolumn; i++){
		//列の一番上のノードと最後に作業したノードが一致。空列が無い限りはない。
		if(tmpnodes[i] != tmpnodes_col[i]){
			connectUD(tmpnodes[i], tmpnodes_col[i]);
		}
	}

	//列のヘッダーノードのsizeを更新
	for(int i = 0; i < ncolumn; i++){
		tmpnodes_col[i]->size = size_col[i];
	}

	delete[] tmpnodes_col;
	delete[] tmpnodes;
	delete[] size_col;

	return;
}

//dlのc行目(cはNodeへのポインタ)に対してカバー操作
int cover(Node* dl, Node* c){
	c->R->L = c->L;
	c->L->R = c->R;

	for(Node* tmp = c->D; tmp != c; tmp = tmp->D){
		for(Node* tmp2 = tmp->R; tmp2 != tmp; tmp2 = tmp2->R ){
			//c行目の成分1の列を取り除く
			tmp2->D->U = tmp2->U;
			tmp2->U->D = tmp2->D;

			//cout << "cutted " << tmp2->name << endl;

			tmp2->C->size -= 1;
		}
	}

}

int uncover(Node* dl, Node* c){
	for(Node *tmp = c->U; tmp != c; tmp = tmp->U){
		for(Node * tmp2 = tmp->L; tmp2 != tmp; tmp2 = tmp2->L){
			tmp2->C->size += 1;
			tmp2->D->U = tmp2; tmp2->U->D = tmp2;

		}
	}
	c->R->L = c;
	c->L->R = c;

}


unsigned long long saiki = 0;
unsigned long long kai = 0;

void search(Node* dl, vector<int> &O ,unsigned int k){
	saiki++;
	if(dl->R == dl){
		kai++;
		return;
	}

	//choose C
	//とりあえずカバーされてないものから
	Node* c = dl->R;
	cover(dl, c); //クヌースの論文によると必要

	for(Node* tmp = c->D; tmp != c; tmp = tmp->D){
		O.push_back(atoi(tmp->name.c_str()));

		for(Node* tmp2 = tmp->R; tmp2 != tmp; tmp2 = tmp2->R){
			cover(dl, tmp2->C);
		}

		search(dl, O, k+1);

		O.pop_back();
		for(Node* tmp2 = tmp->L; tmp2 != tmp; tmp2 = tmp2->L){
			uncover(dl, tmp2->C);
		}
	}
	uncover(dl, c);
	return;
}


//アルゴリズムDLX
int algorithmDLX(Node* dl){
	vector<int> O;
	O.reserve(256);
	search(dl, O, 0);

	cout << "The number of solutions: " << kai << endl;

	return 0;
}

ZddNode* searchWithZDD(Node* dl, vector<int> &O ,unsigned int k, vector<bool> &key){
	saiki++;
	if(dl->R == dl){
		return ZddForMemo::nodes.at(1); //1終端を返す
	}

	auto it = ZddForMemo::columnMap.find(key);
	if( it != ZddForMemo::columnMap.end() )
		return ZddForMemo::nodes.at(it->second);


	Node* c = dl->R;

	key[c->label] = false;
	cover(dl, c);

	ZddNode* x = ZddForMemo::nodes.at(0);
	for(Node* tmp = c->D; tmp != c; tmp = tmp->D){
		O.push_back(atoi(tmp->name.c_str()));

		for(Node* tmp2 = tmp->R; tmp2 != tmp; tmp2 = tmp2->R){

			key[tmp2->C->label] = false;
			cover(dl, tmp2->C);
		}

		ZddNode* y = searchWithZDD(dl, O, k+1, key);
		if( y != ZddForMemo::nodes.at(0))
			x = ZddForMemo::nodes.at( ZddForMemo::Unique(tmp->label, x->GetID(), y->GetID()) );

		O.pop_back();
		for(Node* tmp2 = tmp->L; tmp2 != tmp; tmp2 = tmp2->L){

			key[tmp2->C->label] = true;
			uncover(dl, tmp2->C);
		}
	}
	uncover(dl, c);
	key[c->label] = true;

	ZddForMemo::columnMap[key] = x->GetID();
	return x;
}


//アルゴリズムDXZ
ZddNode* algorithmDXZ(Node* dl, int& csize){
	ZddForMemo::Init();
	vector<int> O;
	vector<bool> key(csize);
	fill(key.begin(), key.end(), true);


	O.reserve(256);
	ZddNode* zdd = searchWithZDD(dl, O, 0, key);

	//DumpColumnMap();

	return zdd;
}
void searchG(Node* dl, vector<int> &O , vector<int> &vG ,unsigned int k, const int& numG){

	saiki++;
	if(dl->R == dl){
		
		cout << "Found " ;
		//空なので(選択済みでないグループがない)解を出力
		for(auto x : O){
			cout << x << " ";
		}
		cout << endl;
		kai++;
		return;
	} else if( vG.size() == numG ){
		return;
	}

	//choose C
	//とりあえずカバーされてないものから
	Node* c = dl->R;
	cover(dl, c); //クヌースの論文によると必要

	for(Node* tmp = c->D; tmp != c; tmp = tmp->D){
		bool flag = false;

		//vGに含まれていたら飛ばす
		for(auto g : vG){
				if( tmp->group == g )
					flag = true;
		}
		//使えるものが無かったら終わり
		if(flag)continue;

		O.push_back(atoi(tmp->name.c_str()));

		for(Node* tmp2 = tmp->R; tmp2 != tmp; tmp2 = tmp2->R){
			cover(dl, tmp2->C);
		}

		vG.push_back(tmp->group);
		searchG(dl, O, vG, k+1, numG);
		vG.pop_back();

		O.pop_back();
		for(Node* tmp2 = tmp->L; tmp2 != tmp; tmp2 = tmp2->L){
			uncover(dl, tmp2->C);
		}
	}
	uncover(dl, c);
	return;
}


//グルーピングアルゴリズムX
int algorithmXG(Node* dl, multimap<int, Node*>& gList, int& numG){
	vector<int> O;
	vector<int> vG;

	O.reserve(256);
	searchG(dl, O, vG, 0, numG);

	return 0;
}

ZddNode* searchWithZDDG(Node* dl, vector<int> &O , vector<int> &vG ,unsigned int k, vector<bool> &key, const int& numG, const int& csize){
	//再帰の表示
	/*
	printf("%05d:", saiki);
	for(auto &x : O){
		cout << "->" << x << " ";
	}
	cout << endl;
	*/
	saiki++;

	

	if(dl->R == dl){
		return ZddForMemo::nodes.at(1); //1終端を返す
	}// else if( vG.size() == numG + 1 ){ //選択した数が存在するグループ数と一致
	//	return ZddForMemo::nodes.at(0); //0終端を返す
	//}

	auto it = ZddForMemo::columnMap.find(key);
	if( it != ZddForMemo::columnMap.end() )
		return ZddForMemo::nodes.at(it->second);


	Node* c = dl->R;

	key[c->label] = false;
	cover(dl, c);

	ZddNode* x = ZddForMemo::nodes.at(0);

	for(Node* tmp = c->D; tmp != c; tmp = tmp->D){
		//今まで選択されたグループと同じか
		bool flag = false;
		for(auto g : vG){
			if(tmp->group == g)
				flag = true;
		}
		if( flag )continue;

		vG.push_back(tmp->group);
		O.push_back(atoi(tmp->name.c_str()));

		//同グループのキーをtrueに
		key[csize + tmp->group] = true;

		for(Node* tmp2 = tmp->R; tmp2 != tmp; tmp2 = tmp2->R){
			key[tmp2->C->label] = false;
			cover(dl, tmp2->C);
		}

		ZddNode* y = searchWithZDDG(dl, O, vG, k+1, key, numG, csize);
		if( y != ZddForMemo::nodes.at(0))
			x = ZddForMemo::nodes.at( ZddForMemo::Unique(tmp->label, x->GetID(), y->GetID()) );

		O.pop_back();
		vG.pop_back();

		for(Node* tmp2 = tmp->L; tmp2 != tmp; tmp2 = tmp2->L){

			key[tmp2->C->label] = true;
			uncover(dl, tmp2->C);
		}

		key[csize + tmp->group] = false;
	}
	uncover(dl, c);
	key[c->label] = true;

	ZddForMemo::columnMap[key] = x->GetID();
	return x;
}


//アルゴリズムDXZG
ZddNode* algorithmDXZG(Node* dl, int& csize, const int& numG){
	ZddForMemo::Init();
	vector<int> O;
	vector<bool> key(csize + numG);
	vector<int> vG;
	fill(key.begin(), key.end() - numG, true);


	O.reserve(256);
	ZddNode* zdd = searchWithZDDG(dl, O, vG, 0, key, numG, csize);

	DumpZdd(zdd);

	return zdd;
}
