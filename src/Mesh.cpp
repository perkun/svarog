#include "Mesh.h"

IndexModel::IndexModel(string filename)
{
	load_obj(filename);
}


IndexModel::~IndexModel()
{
}

void IndexModel::load_obj(string filename)
{
	FILE *f = fopen(filename.c_str(), "r");
	if (f == NULL)
	{
		perror("File not found");
		return;
	}

	int num_v = 0, num_f = 0, num_vn = 0, num_vt = 0;
	char line[100];

	// get number of elements to allocate memmory
	while (fgets(line, 99, f) != NULL)
	{
		if (line[0] == 'v')
		{
			if (line[1] == ' ')
				num_v++;
			else if (line[1] == 'n')
				num_vn++;
			else if (line[1] == 't')
				num_vt++;
		}
		else if (line[0] == 'f')
		{
			num_f++;
		}
	}

	cout << num_v << "\t" << num_vt << "\t" << num_vn << "\t" << num_f << endl;

	fseek(f, 0, SEEK_SET);  //na poczatek pliku

	ver.reserve(num_v);

	vector<vec3> tmp_tex;
	tmp_tex.reserve(num_vt);

	vector<vec3> tmp_nor;
	tmp_nor.reserve(num_vn);

	ver_idxs.reserve(num_f);

	vector<ivec3> tex_idxs;
	tex_idxs.reserve(num_f);

	vector<ivec3> nor_idxs;
	nor_idxs.reserve(num_f);


	vec2 tmpv2f;
	vec3 tmpvf;
	int f1, f2, f3, n1, n2, n3, t1, t2, t3;
	while (fgets(line, 99, f) != NULL)
	{
		if (line[0] == 'v')
		{
			if (line[1] == ' ')
			{

				int num_matched = sscanf(line, "%*s %f %f %f",
										 &tmpvf.x, &tmpvf.y, &tmpvf.z);

				ver.push_back(tmpvf);

				if (num_matched != 3)

				{
					cout << "problem reading vertex position" << endl;
					return;
				}

			}
			else if (line[1] == 'n')
			{
				int num_matched = sscanf(line, "%*s %f %f %f",
										 &tmpvf.x, &tmpvf.y, &tmpvf.z);

				tmp_nor.push_back(tmpvf);

				if (num_matched != 3)

				{
					cout << "problem reading normal coords" << endl;
					return;
				}
			}
			else if (line[1] == 't')
			{
				int num_matched = sscanf(line, "%*s %f %f",
										 &tmpv2f.x, &tmpv2f.y);

				if (num_matched != 2)

				{
					cout << "problem reading texture coords" << endl;
					return;
				}
			}
		}
		else if (line[0] == 'f')
		{
			if (sscanf(line, "%*s %d %d %d", &f1, &f2, &f3) == 3)
			{
				ver_idxs.push_back(vec3(f1-1, f2-1, f3-1));
			}
			else if (sscanf(line, "%*s %d/%d %d/%d %d/%d",
						    &f1, &t1, &f2, &t2, &f3, &t3) == 6)
			{
				ver_idxs.push_back(ivec3(f1-1, f2-1, f3-1));
				tex_idxs.push_back(ivec3(t1-1, t2-1, t3-1));

			}
			else if (sscanf(line, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
						    &f1, &t1, &n1, &f2, &t2, &n2, &f3, &t3, &n3) == 9)
			{
				ver_idxs.push_back(vec3(f1-1, f2-1, f3-1));
				tex_idxs.push_back(ivec3(t1-1, t2-1, t3-1));
				nor_idxs.push_back(ivec3(n1-1, n2-1, n3-1));

			}
			else if (sscanf(line, "%*s %d//%d %d//%d %d//%d",
						    &f1, &n1, &f2, &n2, &f3, &n3) == 6)
			{
				ver_idxs.push_back(vec3(f1-1, f2-1, f3-1));
				nor_idxs.push_back(ivec3(n1-1, n2-1, n3-1));
			}
			else
				cout << "problem reading indices" << endl;

		}
	}

	fclose(f);

	num_vertices = ver.size();
	num_indices = ver_idxs.size();


	nor.resize(ver.size(), vec3(0));
	tex.resize(ver.size()), vec2(0); // all zeros...

	for (int i = 0; i < ver_idxs.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i < tex_idxs.size())
				tex[ver_idxs[i][j]] = tmp_tex[tex_idxs[i][j]];
			if (i < nor_idxs.size())
				nor[ver_idxs[i][j]] = tmp_nor[nor_idxs[i][j]];
		}
	}


	// calculate normals if there were none
	if (nor_idxs.size() == 0)
	{
		for (int i = 0; i < ver_idxs.size(); i++)
		{
			int i0 = ver_idxs[i].x;
			int i1 = ver_idxs[i].y;
			int i2 = ver_idxs[i].z;

			vec3 v1 = ver[i0] - ver[i1];
			vec3 v2 = ver[i0] - ver[i2];

			vec3 normal = normalize(cross(v1, v2));

			nor[ver_idxs[i].x] += normal;
			nor[ver_idxs[i].y] += normal;
			nor[ver_idxs[i].z] += normal;
		}
	}

	for (int i = 0; i < nor.size(); i++)
	{
		nor[i] = normalize(nor[i]);
	}
}


Mesh::Mesh(string filename)
{
	IndexModel indexed_model(filename);
	create_vao(&indexed_model);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteBuffers(NUM_BUFFS, vertex_buffers);

}

void Mesh::create_vao(IndexModel *indexed_model)
{


	// VAO
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);

	// generate buffers
	glGenBuffers(NUM_BUFFS, vertex_buffers);

	// VBO for vertices
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[POSITIONS]);
	glBufferData(GL_ARRAY_BUFFER, indexed_model->ver.size() * 3 * sizeof(float),
				 &indexed_model->ver[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITIONS);  // 1st attribute buffer : vertices
	glVertexAttribPointer(POSITIONS, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // binds to current VAO


	// texture pos
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[TEXTURE_COORDS]);
	glBufferData(GL_ARRAY_BUFFER, indexed_model->tex.size() * 2 * sizeof(float),
				 &indexed_model->tex[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEXTURE_COORDS);  // 1st attribute buffer : vertices
	glVertexAttribPointer(TEXTURE_COORDS, 2, GL_FLOAT, GL_FALSE, 0, (void*)0); // binds to current VAO
//
//
	// normal pos
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[NORMALS]);
	glBufferData(GL_ARRAY_BUFFER, indexed_model->nor.size() * 3 * sizeof(float),
				 &indexed_model->nor[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMALS);  // 1st attribute buffer : vertices
	glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // binds to current VAO


	// indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffers[INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				 indexed_model->ver_idxs.size() * 3 * sizeof(unsigned int),
				 &indexed_model->ver_idxs[0], GL_STATIC_DRAW);

	num_triangles = indexed_model->num_indices;
}




void Mesh::draw()
{
	// DRAWING
	glBindVertexArray(vertex_array_object);
	glDrawElements(GL_TRIANGLES, num_triangles * 3, GL_UNSIGNED_INT, NULL);
}

