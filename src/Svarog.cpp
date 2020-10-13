#include "Svarog.h"

Svarog::Svarog() {}

Svarog::~Svarog()
{
	mesh_ledger.delete_all();

	delete scene_graph.root;
}

Mesh* Svarog::create_mesh()
{
	Mesh *m = new Mesh();
	mesh_ledger.add(m);
	return m;
}
