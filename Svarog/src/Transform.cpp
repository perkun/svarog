#include "svpch.h"
#include "Transform.h"

#define GLM_ENABLE_EXPERIMENTAL

Transform::Transform()
{
    position = vec3(0.0);
    rotation = vec3(0.0);
    scale = vec3(1.0);
    lambda = beta = gamma = 0.0;
}

Transform::~Transform()
{
}


void Transform::update_local()
{
    local =
        glm::translate(position) * get_rotation_matrix() * glm::scale(scale);
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


mat4 Transform::get_rotation_matrix()
{
    return toMat4(quat(rotation));
}

// mat4 Transform::get_rotation_matrix_313()
// {
//     /**
//      * zwróci macierz obrotu o kąty Eulera 313 (ZXZ)
//      */
//     return rotate(alpha, vec3(0.0, 0.0, 1.0)) *
//            rotate(beta, vec3(1.0, 0.0, 0.0)) *
//            rotate(gamma, vec3(0.0, 0.0, 1.0));
// }


void Transform::set_euler_lbg()
{
    quat ql, qb, qg;
    // rotate gamma about z axis
    qg.w = cos((gamma + M_PI_2) / 2);
    qg.x = 0;
    qg.y = 0;
    qg.z = sin((gamma + M_PI_2)/ 2);

    // rotate beta about y axis
    qb.w = cos((M_PI_2 - beta) / 2);
    qb.x = 0;
    qb.y = sin((M_PI_2 - beta) / 2);
    qb.z = 0;

    // rotate alpha about z axis
    ql.w = cos(lambda / 2);
    ql.x = 0;
    ql.y = 0;
    ql.z = sin(lambda / 2);

    rotation = glm::eulerAngles(ql * qb * qg);
}
