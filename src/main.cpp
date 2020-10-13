#include <stdio.h>
#include <iostream>
#include <vector>

#include "Svarog.h"


using namespace std;


void print_id(SceneNode* n)
{
	cout << n << endl;
}




////////////////////////////////////////////////////////////////////////////////
///////////////////////////////   MAIN   ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


int main()
{
	cout << "This is Svarog!" << endl;

	Svarog svarog;


// 	Mesh *mesh_1 = new Mesh;  // this needs to be hidden behind interface
// 							  // returned pointer as handle for future

	Mesh *mesh_1 = svarog.create_mesh();
	Transform trans;

	// tree neds sth to store visited/unvisited information
// 	map<SceneNode*, bool> visited;

	SceneNode *asteroid_0 = new SceneNode(trans, mesh_1);
	SceneNode *asteroid_1 = new SceneNode(trans, mesh_1);
	SceneNode *asteroid_2 = new SceneNode(trans, mesh_1);

	SceneNode *asteroid_1_0 = new SceneNode(trans, mesh_1);
	SceneNode *asteroid_1_1 = new SceneNode(trans, mesh_1);

	SceneNode *asteroid_2_0 = new SceneNode(trans, mesh_1);
	SceneNode *asteroid_2_1 = new SceneNode(trans, mesh_1);


	svarog.scene_graph.root->add_child(asteroid_0);
	svarog.scene_graph.root->add_child(asteroid_1);
	svarog.scene_graph.root->add_child(asteroid_2);

	asteroid_1->add_child(asteroid_1_0);
	asteroid_1->add_child(asteroid_1_1);

	asteroid_2->add_child(asteroid_2_0);
	asteroid_2->add_child(asteroid_2_1);


	cout << "adresy: \n"
		 << "root: " << svarog.scene_graph.root << endl
		 << "asteroid_0: " << asteroid_0 << endl
		 << "asteroid_1: " << asteroid_1 << endl
		 << "asteroid_2: " << asteroid_2 << endl
		 << "asteroid_1_0: " << asteroid_1_0 << endl
		 << "asteroid_1_1: " << asteroid_1_1 << endl
		 << "asteroid_2_0: " << asteroid_2_0 << endl
		 << "asteroid_2_1: " << asteroid_2_1 << endl
		 << endl;


	svarog.scene_graph.depth_first_search(svarog.scene_graph.root, print_id);

	cout << "detatching noede..." << endl;
// 	SceneNode* detatched = svarog.scene_graph.root->detatch_child(asteroid_2);
	asteroid_2->detatch();


	cout << "detatched:" << endl;
	svarog.scene_graph.depth_first_search(asteroid_2, print_id);

	cout << "DFS:" << endl;
	svarog.scene_graph.depth_first_search(svarog.scene_graph.root, print_id);


	return 0;
}
