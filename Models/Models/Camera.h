#ifndef _CAMERA_INCLUDE
#define _CAMERA_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"

class Camera
{
public:
	float pos_x, pos_z;
	float rot_y;
	int camera;

	Camera();
	~Camera();

	void update(bool *keys);

	glm::mat4 look(ShaderProgram &texProgram, glm::vec3 posPlayer);
};


#endif // _CAMERA_INCLUDE
