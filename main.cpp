#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void print_append(string, string);


class treeNode
{
	public:
		string chStr;
		int prob;
		treeNode* next;
		treeNode* left;
		treeNode* right;
		string code;
		// Constructors
		treeNode();
		treeNode(string, int);
};

treeNode :: treeNode()
{
	chStr = "dummy";
	prob = 0;
	next = NULL;
	left = NULL;
	right = NULL;
	code = "";
}

treeNode :: treeNode(string c, int p)
{
	chStr = c;
	prob = p;
	next = NULL;
	left = NULL;
	right = NULL;
	code = "";
}


class linkedList
{
	public:
		treeNode* listHead;
		// Constructor
		linkedList();
		treeNode* findSpot(treeNode *);
		void insertOneNode(treeNode *, treeNode *);
		string printList();
};

linkedList :: linkedList()
{
	listHead = new treeNode();
}

treeNode* linkedList :: findSpot(treeNode *newNode)
{
	treeNode* spot = this->listHead;
	while(spot->next != NULL)
	{
		if(spot->next != NULL and spot->next->prob < newNode->prob)
			spot = spot->next;
		else
			return spot;
	}
	return spot;
}

void linkedList :: insertOneNode(treeNode *spot, treeNode *newNode)
{
	newNode->next = spot->next;
	spot->next = newNode;
}

string linkedList :: printList()
{
	string line = "listHead --> ";
	treeNode* ptr = this->listHead;
	while(ptr->next != NULL)
	{
		line += "(\"";
		line += ptr->chStr;
		line += "\", ";
		line += std::to_string(ptr->prob);
		line += ", \"";
		line += ptr->next->chStr;
		line += "\") --> ";
		ptr = ptr->next;
	}
	line += "(\"";
	line += ptr->chStr;
	line += "\", ";
	line += std::to_string (ptr->prob);
	line += ", NULL) --> NULL\n";

	return line;
}

class HuffmanBinaryTree
{
	public:
		treeNode* Root;
		HuffmanBinaryTree();
		HuffmanBinaryTree(treeNode *);
		linkedList constructHuffmanLList(string, string);
		void constructHuffmanBinTree(linkedList *);

		void preOrderTraveral(treeNode *, string);
		void postOrderTraveral(treeNode *, string);
		void inOrderTraveral(treeNode *, string);
};

HuffmanBinaryTree :: HuffmanBinaryTree()
{
	Root = new treeNode();
}

HuffmanBinaryTree :: HuffmanBinaryTree(treeNode *listHead)
{
	Root = listHead;
}

linkedList HuffmanBinaryTree :: constructHuffmanLList(string input_file_name, string output_file_name)
{
	// Opening files
	ifstream inFile;
	ofstream outFile(output_file_name);
	inFile.open(input_file_name);
	// Initializing variables
	string word;
	string charIn;
	int prob;
	bool read_str = true; // True/False = reading characters/probabilities

	// Linked List
	linkedList L;

	while(inFile >> word)
	{
		if(read_str == true)
		{
			charIn = word;
			read_str = false;
		}
		else
		{
			prob = atoi(word.c_str());
			treeNode* temp = new treeNode(charIn, prob);
			L.insertOneNode(L.findSpot(temp), temp);
			read_str = true;
		}	
	}

	// Making the Root point to the linked list(Empty changed to point to somewhere)
	this->Root = L.listHead;

	outFile << L.printList();

	inFile.close();
	outFile.close();

	return L;
}

void HuffmanBinaryTree :: constructHuffmanBinTree(linkedList *L)
{
	string new_chStr;
	int new_prob = 0;

	while(L->listHead->next->next != NULL)
	{
		// Concatenating characters
		new_chStr = L->listHead->next->chStr;
		new_chStr += L->listHead->next->next->chStr;

		// Summing probabilities up
		new_prob = L->listHead->next->prob;
		new_prob += L->listHead->next->next->prob;

		// Creating new node
		treeNode* new_node = new treeNode(new_chStr, new_prob);
		// Pointing the new node's left and right child to its original nodes
		new_node->left = L->listHead->next;
		new_node->right = L->listHead->next->next;

		// Inserting new node
		L->insertOneNode(L->findSpot(new_node), new_node);

		// Shifting the linked list head to the next next one
		L->listHead->next = L->listHead->next->next->next;
	}

	// Renewing the Root
	this->Root = L->listHead->next;
}

void HuffmanBinaryTree :: preOrderTraveral(treeNode *Root, string output_file_name)
{
	if(Root == NULL)
		return;
	else
	{
		string line = "(\"" + Root->chStr + "\", " + to_string(Root->prob) + ")  "; 
		print_append(line, output_file_name);
		preOrderTraveral(Root->left, output_file_name);
		preOrderTraveral(Root->right, output_file_name);
	}
}

void HuffmanBinaryTree :: postOrderTraveral(treeNode *Root, string output_file_name)
{
	if(Root == NULL)
		return;
	else
	{
		postOrderTraveral(Root->left, output_file_name);
		postOrderTraveral(Root->right, output_file_name);
		string line = "(\"" + Root->chStr + "\", " + to_string(Root->prob) + ")  ";
		print_append(line, output_file_name);
	}
}

void HuffmanBinaryTree :: inOrderTraveral(treeNode *Root, string output_file_name)
{
	if(Root == NULL)
		return;
	else
	{
		inOrderTraveral(Root->left, output_file_name);
		string line = "(\"" + Root->chStr + "\", " + to_string(Root->prob) + ")  ";
		print_append(line, output_file_name);
		inOrderTraveral(Root->right, output_file_name);
	}
}


bool isLeaf(treeNode *node)
{
	if(node->left == NULL and node->right == NULL)
		return true;
	else
		return false;
}

void getCode(treeNode *T, string code, string output_file_name)
{
	if(isLeaf(T))
	{	
		T->code = code;
		// Writing code to output file
		string line = T->chStr + "   " + T->code + "\n";
		print_append(line, output_file_name);
	}
	else
	{
		// NEED TO DO BOTH LEFT AND RIGHT!!!!
		// If it has left child, go to its left child
		if(T->left != NULL)
		{	
			string newCode_left = code + "0";
			getCode(T->left, newCode_left, output_file_name);
		}
		// Otherwise it must have right child, go to its right child
		if(T->right != NULL)
		{
			string newCode_right = code + "1";
			getCode(T->right, newCode_right, output_file_name);
		}
	}
}

void print_append(string content, string file_name)
{
	ofstream outFile(file_name, fstream::app);
	outFile << content;
	outFile.close();
}


int main(int argc, char ** argv)
{
	HuffmanBinaryTree T;
	linkedList temp = T.constructHuffmanLList(argv[1], argv[6]);
	T.constructHuffmanBinTree(&temp);

	string fakeCode;
	// Cleaning up the coding list first before writing code
	ofstream codeList(argv[2], ifstream::trunc);
	codeList.close();
	getCode(T.Root, fakeCode, argv[2]);

	// Preorder traveral the tree
	// Cleaning up the list first before writing the order
	ofstream preOrder(argv[3], ifstream::trunc);
	preOrder.close();
	T.preOrderTraveral(T.Root, argv[3]);

	// PostOrder traveral the tree
	// Cleaning up
	ofstream postOrder(argv[4], ifstream::trunc);
	postOrder.close();
	T.postOrderTraveral(T.Root, argv[4]);

	// Inorder traveral the tree
	// Cleaning up
	ofstream inOrder(argv[5], ifstream::trunc);
	inOrder.close();
	T.inOrderTraveral(T.Root, argv[5]);

	return 0;
}
