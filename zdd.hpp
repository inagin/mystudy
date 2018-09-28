//Todo:
//  ハッシュ関数の定義
//　Vectorメモリ確保の効率化
//　もしかしたらZddNodeにID持たせる必要は無いかもしれない


#include <unordered_set>
#include <unordered_map>
#include <vector>

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

private:
	int _id; //ノードのID。0-終端ノードは0、1-終端ノードは1、それ以外は2～
	static int _total_id;


public:

	static void Init(){
		ZeroTerminal = &_zero;
		OneTerminal = &_one;

		ZeroTerminal->_id = 0;
		OneTerminal->_id = 1;

	}

	static ZddNode* CreateZddNode(int _val, int _zero_child, int _one_child){
		//新たにノードを作成し、返却する
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

int ZddNode::_total_id = 2;
ZddNode* ZddNode::ZeroTerminal;
ZddNode* ZddNode::OneTerminal;
ZddNode ZddNode::_zero;
ZddNode ZddNode::_one;

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
		nodes.push_back(zdd); // IDと一致してくれるかな？
		
		//nodeMapに追加


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
	DumpZddR(node, O, text);

	cout << endl;
	cout << "The Number Of ZDD Nodes: " << ZddNode::GetNumOfZDDNode() << endl;
	cout << "The Number Of ZDD Sets : " << countOfSets << endl;
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