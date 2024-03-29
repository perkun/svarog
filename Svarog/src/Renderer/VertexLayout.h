#ifndef VERTEXLAYOUT_H_
#define VERTEXLAYOUT_H_


using namespace std;

enum class VertexDataType
{
	NONE = 0, FLOAT, FLOAT2, FLOAT3, FLOAT4, INT, INT2, INT3
};


class VertexLayoutElement
{
public:
    VertexLayoutElement(VertexDataType vdt) : data_type(vdt)
    {
    }

	size_t offset;
	VertexDataType data_type;

	int get_num_components() const;
	int get_gl_type() const;
	size_t get_size() const;
};


class VertexLayout
{
public:
	VertexLayout() {}
    VertexLayout(vector<VertexLayoutElement> elems);
    ~VertexLayout();

	void calculate_stride_and_elem_offsets();

	vector<VertexLayoutElement> elements;
	unsigned int stride;

};

#endif /* RENDERER/VERTEXLAYOUT_H_ */
