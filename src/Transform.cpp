#include "Transform.h"

Transform::Transform()
{
	position = vec3(0.0);
	scale = vec3(1.0);
	alpha = beta = gamma = 0;
}

Transform::~Transform() {}


void Transform::update_model_matrix()
{
	model_matrix =
		glm::translate(position) * get_rotation_matrix_313() *glm::scale(scale);
}


mat4 Transform::get_model_matrix()
{
	update_model_matrix();
	return model_matrix;
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
