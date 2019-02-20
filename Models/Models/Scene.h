#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Level.h"
#include "AssimpModel.h"



#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(ShaderProgram &texProgram);
	void update(int deltaTime);
	void render(ShaderProgram &texProgram, glm::mat4 viewMatrix);
	glm::vec3 getPosPlayer();


private:
	void initShaders();

private:
	Level *level;
	AssimpModel *player1, *player2, *model;
	float currentTime;
	bool moving, anim1, goon, gooff;
	glm::mat4 projection;
	int posx, posz, dir;
	int incr, incrx, incrz, backgone;


};


#endif // _SCENE_INCLUDE

