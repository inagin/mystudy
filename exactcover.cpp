/* Todo:
	コマンドライン引数の処理
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
#include <unistd.h>

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

enum {
	ALG_DLX,
	ALG_DXZ,
	ALG_DLXG,
	ALG_DXZG
};

int algOpt = ALG_DXZG; //アルゴリズムの種類
string instanceName = "";
string dotName = "";
string pngName = "";

bool outDOT = false;
bool outPNG = false;
bool ordered = false;

int main(int argc, char* argv[]){
	int opt;
	

	opterr = 0;
	while((opt = getopt(argc, argv, "ia:f:d:p:o")) != -1){
		switch(opt) {
			case 'i':
				//sizeofなどの情報を出力
				cout << "sizeof(Node):" << sizeof(Node) << endl;
				cout << "sizeof(ZddNode):" << sizeof(ZddNode) << endl;
				return 0;

			case 'a':
				if(string(optarg) == "dlx"){
					algOpt = ALG_DLX;
				} else if(string(optarg) == "dxz"){
					algOpt = ALG_DXZ;
				} else if(string(optarg) == "dlxg"){
					algOpt = ALG_DLXG;
				} else if(string(optarg) == "dxzg"){
					algOpt = ALG_DXZG;
				} else {
					cout << "Algorithm is invalid." << endl;
					return -1;
				}

				break;
			case 'f':
				instanceName = optarg;
				break;
			case 'd':
				outDOT = true;
				dotName = optarg;
				break;
			case 'p':
				outPNG = true;
				pngName = optarg;
				break;
			case 'o':
				ordered = true;
				break;
			default:
				cout << "Invalid Option." << endl;
			return -1;
		}
	}

	//Validation Check
	if(instanceName == ""){
		cout << "Could I have your file name?" << endl;
		return -1;
	}

	if(outDOT == false && outPNG == true){
		outDOT = true;
		dotName = pngName;
	}

	if(outDOT == true && (algOpt == ALG_DLX || algOpt == ALG_DLXG)){
		cout << "This algorithm does not support output DOT file." << endl;
		return -1;
	}

	ifstream ifs(instanceName);

	int** arr;
	int* rowg;

	int rsize;
	int csize;
	int numG;

	Node* link;
	multimap<int, Node*> gList; //必要？

	//初期データの構築


	if(algOpt == ALG_DLX){
		cout << "Algorithm DLX" << endl;
		//Algorithm DLXを使用
		dataToArr(ifs, arr, rsize, csize);
		link = constructDL(arr, rsize, csize);

	} else if(algOpt == ALG_DXZ) {
		cout << "Algorithm DXZ" << endl;
		//Algorithm DXZを使用
		dataToArr(ifs, arr, rsize, csize);
		link = constructDL(arr, rsize, csize);

	} else if(algOpt == ALG_DXZG){
		cout << "Algorithm DXZwithG" << endl;
		//Algorithm DXZwithGを使用
		dataToArrForGrouping(ifs, arr, rowg, rsize, csize, numG);
		constructDLForGrouping(link, gList, arr, rowg, rsize, csize);
	}

	cout << endl;

	if(outDOT == true){
		ZddNode::OnDraw();
	}

	//列の番号を出力
	cout << "Instance " << instanceName << ":" << endl;
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

	if(algOpt == ALG_DLX){
		//Algorithm DLXを使用
		algorithmDLX(link);
	} else if(algOpt == ALG_DXZ) {
		//Algorithm DXZを使用
		dataToArr(ifs, arr, rsize, csize);
		link = constructDL(arr, rsize, csize);
		zdd = algorithmDXZ(link, csize);
	} else if(algOpt == ALG_DXZG){
		//Algorithm DXZwithGを使用
		dataToArrForGrouping(ifs, arr, rowg, rsize, csize, numG);
		constructDLForGrouping(link, gList, arr, rowg, rsize, csize);
		zdd = algorithmDXZG(link, csize, numG);
	}

	//計測終了
	auto end = chrono::system_clock::now();
	auto dur = end - start;

	auto msec = chrono::duration_cast<chrono::milliseconds>(dur).count();

	if(algOpt == ALG_DXZ) {
		DumpZdd(zdd);
	} else if(algOpt == ALG_DXZG){
		DumpZdd(zdd);
	}
	
	cout << "Elappsed Time :" << msec << " milli sec" << endl;
	cout << "Count         :" << saiki << endl;
	if(algOpt == ALG_DXZ || algOpt == ALG_DXZG )cout << "Cut Count     :" << cut << endl;
	cout << endl;

	if(outDOT == true){
		dotName = dotName + ".dot";
		pngName = pngName + ".png";
		DumpDOT("result", dotName, outPNG?pngName:"", ordered);
	}

	return 0;
}
