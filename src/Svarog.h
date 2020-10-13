#ifndef SVAROG_H_
#define SVAROG_H_

#include <iostream>
#include <stdio.h>
#include "MemmoryLedger.h"
#include "Mesh.h"
#include "Transform.h"
#include "SceneGraph.h"


using namespace std;

class Svarog
{
public:
	Svarog();
	~Svarog();
	Mesh* create_mesh();

	SceneGraph scene_graph;

private:
	MemmoryLedger<Mesh*> mesh_ledger;



};

#endif /* SVAROG_H_ */
