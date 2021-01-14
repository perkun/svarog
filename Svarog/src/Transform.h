#ifndef TRANSFORM_H_
#define TRANSFORM_H_


using namespace std;
using namespace glm;

class Transform
{
public:
    Transform();
    ~Transform();

    void update_local();
    mat4 get_local_tansform();
    mat4 get_world_tansform();
    mat4 get_view();

    float speed = 0.6;            // world space units per second

    mat4 local;
    mat4 world;


    vec3 scale, position, rotation;

    // 	mat4 get_rotation_matrix_313();
private:
    mat4 get_rotation_matrix();
};


#endif
