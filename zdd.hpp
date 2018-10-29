//Todo:
//  ハッシュ関数の定義
//　Vectorメモリ確保の効率化
//　もしかしたらZddNodeにID持たせる必要は無いかもしれない


#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>

using namespace std;

class ZddNode{
public:
	int val;
	int zero_child;
	int one_child;

	static ZddNode* ZeroTerminal;
	static ZddNode* OneTerminal;
	static ZddNode _zero;
	static ZddNode _one;

	static bool draw; //dot形式を出力するかどうか
	static int maxRow;
	//static vector<int> orderNode; //追加されたvalの順序を保存（描画用）

	virtual ~ZddNode(){};

protected:
	int _id; //ノードのID。0-終端ノードは0、1-終端ノードは1、それ以外は2～
	static int _total_id;


public:

	static void Init(){
		ZeroTerminal = &_zero;
		OneTerminal = &_one;

		ZeroTerminal->val = -1;
		OneTerminal->val = -1;

		ZeroTerminal->_id = 0;
		OneTerminal->_id = 1;

	}

	static void OnDraw(){
		draw = true;
		//orderNode.reserve(2048);
	}

	static ZddNode* CreateZddNode(int _val, int _zero_child, int _one_child){
		//新たにノードを作成し、返却する
		if(draw){
			if(_val > maxRow)maxRow = _val;
		}
		//if( find(orderNode.begin(), orderNode.end(), _val) == orderNode.end()){
		//	orderNode.push_back(_val);
		//} //下に配置するとなぜか落ちる

		ZddNode* node = new ZddNode();
		node->_id = _total_id++; //ノード番号を割り振る

		node->zero_child = _zero_child;
		node->one_child = _one_child;
		node->val = _val;


	}


	int GetID(){
		return _id;
	}

	static int GetNumOfZDDNode(){
		return _total_id;
	}

};

class ZddNodeAnalyze : public ZddNode{
public:
	unsigned int count_share; //共有に使われた回数
	unsigned int count_depth; //深さ
	unsigned int count_reduce; //節点共有規則が適用された回数 


	static ZddNodeAnalyze* CreateZddNode(int _val, int _zero_child, int _one_child, unsigned int depth){
		if(draw){
			if(_val > ZddNode::maxRow)ZddNode::maxRow = _val;
		}

		ZddNodeAnalyze* node = new ZddNodeAnalyze();
		node->_id = _total_id++; //ノード番号を割り振る

		node->zero_child = _zero_child;
		node->one_child = _one_child;
		node->val = _val;

		node->count_share = 0;
		node->count_depth = depth;
		node->count_reduce = 0;


	}

};

int ZddNode::_total_id = 2;
ZddNode* ZddNode::ZeroTerminal;
ZddNode* ZddNode::OneTerminal;
ZddNode ZddNode::_zero;
ZddNode ZddNode::_one;

bool ZddNode::draw = false;
int ZddNode::maxRow = 0;

struct ZddQuery{
	int val;
	int id_0;
	int id_1;

	ZddQuery(int _val, int _id_0, int _id_1):val(_val), id_0(_id_0), id_1(_id_1){

	}

	bool operator==(const ZddQuery& rhs) const{
		return this->val == rhs.val && this->id_0 == rhs.id_0 && this->id_1 == rhs.id_1;
	}

	bool operator!=(const ZddQuery& rhs) const{
		return this->val != rhs.val || this->id_0 != rhs.id_0 || this->id_1 != rhs.id_1;
	}
};

class HashZddQuery{
	public:
		size_t operator()(const ZddQuery &q) const{
			size_t t = q.val * 3 + q.id_0 * 11 + q.id_1 * 31;
			return t;
		}
};


class ZddForMemo{
public:
	static void Init(){
		ZddNode::Init();
		nodes.push_back( &ZddNode::_zero );
		nodes.push_back( &ZddNode::_one );
	}
	
	//ZDDNodeを探して、ノードのIDを返却。もし無かったら追加する（整数番号、0-枝側の接続先ノード、1-枝側の接続先ノード）
	static int Unique(int val, int id_0, int id_1){
		ZddQuery q = ZddQuery(val, id_0, id_1);
		auto search = nodeMap.find( q );
		if(search != nodeMap.end()){
			return search->second;
		}
		
		//ZDDノードの作成
		ZddNode* zdd = ZddNode::CreateZddNode(val, id_0, id_1);
		nodes.push_back(zdd);
		
		//nodeMapに追加
		nodeMap.insert(make_pair(q, zdd->GetID()));

		return zdd->GetID();
	}

	//重複を調べずにノードを追加
	static int Append(int val, int id_0, int id_1){
		ZddNode* zdd = ZddNode::CreateZddNode(val, id_0, id_1);
		nodes.push_back(zdd);

		return zdd->GetID();
	}

	//分析用ノードを追加
	static int Append(int val, int id_0, int id_1, unsigned int depth){
		ZddNodeAnalyze* zdd = ZddNodeAnalyze::CreateZddNode(val, id_0, id_1, depth);
		nodes.push_back(zdd);

		return zdd->GetID();
	}
	static vector<ZddNode*> nodes; //ZDDノードを入れておく
	static unordered_map<ZddQuery, int, HashZddQuery> nodeMap; //IDを値とするハッシュ
	static unordered_map<vector<bool>, int> columnMap; //選択された列をキー、IDを値とするハッシュ

};

vector<ZddNode*> ZddForMemo::nodes;
unordered_map<ZddQuery, int, HashZddQuery> ZddForMemo::nodeMap;
unordered_map<vector<bool>, int> ZddForMemo::columnMap;

int countOfSets = 0;

//ZDDの持つ集合を表示する。
void DumpZddR(ZddNode* node, vector<int> &O, bool text = true){
	if(node->GetID() == 1){
		if(text){
			for(int x : O)
				cout << x << " ";
			cout << endl;
		}
		countOfSets++;
		return;

	} else if(node->GetID() == 0){
		return;

	}

	//0-枝
	DumpZddR(ZddForMemo::nodes[ node->zero_child ], O, text);
	O.push_back(node->val);

	//1-枝
	DumpZddR(ZddForMemo::nodes[ node->one_child ], O, text);
	O.pop_back();
}

void DumpZdd(ZddNode* node, bool text = true){
	countOfSets = 0;
	vector<int> O;

	cout << "ZDD Paths To 1 Node: " << endl;
	DumpZddR(node, O, text);

	cout << endl;
	cout << "Number Of ZDD Nodes: " << ZddNode::GetNumOfZDDNode() << endl;
	cout << "Number Of ZDD Sets : " << countOfSets << endl;
}

//ZDDの構造を表示する
void DumpZddNode(ZddNode* node, int k = 0){
	for(int i = 0; i < k; i++){
		if(i == k-1){
			cout << "L-";
			break;
		}
		cout << "  ";
	}


	if(node->GetID() == 0){
		cout << "(False)" << endl;
		return;
	} else if(node->GetID() == 1){
		cout << "(True)" << endl;
		return;
	}

	cout << "Node: " << /* node->GetID() << */ "-val(" << node->val << ")" << endl;
	cout << "0-";
	DumpZddNode(ZddForMemo::nodes[ node->zero_child ], k+1);

	cout << "1-";
	DumpZddNode(ZddForMemo::nodes[ node->one_child ], k+1);
}

void DumpColumnMap(){
	cout << "ColumnMap: " << endl;

	for(const auto& x : ZddForMemo::columnMap){
		cout << "Key: ";
		for(const bool& b : x.first ){
			cout << (b)?"1":"0";
		}

		cout << " Id: " << x.second << "\n";
	}
	return;
}

//ordered: 一行に同じノードが並ぶようにする
void DumpDOT(string graphName, string fileName, string outputPNGFile = "", bool ordered = false, bool analyze = false){
	//DOTファイルを出力
	ofstream ofs(fileName, ofstream::out);

	ofs << "digraph \"" << graphName << "\" {" << endl;

	/*
	for(int i = ZddNode::maxRow; i >= 0; i--){
		ofs << " " << i << " [shape=none,label=\"" << i << "\"];" << endl;
	}

	for(int i = ZddNode::maxRow; i >= 1; i--){
		ofs << " " << i << " -> " << i-1 << " [style=invis];" << endl;
	}
	
	ofs << " " << "\"^\" [shape=none,label=\"root\"];" << endl;
	ofs << " " << "\"^\" -> \"" << ZddNode::maxRow << ":0\" [style=dashed];" << endl;
	*/
	//ノードのラベル名は"{番号}:{GetId()}"とする

	vector<int> sameList; //同じ値のノードの番号
	for(int i = ZddNode::maxRow; i >= 0; i--){
		int count = 0;

		if(ordered == true){
			sameList.clear();
		}

		if(!analyze){
			for(int j = 0; j < ZddForMemo::nodes.size(); j++){
				if(i == ZddForMemo::nodes[j]->val){
					//iと同じ番号のZDDノードを並べる
					ZddNode* nodej = ZddForMemo::nodes[j];
					ofs << " " << "\"" << i << ":" << j << "\" [label=\"" << i << "\"];" << endl;
					ofs << " " << "\"" << i << ":" << j << "\" -> \"" << ZddForMemo::nodes[nodej->zero_child]->val << ":" << nodej->zero_child << "\" [style=dashed, color=blue];" << endl; 
					ofs << " " << "\"" << i << ":" << j << "\" -> \"" << ZddForMemo::nodes[nodej->one_child]->val << ":" << nodej->one_child << "\" [style=solid, color=red];" << endl; 

					if(ordered == true){
						sameList.push_back(j);
					}
				}
			}
		} else {
			for(int j = 0; j < ZddForMemo::nodes.size(); j++){
				if(i == ZddForMemo::nodes[j]->val){
					//iと同じ番号のZDDノードを並べる
					ZddNode* node = ZddForMemo::nodes[j];
					ZddNodeAnalyze* nodej = dynamic_cast<ZddNodeAnalyze*>(node);
					if(nodej == nullptr){
						cout << "Something is wrong in DumpDOT." << endl;
						return;
					}
					ofs << " " << "\"" << i << ":" << j << "\" [label=\"" << i << ", d:" << nodej->count_depth << "\"];" << endl;
					ofs << " " << "\"" << i << ":" << j << "\" -> \"" << ZddForMemo::nodes[nodej->zero_child]->val << ":" << nodej->zero_child << "\" [style=dashed, color=blue];" << endl; 
					ofs << " " << "\"" << i << ":" << j << "\" -> \"" << ZddForMemo::nodes[nodej->one_child]->val << ":" << nodej->one_child << "\" [style=solid, color=red];" << endl; 

					if(ordered == true){
						sameList.push_back(j);
					}
				}
			}			
		}

		if(ordered == true){
			ofs << " {rank=same; ";
			for(int j : sameList){
				ofs << "\"" << i << ":" << j << "\"" << ";";
			}
			ofs << "}" << endl;
		}
	}

	ofs << " \"-1:0\" [shape=square, label=\"0\"];" << endl; 
	ofs << " \"-1:1\" [shape=square, label=\"1\"];" << endl; 

	ofs << "}";
	ofs.close();

	cout << "Created " << fileName << "." << endl;
	//PNGFileが指定されていればそれも出力
	if(outputPNGFile == ""){
		return;
	}

	stringstream command;
	command << "dot -Tpng " << fileName << " -o " << outputPNGFile << endl;
	system(command.str().c_str());

	cout << "Created " << outputPNGFile << "." << endl;
	return;
}