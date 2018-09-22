/* Todo:
	cの選び方を変更してみる
	高速化の為にstdに頼らない（ unordered_mapなど )
	→連想配列の自作。時間があったら。

	stringstreamなど重そうなのを極力使わない

	instancemakerの様子がおかしい。
	10 x 6 のインスタンスを作成しようとしたところ、バグが生じた……。
	原因を追及しないと。
*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <map>

#include "zdd.hpp"
#include "algorithm.hpp"

using namespace std;

//データを二次配列に変換
void dataToArr(ifstream& ifs, int**&arr, int& rsize, int& csize){
	ifs.clear(); //一回終端まで行ってしまうと、ステートが立って戻れなくなってしまうらしい
	ifs.seekg(0, ifs.beg);

	string str;
	getline(ifs,str);
	istringstream iss(str);

	char tmp[16];
	iss.getline(tmp,16,',');
	rsize = atoi(tmp);

	iss.getline(tmp,16,',');
	csize = atoi(tmp);

	//二次元配列の初期化
	arr = new int*[rsize];
	for(int i = 0; i < rsize; i++){
		arr[i] = new int[csize];
		getline(ifs, str);

		istringstream iss(str);		
		for(int j = 0; j < csize; j++){
			iss.getline(tmp, 16, ',');
			arr[i][j] = atoi(tmp);

		}
	}

	return;
}

//グループ化した場合のデータを二次配列に変換
void dataToArrForGrouping(ifstream& ifs, int**&arr, int*& rowGroup, int& rsize, int& csize, int& numG){
	ifs.clear(); //一回終端まで行ってしまうと、ステートが立って戻れなくなってしまうらしい
	ifs.seekg(0, ifs.beg);

	string str;
	getline(ifs,str);
	istringstream iss(str);

	char tmp[16];
	iss.getline(tmp,16,',');
	rsize = atoi(tmp);

	iss.getline(tmp,16,',');
	csize = atoi(tmp);

	iss.getline(tmp,16,',');
	numG = atoi(tmp);

	//二次元配列、グループの初期化
	arr = new int*[rsize];
	rowGroup = new int[rsize];

	for(int i = 0; i < rsize; i++){
		arr[i] = new int[csize];
		getline(ifs, str);

		istringstream iss(str);	

		iss.getline(tmp, 16, ',');
		rowGroup[i] = atoi(tmp);

		for(int j = 0; j < csize; j++){
			iss.getline(tmp, 16, ',');
			arr[i][j] = atoi(tmp);
		}
	}

	return;
}

int main(int argc, char* argv[]){

	if(argc == 1){
		ifstream ifs("instance\\instanceG.dat");
	} else if(argc == 2){
		ifstream ifs(argv[1]);
	}

	int** arr;
	int* rowg;

	int rsize;
	int csize;
	int numG;

	Node* link;
	multimap<int, Node*> gList;

	//dataToArr(ifs, arr, rsize, csize);
	//link = constructDL(arr, rsize, csize);
	dataToArrForGrouping(ifs, arr, rowg, rsize, csize, numG);
	constructDLForGrouping(link, gList, arr, rowg, rsize, csize);

	//行の名前を出力
	cout << "   ";
	for(Node* tmp = link->R; tmp != link; tmp = tmp->R){
		cout << tmp->name << " ";
	}
	cout << endl;

	for(int i = 0; i < rsize; i++){
		cout << i << " :";
		for(int j = 0; j < csize; j++){
			cout << arr[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;

	//アルゴリズム開始
	auto start = chrono::system_clock::now();

	//algorithmX(link);
	//ZddNode* zdd = algorithmDXZ(link, csize);
	ZddNode* zdd = algorithmDXZG(link, csize, numG);

	auto end = chrono::system_clock::now();
	auto dur = end - start;

	auto msec = chrono::duration_cast<chrono::milliseconds>(dur).count();

	DumpZdd(zdd);
	cout << "elappsed time :" << msec << " milli sec" << endl;
	cout << "count         :" << saiki << endl;

	//解放処理
	for(int i = 0; i < rsize; i++){
		delete[] arr[i];
		arr[i] = nullptr;
	}
	delete[] arr;
	delete[] rowg;
	arr = nullptr;

	return 0;
}