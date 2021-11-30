// Red Black Tree implementation in C++
// Author: Algorithm Tutor
// Tutorial URL: https://algorithmtutor.com/Data-Structures/Tree/Red-Black-Trees/

#include <iostream>
#include <optional>
#include <vector>
#include <fstream>

using namespace std;

class Logger
{
public:
	Logger() = default;
	void inc()
	{
		counter++;
	}
	size_t cnt() const
	{
		return counter;
	}
private:
	size_t counter = 0;
};

// key structure that represents a node in the tree
struct Node {
	int key; // holds the key
	Node *parent; // pointer to the parent
	Node *left; // pointer to left child
	Node *right; // pointer to right child
	int color; // 1 -> Red, 0 -> Black
	std::string data;
};

typedef Node *NodePtr;

// class RBTree implements the operations in Red Black Tree
class RBTree {
private:
	NodePtr root;
	NodePtr TNULL;

	// initializes the nodes with appropirate values
	// all the pointers are set to point to the null pointer
	void initializeNULLNode(NodePtr node, NodePtr parent) {
		node->key = 0;
		node->parent = parent;
		node->left = nullptr;
		node->right = nullptr;
		node->color = 0;
	}

	NodePtr searchTreeHelper(NodePtr node, int key, Logger* logger = nullptr) {
		if (node == TNULL || key == node->key) {
			return node;
		}
		if (logger)
			logger->inc();
		if (key < node->key) {
			return searchTreeHelper(node->left, key, logger);
		} 
		return searchTreeHelper(node->right, key, logger);
	}

	// fix the rb tree modified by the delete operation
	void fixDelete(NodePtr x) {
		NodePtr s;
		while (x != root && x->color == 0) {
			if (x == x->parent->left) {
				s = x->parent->right;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					leftRotate(x->parent);
					s = x->parent->right;
				}

				if (s->left->color == 0 && s->right->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				} else {
					if (s->right->color == 0) {
						// case 3.3
						s->left->color = 0;
						s->color = 1;
						rightRotate(s);
						s = x->parent->right;
					} 

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->right->color = 0;
					leftRotate(x->parent);
					x = root;
				}
			} else {
				s = x->parent->left;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (s->right->color == 0 && s->left->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				} else {
					if (s->left->color == 0) {
						// case 3.3
						s->right->color = 0;
						s->color = 1;
						leftRotate(s);
						s = x->parent->left;
					} 

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->left->color = 0;
					rightRotate(x->parent);
					x = root;
				}
			} 
		}
		x->color = 0;
	}


	void rbTransplant(NodePtr u, NodePtr v){
		if (u->parent == nullptr) {
			root = v;
		} else if (u == u->parent->left){
			u->parent->left = v;
		} else {
			u->parent->right = v;
		}
		v->parent = u->parent;
	}

	void deleteNodeHelper(NodePtr node, int key) {
		// find the node containing key
		NodePtr z = TNULL;
		NodePtr x, y;
		while (node != TNULL){
			if (node->key == key) {
				z = node;
			}

			if (node->key <= key) {
				node = node->right;
			} else {
				node = node->left;
			}
		}

		if (z == TNULL) {
			cout<<"Couldn't find key in the tree"<<endl;
			return;
		} 

		y = z;
		int y_original_color = y->color;
		if (z->left == TNULL) {
			x = z->right;
			rbTransplant(z, z->right);
		} else if (z->right == TNULL) {
			x = z->left;
			rbTransplant(z, z->left);
		} else {
			y = minimum(z->right);
			y_original_color = y->color;
			x = y->right;
			if (y->parent == z) {
				x->parent = y;
			} else {
				rbTransplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}

			rbTransplant(z, y);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}
		delete z;
		if (y_original_color == 0){
			fixDelete(x);
		}
	}
	
	// fix the red-black tree
	void fixInsert(NodePtr k) {
		NodePtr u;
		while (k->parent->color == 1) {
			if (k->parent == k->parent->parent->right) {
				u = k->parent->parent->left; // uncle
				if (u->color == 1) {
					// case 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				}
				else {
					if (k == k->parent->left) {
						// case 3.2.2
						k = k->parent;
						rightRotate(k);
					}
					// case 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					leftRotate(k->parent->parent);
				}
			}
			else {
				u = k->parent->parent->right; // uncle

				if (u->color == 1) {
					// mirror case 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				}
				else {
					if (k == k->parent->right) {
						// mirror case 3.2.2
						k = k->parent;
						leftRotate(k);
					}
					// mirror case 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root) {
				break;
			}
		}
		root->color = 0;
	}
	
	// rotate left at node x
	void leftRotate(NodePtr x) {
		NodePtr y = x->right;
		x->right = y->left;
		if (y->left != TNULL) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		}
		else if (x == x->parent->left) {
			x->parent->left = y;
		}
		else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	// rotate right at node x
	void rightRotate(NodePtr x) {
		NodePtr y = x->left;
		x->left = y->right;
		if (y->right != TNULL) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		}
		else if (x == x->parent->right) {
			x->parent->right = y;
		}
		else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	// find the node with the minimum key
	NodePtr minimum(NodePtr node) {
		while (node->left != TNULL) {
			node = node->left;
		}
		return node;
	}

	// find the node with the maximum key
	NodePtr maximum(NodePtr node) {
		while (node->right != TNULL) {
			node = node->right;
		}
		return node;
	}

	void _outputTree(NodePtr node_ptr, ostream& out)
	{
		if (!node_ptr || node_ptr == TNULL)
			return;
		_outputTree(node_ptr->left, out);
		out << node_ptr->key << ' ' << node_ptr->data << '\n';
		_outputTree(node_ptr->right, out);
	}

public:
	RBTree() {
		TNULL = new Node;
		TNULL->color = 0;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		root = TNULL;
	}

	// search the tree for the key k
	// and return the corresponding node
	std::optional<std::string> searchTree(int k, Logger* logger = nullptr) {
		auto node_ptr = searchTreeHelper(this->root, k, logger);
		if (node_ptr == TNULL)
			return std::nullopt;
		return node_ptr->data;
	}

	// insert the key to the tree in its appropriate position
	// and fix the tree
	void insert(int key, std::string data = "") {
		// Ordinary Binary Search Insertion
		NodePtr node = new Node;
		node->parent = nullptr;
		node->key = key;
		node->left = TNULL;
		node->right = TNULL;
		node->color = 1; // new node must be red
		node->data = data;

		NodePtr y = nullptr;
		NodePtr x = this->root;

		while (x != TNULL) {
			y = x;
			if (node->key < x->key) {
				x = x->left;
			} else {
				x = x->right;
			}
		}

		// y is parent of x
		node->parent = y;
		if (y == nullptr) {
			root = node;
		} else if (node->key < y->key) {
			y->left = node;
		} else {
			y->right = node;
		}

		// if new node is a root node, simply return
		if (node->parent == nullptr){
			node->color = 0;
			return;
		}

		// if the grandparent is null, simply return
		if (node->parent->parent == nullptr) {
			return;
		}

		// Fix the tree
		fixInsert(node);
	}

	// delete the node from the tree
	void deleteNode(int data) {
		deleteNodeHelper(this->root, data);
	}

	void outputTree(ostream& out)
	{
		_outputTree(root, out);
	}
};

int main() {
	RBTree bst;

	while (true)
	{
		string command;
		cin >> command;
		if (command == "insert")
		{
			int key;
			string val;
			cin >> key >> val;
			bst.insert(key, val);
		}
		else if (command == "search")
		{
			int key;
			cin >> key;
			auto res = bst.searchTree(key);
			if (res)
			{
				cout << *res << endl;
			}
			else
			{
				cout << "Couldn't find key in the tree" << endl;
			}
		}
		else if (command == "delete")
		{
			int key;
			cin >> key;
			bst.deleteNode(key);
		}
		else if (command == "save")
		{
			string fname;
			cin >> fname;
			ofstream out(fname);
			bst.outputTree(out);
		}
		else if (command == "load")
		{
			string fname;
			cin >> fname;
			ifstream in(fname);
			while (!in.eof())
			{
				int key;
				string val;
				in >> key >> val;
				bst.insert(key, val);
			}
		}
	}

	/*vector<int> v;
	for (int i = 0; i < 10000; i++)
	{
		v.push_back(rand());
		bst.insert(v.back());
	}

	size_t cmp_cnt = 0;
	for (int i = 0; i < 20; i++)
	{
		Logger logger;
		bst.searchTree(v[rand() % v.size()], &logger);
		cmp_cnt += logger.cnt();
		cout << i + 1 << '\t' << logger.cnt() << '\n';
	}*/
	return 0;
}