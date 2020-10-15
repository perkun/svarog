#include "Mesh.h"

IndexModel::IndexModel(string filename)
{
	load_obj(filename);
}


IndexModel::~IndexModel()
{
	if (vertex_pos != NULL)
		delete[] vertex_pos;

	if (texture_pos != NULL)
		delete[] texture_pos;

	if (normal_pos != NULL)
		delete[] normal_pos;

	if (indices != NULL)
		delete[] indices;
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

	float *positions = new float[num_v * 3];
	float *normals = new float[num_vn * 3];
	float *texture_coords = new float[num_vt * 2];

	unsigned int *position_ids = new unsigned int[num_f * 3];
	unsigned int *normal_ids = new   unsigned int[num_f * 3];
	unsigned int *texture_ids = new  unsigned int[num_f * 3];

	int positions_size = 0;
	int normals_size = 0;
	int texture_size = 0;

	int position_ids_size = 0;
	int normal_ids_size = 0;
	int texture_ids_size = 0;


	fseek(f, 0, SEEK_SET);  //na poczatek pliku

	// parse data
// 	float tmp1, tmp2, tmp3;
	while (fgets(line, 99, f) != NULL)
	{
		if (line[0] == 'v')
		{
			if (line[1] == ' ')
			{
				int num_matched = sscanf(line, "%*s %f %f %f",
					&positions[positions_size],
					&positions[positions_size + 1],
					&positions[positions_size + 2]);

				positions_size += 3;

				if (num_matched != 3)

				{
					cout << "problem reading vertex position" << endl;
					return;
				}

			}
			else if (line[1] == 'n')
			{
				int num_matched = sscanf(line, "%*s %f %f %f",
					&normals[normals_size],
					&normals[normals_size + 1],
					&normals[normals_size + 2]);

				normals_size += 3;

				if (num_matched != 3)

				{
					cout << "problem reading normal coords" << endl;
					return;
				}
			}
			else if (line[1] == 't')
			{
				int num_matched = sscanf(line, "%*s %f %f",
					&texture_coords[texture_size],
					&texture_coords[texture_size + 1]);

				texture_size += 2;

				if (num_matched != 2)

				{
					cout << "problem reading texture coords" << endl;
					return;
				}
			}
		}
		else if (line[0] == 'f')
		{
			int f1, f2, f3, n1, n2, n3, t1, t2, t3;

			if (sscanf(line, "%*s %d %d %d", &f1, &f2, &f3) == 3)
			{
				position_ids[position_ids_size] = f1;
				position_ids[position_ids_size + 1] = f2;
				position_ids[position_ids_size + 2] = f3;

				position_ids_size += 3;
			}
			else if (sscanf(line, "%*s %d/%d %d/%d %d/%d",
						    &f1, &t1, &f2, &t2, &f3, &t3) == 6)
			{
				position_ids[position_ids_size] = f1;
				position_ids[position_ids_size + 1] = f2;
				position_ids[position_ids_size + 2] = f3;

				position_ids_size += 3;

				texture_ids[texture_ids_size] = t1;
				texture_ids[texture_ids_size + 1] = t2;
				texture_ids[texture_ids_size + 2] = t3;

				texture_ids_size += 3;
			}
			else if (sscanf(line, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
						    &f1, &t1, &n1, &f2, &t2, &n2, &f3, &t3, &n3) == 9)
			{
				position_ids[position_ids_size] = f1;
				position_ids[position_ids_size + 1] = f2;
				position_ids[position_ids_size + 2] = f3;

				position_ids_size += 3;

				texture_ids[texture_ids_size] = t1;
				texture_ids[texture_ids_size + 1] = t2;
				texture_ids[texture_ids_size + 2] = t3;

				texture_ids_size += 3;

				normal_ids[normal_ids_size] = n1;
				normal_ids[normal_ids_size + 1] = n2;
				normal_ids[normal_ids_size + 2] = n3;

				normal_ids_size += 3;
			}
			else if (sscanf(line, "%*s %d//%d %d//%d %d//%d",
						    &f1, &n1, &f2, &n2, &f3, &n3) == 6)
			{
				position_ids[position_ids_size] = f1;
				position_ids[position_ids_size + 1] = f2;
				position_ids[position_ids_size + 2] = f3;

				position_ids_size += 3;

				normal_ids[normal_ids_size] = n1;
				normal_ids[normal_ids_size + 1] = n2;
				normal_ids[normal_ids_size + 2] = n3;

				normal_ids_size += 3;
			}
			else
				cout << "problem reading indices" << endl;

		}
	}

	fclose(f);

	num_vertices = num_v;
	num_indices = num_f;

	for (int i = 0; i < position_ids_size; i++)
		position_ids[i]--;
	for (int i = 0; i < texture_ids_size; i++)
		texture_ids[i]--;
	for (int i = 0; i < normal_ids_size; i++)
		normal_ids[i]--;

	// construct texture_coords and normals per position
	indices = new unsigned int[position_ids_size];
	for (int i = 0; i < position_ids_size; i++)
		indices[i] = position_ids[i];

	vertex_pos = new float[positions_size];
	for (int i = 0; i < positions_size; i++)
		vertex_pos[i] = positions[i];


	texture_pos = new float[positions_size];
	normal_pos  = new float[positions_size];



	for (int i = 0; i < position_ids_size; i++)
	{
		if (i < normal_ids_size)
		{
			normal_pos[position_ids[i] * 3]     = normals[normal_ids[i] * 3 ];
			normal_pos[position_ids[i] * 3 + 1] = normals[normal_ids[i] * 3 + 1];
			normal_pos[position_ids[i] * 3 + 2] = normals[normal_ids[i] * 3 + 2];
		}

		if (i < texture_ids_size)
		{
			texture_pos[position_ids[i] * 2] = texture_coords[texture_ids[i] * 2];
			texture_pos[position_ids[i] * 2 + 1] =
				texture_coords[texture_ids[i] * 2 + 1];
		}

	}


	if (normal_ids_size == 0)
	{ // calculate normals
		for(unsigned int i = 0; i < position_ids_size; i += 3)
		{
			int i0 = indices[i];
			int i1 = indices[i + 1];
			int i2 = indices[i + 2];

			vec3 v1 = vertex_pos[] - vertex_pos[i1];
			vec3 v2 = vertex_pos[] - vertex_pos[i2];

			vec3 normal = normalize(cross(v1, v2));

			normals[i0] += normal;
			normals[i1] += normal;
			normals[i2] += normal;
		}

	}

// 	cout << num_indices << endl;
// 	cout << num_vertices << endl;
// 	cout << p << endl;




// 	for (int i = 0; i < 12; i += 3)
// 		cout << "pos: " << positions[i] << "  " << positions[i+1] << "  " << positions[i+2] << endl;
//
// 	for (int i = 0; i < num_vt*2; i += 2)
// 		cout  << "tex: " << texture_coords[i] << "  " << texture_coords[i+1] << endl;
//
// 	for (int i = 0; i < num_vn*3; i += 3)
// 		cout  << "nor " << normals[i] << "  " << normals[i+1] << "  " << normals[i+2] << endl;
//
// 	for (int i = 0; i < 12; i += 3)
// 		cout  << "pid " << position_ids[i] << "  " << position_ids[i+1] << "  " << position_ids[i+2] << endl;
//
// 	for (int i = 0; i < num_f*3; i += 3)
// 		cout  << "tid " << texture_ids[i] << "  " << texture_ids[i+1] << "  " << texture_ids[i+2] << endl;
//
// 	for (int i = 0; i < num_f*3; i += 3)
// 		cout  << "nid " << normal_ids[i] << "  " << normal_ids[i+1] << "  " << normal_ids[i+2] << endl;
	if (positions != NULL)
		delete[] positions;
	if (normals != NULL)
		delete[] normals;
	if (texture_coords != NULL)
		delete[] texture_coords;
	if (position_ids != NULL)
		delete[] position_ids;
	if (normal_ids != NULL)
		delete[] normal_ids;
	if (texture_ids != NULL)
		delete[] texture_ids;

}


Mesh::Mesh(string filename)
{
	IndexModel indexed_model(filename);
	create_vao(&indexed_model);
}

Mesh::~Mesh()
{
//     glDeleteVertexArrays(1, &vertex_array_object);
//     glDeleteBuffers(NUM_BUFFERS, vertex_array_buffers);

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
	glBufferData(GL_ARRAY_BUFFER, indexed_model->num_vertices * 3 * sizeof(float),
				 &indexed_model->vertex_pos[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITIONS);  // 1st attribute buffer : vertices
	glVertexAttribPointer(POSITIONS, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // binds to current VAO


	// texture pos
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[TEXTURE_COORDS]);
	glBufferData(GL_ARRAY_BUFFER, indexed_model->num_vertices * 2 * sizeof(float),
				 &indexed_model->texture_pos[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEXTURE_COORDS);  // 1st attribute buffer : vertices
	glVertexAttribPointer(TEXTURE_COORDS, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // binds to current VAO
//
//
	// normal pos
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[NORMALS]);
	glBufferData(GL_ARRAY_BUFFER, indexed_model->num_vertices * 3 * sizeof(float),
				 &indexed_model->normal_pos[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMALS);  // 1st attribute buffer : vertices
	glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // binds to current VAO


	// indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_buffers[INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				 indexed_model->num_indices * 3 * sizeof(unsigned int),
				 &indexed_model->indices[0], GL_STATIC_DRAW);

	num_triangles = indexed_model->num_indices;
}




void Mesh::draw()
{
	// DRAWING
	glBindVertexArray(vertex_array_object);
	glDrawElements(GL_TRIANGLES, num_triangles * 3, GL_UNSIGNED_INT, NULL);
}

