#include "Transform.h"

Transform::Transform()
{
	position = vec3(0.0);
	scale = vec3(1.0);
	alpha = beta = gamma = 0;
}

Transform::~Transform() {}


void Transform::update_local()
{
	local =
	 	get_rotation_matrix_313() * glm::translate(position) * glm::scale(scale);
}


mat4 Transform::get_local_tansform()
{
	update_local();
	return local;
}


mat4 Transform::get_world_tansform()
{
	return world;
}

mat4 Transform::get_rotation_matrix_313()
{
	/**
	 * zwróci macierz obrotu o kąty Eulera 313 (ZXZ)
	 */
	return
		glm::rotate(alpha, glm::vec3(0.0, 0.0, 1.0)) *
		glm::rotate(beta, glm::vec3(1.0, 0.0, 0.0)) *
		glm::rotate(gamma, glm::vec3(0.0, 0.0, 1.0));
}
