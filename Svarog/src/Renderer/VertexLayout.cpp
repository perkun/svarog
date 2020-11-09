#include "VertexLayout.h"


int VertexLayoutElement::get_num_components()
{
	switch(data_type)
	{
		case VertexDataType::NONE: return 0;
		case VertexDataType::FLOAT: return 1;
		case VertexDataType::FLOAT2: return 2;
		case VertexDataType::FLOAT3: return 3;
		case VertexDataType::FLOAT4: return 4;
		case VertexDataType::INT: return 1;
		case VertexDataType::INT2: return 2;
		case VertexDataType::INT3: return 3;
	}
	return 0;
}


int VertexLayoutElement::get_gl_type()
{
	switch(data_type)
	{
		case VertexDataType::NONE: return GL_NONE;
		case VertexDataType::FLOAT: return GL_FLOAT;
		case VertexDataType::FLOAT2: return GL_FLOAT;
		case VertexDataType::FLOAT3: return GL_FLOAT;
		case VertexDataType::FLOAT4: return GL_FLOAT;
		case VertexDataType::INT: return GL_INT;
		case VertexDataType::INT2: return GL_INT;
		case VertexDataType::INT3: return GL_INT;
	}
	return 0;

}


size_t VertexLayoutElement::get_size()
{
	switch(data_type)
	{
		case VertexDataType::NONE: return 0;
		case VertexDataType::FLOAT: return 4;
		case VertexDataType::FLOAT2: return 4 * 2;
		case VertexDataType::FLOAT3: return 4 * 3;
		case VertexDataType::FLOAT4: return 4 * 4;
		case VertexDataType::INT: return 4;
		case VertexDataType::INT2: return 4 * 2;
		case VertexDataType::INT3: return 4 * 3;
	}
	return 0;
}

VertexLayout::VertexLayout(vector<VertexLayoutElement> elems)
{
	this->elements = elems;

	calculate_stride_and_elem_offsets();
}


VertexLayout::~VertexLayout()
{
}


void VertexLayout::calculate_stride_and_elem_offsets()
{
	stride = 0;
	size_t offset = 0;
	for (VertexLayoutElement &vle: elements)
	{
		vle.offset = offset;
		offset += vle.get_size();
		stride += vle.get_size();
	}

}
