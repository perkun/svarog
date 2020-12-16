#ifndef MATH_H_
#define MATH_H_

#include <glm/glm.hpp>

using namespace std;

namespace Math {

	bool decompose_transform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

}

#endif /* MATH_H_ */
