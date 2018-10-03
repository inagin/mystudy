/* Todo:
	cの選び方を変更してみる
	高速化の為にstdに頼らない（ unordered_mapなど )
	→連想配列の自作。時間があったら。

	instancemakerの様子がおかしい。
	10 x 6 のインスタンスを作成しようとしたところ、バグが生じた……。
*/

#include <iostream>
#include <iomanip>
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

	if(numG == 0){
		return;
	}

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
	if(argc <= 1){
		cout << "Could I have your file name?" << endl;
		return 0;
	}

	ifstream ifs(argv[1]);

	string mode;
	if(argc <= 2){
		//デフォルトではdxzgを使う
		mode = "-dxzg";
	} else {
		mode = argv[2];
	}

	int** arr;
	int* rowg;

	int rsize;
	int csize;
	int numG;

	Node* link;
	multimap<int, Node*> gList; //必要？

	//初期データの構築


	if(mode == "-dlx"){
		cout << "Algorithm DLX" << endl;
		//Algorithm DLXを使用
		dataToArr(ifs, arr, rsize, csize);
		link = constructDL(arr, rsize, csize);
	} else if(mode == "-dxz") {
		cout << "Algorithm DXZ" << endl;
		//Algorithm DXZを使用
		dataToArr(ifs, arr, rsize, csize);
		link = constructDL(arr, rsize, csize);
	} else if(mode == "-dxzg"){
		cout << "Algorithm DXZwithG" << endl;
		//Algorithm DXZwithGを使用
		dataToArrForGrouping(ifs, arr, rowg, rsize, csize, numG);
		constructDLForGrouping(link, gList, arr, rowg, rsize, csize);
	} else {
		cout << "We don't know " << mode <<". We're sorry.";
		return 0;
	}

	
	ZddNode::OnDraw();

	//列の番号を出力
	cout << "      ";
	for(Node* tmp = link->R; tmp != link; tmp = tmp->R){
		cout << (char)(0x30 + tmp->label) << " ";
	}
	cout << endl;

	for(int i = 0; i < rsize; i++){
		cout << setw(4) << i << " :";
		for(int j = 0; j < csize; j++){
			cout << arr[i][j] << " ";
		}
		cout << endl;
	}

	cout << endl;


	ZddNode* zdd = nullptr;
	//計測開始
	auto start = chrono::system_clock::now();

	if(mode == "-dlx"){
		//Algorithm DLXを使用
		algorithmDLX(link);
	} else if(mode == "-dxz") {
		//Algorithm DXZを使用
		dataToArr(ifs, arr, rsize, csize);
		link = constructDL(arr, rsize, csize);
		zdd = algorithmDXZ(link, csize);
	} else if(mode == "-dxzg"){
		//Algorithm DXZwithGを使用
		dataToArrForGrouping(ifs, arr, rowg, rsize, csize, numG);
		constructDLForGrouping(link, gList, arr, rowg, rsize, csize);
		zdd = algorithmDXZG(link, csize, numG);
	}

	//計測終了
	auto end = chrono::system_clock::now();
	auto dur = end - start;

	auto msec = chrono::duration_cast<chrono::milliseconds>(dur).count();

	if(mode == "-dxz") {
		DumpZdd(zdd);
	} else if(mode == "-dxzg"){
		DumpZdd(zdd);
	}
	
	cout << "elappsed time :" << msec << " milli sec" << endl;
	cout << "count         :" << saiki << endl;
	if(mode != "-dlx" )cout << "cut count     :" << cut << endl;

	DumpDOT("result", "output.dot", "output.png");

	return 0;
}
