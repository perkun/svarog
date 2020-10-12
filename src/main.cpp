#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;


class Transform
{
public:
	uint id;
};


class Mesh
{
public:
	uint id;
};


class Node
{
public:
	Node();
	Node(Transform transform, Mesh *mesh);
	~Node();
	void add_child(Node *c);

	virtual void update();
	virtual void draw();

private:
	Transform world_transform, transform;
	Mesh *mesh;

	Node *parent;
	vector<Node*> children;
};


Node::Node()
{
	parent = NULL;
};


Node::Node(Transform transform, Mesh *mesh = NULL)
{
	parent = NULL;
	this->transform = transform;
	this->mesh = mesh;
};


Node::~Node()
{
	for (Node *c: children)
		delete c;
}

void Node::add_child(Node *c)
{
	c->parent = this;
	children.push_back(c);
}

void Node::update()
{
	return;
}

void Node::draw()
{
	return;
}


template <typename T>
class MemmoryLedger
{
public:
	void add(T *p);
	void delete_all();

private:
	vector<T*> pointer_list;
};


template <typename T>
void MemmoryLedger<T>::add(T *p)
{
	pointer_list.push_back(p);
}


template <typename T>
void MemmoryLedger<T>::delete_all()
{
	for (T *p: pointer_list)
		delete p;
}

int main()
{
	cout << "This is Svarog!" << endl;

	// define ledgersk
	MemmoryLedger<Mesh> mesh_ledger;


	Mesh *mesh_1 = new Mesh;  // this needs to be hidden behind interface
							  // returned pointer as handle for future
	mesh_ledger.add(mesh_1);

	Transform trans;

	Node *root = new Node();

	Node *asteroid_0 = new Node(trans, mesh_1);
	Node *asteroid_1 = new Node(trans, mesh_1);
	Node *asteroid_2 = new Node(trans, mesh_1);

	Node *asteroid_1_0 = new Node(trans, mesh_1);
	Node *asteroid_1_1 = new Node(trans, mesh_1);

	Node *asteroid_2_0 = new Node(trans, mesh_1);
	Node *asteroid_2_1 = new Node(trans, mesh_1);


	root->add_child(asteroid_0);
	root->add_child(asteroid_1);
	root->add_child(asteroid_2);

	asteroid_1->add_child(asteroid_1_0);
	asteroid_1->add_child(asteroid_1_1);

	asteroid_2->add_child(asteroid_2_0);
	asteroid_2->add_child(asteroid_2_1);
// 	root->add_child(trans,



	delete root;
	mesh_ledger.delete_all();

	return 0;
}
