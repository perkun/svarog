#include "IndexedModel.h"


IndexedModelObj::IndexedModelObj(string filename, NormalIndexing mode)
{
	load(filename, mode);
}

void IndexedModelObj::load(string filename, NormalIndexing mode)
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

	fseek(f, 0, SEEK_SET);  //na poczatek pliku

	vector<vec3> tmp_pos;
	tmp_pos.reserve(num_v);

	vector<vec2> tmp_tex;
	tmp_tex.reserve(num_vt);

	vector<vec3> tmp_nor;
	tmp_nor.reserve(num_vn);

	pos_idxs.reserve(num_f);

	vector<uvec3> tex_idxs;
	tex_idxs.reserve(num_f);

	vector<uvec3> nor_idxs;
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

				tmp_pos.push_back(tmpvf);

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

				tmp_tex.push_back(tmpv2f);

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
				pos_idxs.push_back(vec3(f1-1, f2-1, f3-1));
			}
			else if (sscanf(line, "%*s %d/%d %d/%d %d/%d",
						    &f1, &t1, &f2, &t2, &f3, &t3) == 6)
			{
				pos_idxs.push_back(ivec3(f1-1, f2-1, f3-1));
				tex_idxs.push_back(ivec3(t1-1, t2-1, t3-1));

			}
			else if (sscanf(line, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
						    &f1, &t1, &n1, &f2, &t2, &n2, &f3, &t3, &n3) == 9)
			{
				pos_idxs.push_back(vec3(f1-1, f2-1, f3-1));
				tex_idxs.push_back(ivec3(t1-1, t2-1, t3-1));
				nor_idxs.push_back(ivec3(n1-1, n2-1, n3-1));

			}
			else if (sscanf(line, "%*s %d//%d %d//%d %d//%d",
						    &f1, &n1, &f2, &n2, &f3, &n3) == 6)
			{
				pos_idxs.push_back(vec3(f1-1, f2-1, f3-1));
				nor_idxs.push_back(ivec3(n1-1, n2-1, n3-1));
			}
			else
				cout << "problem reading indices" << endl;

		}
	}

	fclose(f);

// 	cout << tex_idxs.size() << endl;

	if (mode == NormalIndexing::PER_VERTEX)
		produce_arrays_per_vertex(tmp_pos, tmp_tex, tmp_nor,
								  pos_idxs, tex_idxs, nor_idxs);
	if (mode == NormalIndexing::PER_FACE)
		produce_arrays_per_face(tmp_pos, tmp_tex, tmp_nor,
								pos_idxs, tex_idxs, nor_idxs);

	prepare_vertices_and_indices();
}

void IndexedModelFile::produce_arrays_per_face(vector<vec3> &tmp_pos,
								   		 vector<vec2> &tmp_tex,
								   		 vector<vec3> &tmp_nor,
								   		 vector<uvec3> &pos_idxs,
								   		 vector<uvec3> &tex_idxs,
								   		 vector<uvec3> &nor_idxs)
{
	pos.reserve(pos_idxs.size() * 3);
	tex.reserve(pos_idxs.size() * 3);
	nor.reserve(pos_idxs.size() * 3);

	vector<uvec3> tmp_pos_idxs;
	tmp_pos_idxs.reserve(pos_idxs.size());

	unsigned int face_id = 0;
	for (int i = 0; i < pos_idxs.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i < pos_idxs.size())
				pos.push_back( tmp_pos[pos_idxs[i][j]] );

			if (i < tex_idxs.size())
				tex.push_back( tmp_tex[tex_idxs[i][j]] );

			if (i < nor_idxs.size())
				nor.push_back( tmp_nor[nor_idxs[i][j]] );
		}
		tmp_pos_idxs.push_back(uvec3(face_id, face_id+1, face_id+2));
		face_id += 3;
	}


	pos_idxs.clear();
	pos_idxs.resize(tmp_pos_idxs.size());
	for (int i = 0; i < tmp_pos_idxs.size(); i++)
	{
		pos_idxs[i] = tmp_pos_idxs[i];
	}

	// calculate normals if there were none
	if (nor_idxs.size() == 0)
		calculate_per_face_normals();


}


void IndexedModelFile::produce_arrays_per_vertex(vector<vec3> &tmp_pos,
								   				 vector<vec2> &tmp_tex,
								   				 vector<vec3> &tmp_nor,
								   				 vector<uvec3> &pos_idxs,
								   				 vector<uvec3> &tex_idxs,
								   				 vector<uvec3> &nor_idxs)
{
	pos.resize(tmp_pos.size(), vec3(0));
	tex.resize(tmp_pos.size(), vec2(0));
	nor.resize(tmp_pos.size(), vec3(0));

	for (int i = 0; i < pos_idxs.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i < pos_idxs.size())
				pos[pos_idxs[i][j]] = tmp_pos[pos_idxs[i][j]];

			if (i < tex_idxs.size())
				tex[pos_idxs[i][j]] = tmp_tex[tex_idxs[i][j]];

			if (i < nor_idxs.size())
				nor[pos_idxs[i][j]] = tmp_nor[nor_idxs[i][j]];
		}
	}


	// calculate normals if there were none
	if (nor_idxs.size() == 0)
		calculate_per_vertex_normals();


}


void IndexedModelFile::calculate_per_vertex_normals()
{
	for (int i = 0; i < pos_idxs.size(); i++)
	{
		int i0 = pos_idxs[i].x;
		int i1 = pos_idxs[i].y;
		int i2 = pos_idxs[i].z;

		vec3 v1 = pos[i0] - pos[i1];
		vec3 v2 = pos[i0] - pos[i2];

		vec3 normal = normalize(cross(v1, v2));

		nor[pos_idxs[i].x] += normal;
		nor[pos_idxs[i].y] += normal;
		nor[pos_idxs[i].z] += normal;
	}

	for (int i = 0; i < nor.size(); i++)
		nor[i] = normalize(nor[i]);
}


void IndexedModelFile::calculate_per_face_normals()
{
	nor.clear();
	nor.resize(pos_idxs.size() * 3);

	for (int i = 0; i < pos_idxs.size(); i++)
	{
		int i0 = pos_idxs[i].x;
		int i1 = pos_idxs[i].y;
		int i2 = pos_idxs[i].z;

		vec3 v1 = pos[i0] - pos[i1];
		vec3 v2 = pos[i0] - pos[i2];

		vec3 normal = normalize(cross(v1, v2));

		nor[pos_idxs[i].x] = normal;
		nor[pos_idxs[i].y] = normal;
		nor[pos_idxs[i].z] = normal;
	}

// 	for (int i = 0; i < nor.size(); i++)
// 		nor[i] = normalize(nor[i]);
}

void IndexedModelFile::prepare_vertices_and_indices()
{
	cout << pos.size() << " " << nor.size() << " " << tex.size() << "\n";
	layout.elements.push_back(VertexDataType::FLOAT3);

	unsigned int vertices_size = pos.size() * 3;
	unsigned int stride = 3;

	if (nor.size() > 0)
	{
		layout.elements.push_back(VertexDataType::FLOAT3);
		vertices_size += nor.size() * 3;
		stride += 3;
	}
	if (tex.size() > 0)
	{
		layout.elements.push_back(VertexDataType::FLOAT2);
		vertices_size += tex.size() * 2;
		stride += 2;
	}

	vertices.resize(vertices_size);

	for (unsigned int i = 0; i < pos.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			vertices[i * stride + j] = pos[i][j];
		}

		unsigned int offset = 3;

		if (nor.size() > 0)
		{
			for (int j = 0; j < 3; j++)
			{
				vertices[i * stride + j + offset] = nor[i][j];
				cout << nor[i][j] << ", ";
			}
			offset += 3;
		}
//
		if (tex.size() > 0)
		{
			for (int j = 0; j < 2; j++)
			{
				vertices[i * stride + j + offset] = tex[i][j];
			}
			offset += 2;
		}

		cout  << "\n";
	}

	indices.resize(pos_idxs.size() * 3);
	for (unsigned int i = 0; i < pos_idxs.size(); i++)
	{
		for (int j = 0; j < 3; j++)
			indices[i * 3 + j] = pos_idxs[i][j];
	}
}
