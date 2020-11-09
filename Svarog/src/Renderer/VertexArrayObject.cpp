#include "VertexArrayObject.h"


VertexArrayObject::VertexArrayObject(VertexLayout vl)
{
	vertices_layout = vl;

}


VertexArrayObject::~VertexArrayObject()
{
// 	if (created)
// 	{
// 		glDeleteVertexArrays(1, &vao_id);
// 		glDeleteBuffers(1, &vbo_id);
// 		glDeleteBuffers(1, &idxs_id);
// 	}
}

VertexArrayObject::VertexArrayObject(const VertexArrayObject &other)
{
	vao_id = other.vao_id;
	vbo_id = other.vbo_id;
	idxs_id = other.idxs_id;

	num_draw_elements = other.num_draw_elements;
	created = other.created;
	vertices_layout = other.vertices_layout;
}

void VertexArrayObject::create()
{
	vertices_layout.calculate_stride_and_elem_offsets();

    float vertices[] = {0.5,  -0.5, -0.5, 0.5,  0.5, -0.5, -0.5, 0.5,
                        -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5,  0.5,
                        0.5,  0.5,  -0.5, 0.5,  0.5, -0.5, -0.5, 0.5};

    int indices[] = {0, 1, 4, 1, 5, 4, 1, 2, 5, 2, 6, 5, 3, 1, 0, 3, 2, 1,
                     3, 4, 7, 3, 0, 4, 7, 2, 3, 6, 2, 7, 4, 5, 7, 7, 5, 6};

	num_draw_elements = 12 * 3;

    unsigned int buffer_index = 0;

    // create VAO
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    // generate buffers
    glGenBuffers(1, &vbo_id);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0],
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(buffer_index);
    // bind to current VAO
    for (VertexLayoutElement &vle : vertices_layout.elements)
    {
		cout << "attrob pointer" << endl;
        glVertexAttribPointer(buffer_index, vle.get_num_components(),
                              vle.get_gl_type(), GL_FALSE,
                              vertices_layout.stride, (void *)vle.offset);
    }

    // indices
    glGenBuffers(1, &idxs_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxs_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
                 &indices[0], GL_STATIC_DRAW);

	created = true;
}
