#include<iostream>
#include<string.h>
#include<sstream>
#include<vector>
#include<queue>
#include<fstream>
#include<filesystem>
namespace fs = std::filesystem; // if you faced any error here follow this seq(view->property page->c/c++->language->set c++ languge standers to std c++17)
std::string My_paht;
std::int32_t j = 0;
std::int32_t k = 0;
void adress1(std::vector<std::string>& adress) {
	std::string path2 = "E:\\textOs\\";// not used
	std::string path = My_paht;
	for (const auto& entry : fs::directory_iterator(path)) //iterate on the all addresses in that path
	{
		adress[j] = adress[j] + entry.path().filename().u8string();// adress[j]=path"adress[j]"+file name
		j++;                                                 //path().filename() returns file name only of type path
	}                                                        // u8string() to convert it to string
}

using namespace std;
class treeNode {
private:
	string word;                        //word
	vector<int>ids;                     // ids"indexs of the adress vector" of the files that this word was in
	treeNode* leftP;
	treeNode* rightP;
public:
	treeNode(string word1, int id, treeNode* lp = nullptr, treeNode* rp = nullptr)//contructor in case of reading all files
	{
		word = word1;
		ids.push_back(id);
		leftP = lp;
		rightP = rp;
	}
	treeNode(string word1, vector<int> id, treeNode* lp = nullptr, treeNode* rp = nullptr)//constructor in case of reading the file "inverted index"
	{
		word = word1;
		ids.resize(id.size());        // put all the ids in that node
		ids = id;
		leftP = lp;
		rightP = rp;
	}
	void setLeft(treeNode* L) { leftP = L; }
	void setRight(treeNode* R) { rightP = R; }
	string get_vale() { return word; }
	treeNode* get_left() { return leftP; }
	treeNode* get_right() { return rightP; }
	void get_ids(vector<int>& ids1) { ids1 = ids; }//returns the vector of ids of one word
	friend class tree;
};
class tree
{
private:
	treeNode* rootPtr;
public:
	tree(treeNode* rotPtr = nullptr) { rootPtr = rotPtr; }
	treeNode* getRoot() { return rootPtr; }
	bool isEmpty() { return rootPtr == nullptr; }
	void insert(string x, int fileid) { //used in case of building the tree by reading all files 
		if (isEmpty()) { rootPtr = new treeNode(x, fileid); return; }
		treeNode* p = rootPtr;
		while (p != NULL) {
			string a = p->get_vale();
			if (x == a) {
				p->ids.push_back(fileid);
				return;
			}
			else if (x < a) {
				if (p->get_left() == nullptr) {
					treeNode* newNodePtr = new treeNode(x, fileid);
					p->setLeft(newNodePtr); return;
				}
				else p = p->get_left();
			}
			else {
				if (p->get_right() == nullptr) {
					treeNode* newNodePtr = new treeNode(x, fileid);
					p->setRight(newNodePtr); return;
				}
				else p = p->get_right();
			}
		}
	}
	void insert2(string x, vector<int> fileid) {//used in case of building the tree by reading the inverted index file 
		if (isEmpty()) { rootPtr = new treeNode(x, fileid); return; }
		treeNode* p = rootPtr;
		while (p != NULL) {
			string a = p->get_vale();
			if (x == a) {
				p->ids.resize(fileid.size());
				p->ids = fileid;
				return;
			}
			else if (x < a) {
				if (p->get_left() == nullptr) {
					treeNode* newNodePtr = new treeNode(x, fileid);
					p->setLeft(newNodePtr); return;
				}
				else p = p->get_left();
			}
			else {
				if (p->get_right() == nullptr) {
					treeNode* newNodePtr = new treeNode(x, fileid);
					p->setRight(newNodePtr); return;
				}
				else p = p->get_right();
			}
		}
	}
	void delete_node(treeNode* k)
	{
		if (isEmpty()) { return; }
		if (k == nullptr) { return; }
		delete_node(k->get_left());
		delete_node(k->get_right());
		delete k;
		return;
	}
	void clear()
	{
		delete_node(rootPtr);
		rootPtr = nullptr;
	}
	bool search(string wanted_word, vector<string>adress, ofstream& s_out) {
		if (isEmpty()) { return false; }
		treeNode* p = rootPtr;
		while (p != nullptr) {
			string node_word = p->word;
			if (node_word == wanted_word) {
				for (int i = 0; i < p->ids.size(); i++) {
					if ((i > 0) && (p->ids[i] == p->ids[i - 1]))
						continue;
					s_out << adress[p->ids[i]];
					s_out << endl;
				}
				return true;
			}
			else if (wanted_word < node_word)
			{
				p = p->get_left();
			}
			else {
				p = p->get_right();
			}
		}
		return false;
	}
	void print_lvl(treeNode* root, ofstream& out) { //print lvl by lvl from the node root to the leaves
		if (root == nullptr)                      // used to builed inverted index file
			return;
		queue<treeNode*>que;
		que.push(root);        //push the first node
		while (!que.empty()) {
			int nodeCount_in1lvl = que.size(); //get no of nodes in the queue
			while (nodeCount_in1lvl > 0) {
				treeNode* temp = que.front();  //pop the first one
				k++;
				out << temp->word << " ";  //print the word & its all id
				for (int i = 0; i < temp->ids.size(); i++) {

					out << temp->ids[i] << ",";
				}
				out << '*';
				out << endl;
				que.pop();
				if (temp->leftP != nullptr) {  //if this node has left or right chiled push it in the queue "the next lvl"
					que.push(temp->leftP);     // before poping the next node
				}
				if (temp->rightP != nullptr) {
					que.push(temp->rightP);
				}
				nodeCount_in1lvl--;
			}
		}
	}
	void print_all(ofstream& out) {
		print_lvl(rootPtr, out);
	}
	~tree() {
		clear();
	}
};
void delay(int delay1) { //used for files syncornization between GUI & code 

	clock_t s_T = clock() + delay1;
	while (clock() < s_T);

}
bool check(char y) {//check whether the char is symbol or not -used to detect the sympols ?!.,"(){}[]  to eleminate them from the word
	int fst_chr = (int)y;

	if ((fst_chr >= 48) && (fst_chr < 58)) { //numiric digits
		return false;
	}
	else if ((fst_chr >= 65) && (fst_chr < 91)) { //capital letters
		return false;
	}
	else if ((fst_chr >= 97) && (fst_chr < 123)) { //small 
		return false;
	}
	else if ((fst_chr >= 193) && (fst_chr < 219)) {//arabic letters
		return false;
	}
	else if ((fst_chr >= 224) && (fst_chr < 243)) {//arabic letters2
		return false;
	}
	else
		return true;
}
void read_push(string line, int id, vector<tree>& myInv) { //separte that line into words  
	string piece;                                       // push each word in the tree
	int space;
	while (line.length() > 0) {                            // incase of that theline starts with a space 
		while (line[0] == ' ')
		{
			line = line.substr(1, line.length() - 1);
		}
		space = line.find(" ");
		piece = line.substr(0, space);
		if (space != -1) {                  // it is not end of the line
			line = line.substr(space + 1, line.length() - 1 - space);
		}
		else
			line = "";

		while ((piece[0] == '&') || (piece[0] == '"') || (piece[0] == '(') || (piece[0] == '{') || (piece[0] == '[')) {
			piece = piece.substr(1, piece.length() - 1); //remove all of these chars from the word
		}

		if (piece.length() > 0) {
			int fst_chr = (int)piece[0]; //check the ascii of the first char in the word
			if ((fst_chr >= 48) && (fst_chr < 58)) {
				myInv[26].insert(piece, id);//tree of digits
			}
			else if ((fst_chr >= 65) && (fst_chr < 91)) {//digits
				while (check(piece[piece.length() - 1]))
					piece = piece.substr(0, piece.length() - 1);// remove any sympol from the end
				myInv[fst_chr - 65].insert(piece, id);
			}
			else if ((fst_chr >= 97) && (fst_chr < 123)) {//capital
				while (check(piece[piece.length() - 1]))
					piece = piece.substr(0, piece.length() - 1);// remove any sympol from the end
				myInv[fst_chr - 97].insert(piece, id);
			}
			else if ((fst_chr >= 193) && (fst_chr < 219)) {//arabic
				while (check(piece[piece.length() - 1]))
					piece = piece.substr(0, piece.length() - 1);// remove any sympol from the end
				myInv[27].insert(piece, id);// arabic tree
			}
			else if ((fst_chr >= 224) && (fst_chr < 243)) {//arabic2
				while (check(piece[piece.length() - 1]))
					piece = piece.substr(0, piece.length() - 1);// remove any sympol from the end
				myInv[27].insert(piece, id);
			}
			else {
				myInv[28].insert(piece, id);//tree sympols 
			}
		}
	}
}
int main() {

	vector<tree> kal;
	kal.resize(29);
	vector<string> adress;
	string x;
	string inputpath;
	ifstream inputP("The Path.txt");// the path to file that contains the path passed from gui
	getline(inputP, inputpath);
	inputP.close();
	My_paht = inputpath;
	adress.resize(100002, inputpath); //vector of pathes

	vector<int>idss;
	int id;
	int space;
	cout << My_paht << "the path" << endl;
	adress1(adress);
	ofstream perm1;  //if the code writes 1 in that files then the GUI have the permission to read the search result from the search result file 
	ifstream comm;   // file that contains the orders from the gui 0:close the program 1:built the tree by rading all files 2:built the tree from the inverted index file
					 // 3:search 4: print the tree in the inverted index file
	ifstream perm;   // this file caled read if the gui writes 1 in it the it means that there is an order in command file  
	string order = "5";
	string peerm;
	perm.exceptions(ifstream::failbit | ifstream::badbit); //files reading syncronization 
	while (true) {

		try {
			perm.open("Read.txt");
			getline(perm, peerm);
			perm.close();
			if (peerm == "1") {
				perm1.open("Read.txt");//permission to read
				perm1 << "6" << endl;            //clearing the permission
				perm1.close();
				comm.open("The Word.txt");//command file 's path 
				getline(comm, order);
				comm.close();
			}
		}
		catch (ifstream::failure e) { //in case of the gui and code are accessing the same file which called read
			cout << "el try catch sha8ala" << endl;
			comm.open("The Word.txt");//command file 's path 
			getline(comm, order);
			comm.close();
			perm1.open("Read.txt");//permission to read
			perm1 << "6" << endl;
			perm1.close();
		}

		if (order == "1") { //build the tree by reading all files

			ifstream infile;
			for (int i = 0; i < j; i++) {
				infile.open(adress[i]);

				cout << adress[i] << endl;

				if (infile.is_open()) {
					//cout << "opened" << ",";
					while (getline(infile, x)) {
						read_push(x, i, kal);
					}
					//cout<< endl;
				}

				else
					cout << "failed" << endl;
				infile.close();
			}
			cout << "finished yasta" << endl;
			cout << k << endl; //dummy
			cout << adress.size();
			ofstream done("Done.txt");//premission to read search result;
			done << "1" << endl; //tell the gui that we finished building the tree
			done.close();
		}
		else if (order == "2") { //build the tree by reading inverted index file
			string line, seq, word, rest;
			int space, val, lastc;
			vector<int>idse;
			ifstream in("The Inverted Index.txt");//the path tothe file where the code read inverted index from
			if (in.is_open()) {
				while (getline(in, line)) {
					//cout << line << endl;
					lastc = line.length() - 1;
					if (line.length() > 0) {
						while (line[lastc] != '*')//in case of that one word has more than one line of adresses
						{
							cout << "595" << endl;
							getline(in, rest);
							line = line + rest;
							lastc = line.length() - 1;
						}
						if (line[lastc] == '*')
							line = line.substr(0, line.length() - 1);
					}
					if (line.length() == 0) {
						continue;
					}
					space = line.find(" "); // there is a space separating betwen the wordand its ids in the inverted index file 
					if (space != -1)
					{
						word = line.substr(0, space);
						line = line.substr(space + 1, line.length() - 1 - space);
						while (line.length() > 0) { //read all the ids in one line then push them in the vector vector 
							space = line.find(",");
							seq = line.substr(0, space);
							if (space != (line.length() - 1)) {
								line = line.substr(space + 1, line.length() - 1 - space);
							}
							else
								line = "";
							stringstream ad(seq);
							ad >> val;
							idse.push_back(val);
						}
						int fst_chr = (int)word[0];
						if ((fst_chr >= 48) && (fst_chr < 58)) {
							kal[26].insert2(word, idse);
						}
						else if ((fst_chr >= 65) && (fst_chr < 91)) {
							kal[fst_chr - 65].insert2(word, idse);
						}
						else if ((fst_chr >= 97) && (fst_chr < 123)) {
							kal[fst_chr - 97].insert2(word, idse);
						}
						else if ((fst_chr >= 193) && (fst_chr < 219)) {
							kal[27].insert2(word, idse);
						}
						else if ((fst_chr >= 224) && (fst_chr < 243)) {
							kal[27].insert2(word, idse);
						}
						else {
							kal[28].insert2(word, idse);
						}
						idse.clear();

					}
				}
			}
			else {
				cout << "cant open invertedindex file" << endl;
			}
			in.close();
			cout << "finished reading" << endl;
			ofstream done("Done.txt");//premission to read search result;
			done << "1" << endl;
			done.close();
		}
		else if (order[0] == '3') {//search for a word example 3rock the order is 3 and the word is rock
			ofstream s_out("The Result.txt");//search result 
			order = order.substr(1, order.length() - 1);//remove 3

			while ((order[0] == '&') || (order[0] == '"') || (order[0] == '(') || (order[0] == '{') || (order[0] == '[')) {
				order = order.substr(1, order.length() - 1); //remove all of these chars from the word
			}
			if (order.length() > 0) {
				int fst_chr = (int)order[0];
				if ((fst_chr >= 48) && (fst_chr < 58)) {
					while (check(order[order.length() - 1]))
						order = order.substr(0, order.length() - 1);
					if (kal[26].search(order, adress, s_out)) {
						cout << "found" << endl;
					}
					else {

						s_out << "not found" << endl;
						cout << "not found" << endl;
					}
				}
				else if ((fst_chr >= 65) && (fst_chr < 91)) {
					while (check(order[order.length() - 1]))
						order = order.substr(0, order.length() - 1);
					if (kal[fst_chr - 65].search(order, adress, s_out)) {
						cout << "found" << endl;
					}
					else
						s_out << "not found" << endl;
				}
				else if ((fst_chr >= 97) && (fst_chr < 123)) {
					while (check(order[order.length() - 1]))
						order = order.substr(0, order.length() - 1);
					if (kal[fst_chr - 97].search(order, adress, s_out)) {
						cout << "found" << endl;
					}
					else
						s_out << "not found" << endl;
				}
				else if ((fst_chr >= 193) && (fst_chr < 219)) {
					while (check(order[order.length() - 1]))
						order = order.substr(0, order.length() - 1);
					if (kal[27].search(order, adress, s_out)) {
						cout << "found" << endl;
					}
					else
						s_out << "not found" << endl;
				}
				else if ((fst_chr >= 224) && (fst_chr < 243)) {
					while (check(order[order.length() - 1]))
						order = order.substr(0, order.length() - 1);
					if (kal[27].search(order, adress, s_out)) {
						cout << "found" << endl;
					}
					else
						s_out << "not found" << endl;
				}
				else {
					while (check(order[order.length() - 1]))
						order = order.substr(0, order.length() - 1);
					if (kal[28].search(order, adress, s_out)) {
						cout << "found" << endl;
					}
					else
						s_out << "not found" << endl;

				}
			}
			s_out.close();
			ofstream done("Done.txt");//premission to read search result;
			done << "1" << endl;
			done.close();
		}
		else if (order == "4") {
			ofstream out("The Inverted Index.txt");//inverted index again put this time for saving it not reading
			for (int i = 0; i < 29; i++)
			{
				kal[i].print_all(out);
				out << endl;
			}
			out.close();
			cout << "finished printing" << endl;
			cout << k << endl;
			ofstream done("Done.txt");//premission to read search result;
			done << "1" << endl;
			done.close();
		}
		else if (order == "0")
		{
			break;
		}
		order = "5";
		delay(50);//the delay in ms
	}
	return 0;
}