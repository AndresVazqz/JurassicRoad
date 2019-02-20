#include "Camera.h"
#include <iostream>
#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"

#define PI 3.14159f

enum CamPoses
{
	BASIC ,FPS, TOP, RIGHT, LEFT, BACK, NOTHING
};


glm::mat4 Camera::look(ShaderProgram &texProgram, glm::vec3 posPlayer){
	glm::mat4 viewMatrix;
	switch (camera)	{
	case BASIC:
		viewMatrix = glm::lookAt(glm::vec3(posPlayer.x - 3.f, 3.f, posPlayer.z+1),
			glm::vec3(posPlayer.x, 0.0f, posPlayer.z),
			glm::vec3(0.8f, 0.0f, -0.2f));
		break;
	case FPS:
		viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		break;
	case TOP:
		viewMatrix = glm::lookAt(glm::vec3(8.0f, 20.0f, 4.0f),
			glm::vec3(8.0f, 0.0f, 4.0f),
			glm::vec3(1.0f, 0.0f, 0.0f));
		break;
	case RIGHT:
		viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(5.0f, 5.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f));
		break;
	case LEFT:
		viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(5.0f, 5.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f));
		break;
	case BACK:
		viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(5.0f, 5.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f));
		break;
	case NOTHING:
		break;
	}
	if (camera == FPS){
		viewMatrix = glm::rotate(viewMatrix, -rot_y, glm::vec3(0, 1, 0));
		viewMatrix = glm::translate(viewMatrix, glm::vec3(pos_x, 0, pos_z));
	}
	//rotate(-rot_y, 0, 1, 0);
	//translate(pos_x, 0, pos_z);
	return viewMatrix;

	//texProgram.setUniformMatrix4f("modelview", viewMatrix);
}

void Camera::update(bool*keys){

	if (keys['1']) camera = FPS;
	if (keys['2']) camera = TOP;
	if (keys['3']) camera = RIGHT;
	if (keys['4']) camera = LEFT;
	if (keys['5']) camera = BACK;

	if (camera == FPS){
		if (keys['q']){
			rot_y += 0.05f*ROT_LENGTH;
		}
		else if (keys['e']){
			rot_y -= 0.05f*ROT_LENGTH;
		}
	}

	if (keys['s']){
		pos_x -= sin(rot_y)*STEP_LENGTH;
		pos_z -= cos(rot_y)*STEP_LENGTH;
	}
	else if (keys['w']){
		pos_x += sin(rot_y)*STEP_LENGTH;
		pos_z += cos(rot_y)*STEP_LENGTH;
	}
	if (keys['a']){
		pos_x += cos(rot_y)*STEP_LENGTH;
		pos_z -= sin(rot_y)*STEP_LENGTH;
	}
	else if (keys['d']){
		pos_x -= cos(rot_y)*STEP_LENGTH;
		pos_z += sin(rot_y)*STEP_LENGTH;
	}
}

Camera::Camera()
{
}


Camera::~Camera()
{
}
