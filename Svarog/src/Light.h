#ifndef LIGHT_H_
#define LIGHT_H_


using namespace std;
using namespace glm;

class Light
{
public:
    Light();
    Light(vec3 dir) : direction(dir) {}
	~Light();

	vec3 direction = vec3(0, 0, 1);

};

#endif /* LIGHT_H_ */
