#include <iostream>
#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define PI 3.14159f


Scene::Scene()
{
	level = NULL;
}

Scene::~Scene()
{
	if(level != NULL)
		delete level;
}


void Scene::init(ShaderProgram &texProgram)
{
	level = Level::createLevel(glm::vec3(16, 4, 32), texProgram, "images/floor.png", "images/wall.png");
	player1 = new AssimpModel();
	player2 = new AssimpModel();
	//model->loadFromFile("models/Ptero1.obj", texProgram);

	player2->loadFromFile("models/T-Rex2-00.obj", texProgram);
	player1->loadFromFile("models/T-Rex-00.obj", texProgram);
	//player2->loadFromFile("models/Ptero2.obj", texProgram);
	//player1->loadFromFile("models/Ptero1.obj", texProgram);

	model = player1;
	projection = glm::perspective(45.f / 180.f * PI, float(CAMERA_WIDTH) / float(CAMERA_HEIGHT), 0.1f, 100.f);
	currentTime = 0.0f;
	moving = false;
	posx = 3;
	posz = 0;
	dir = 2;
	backgone = 0;
	anim1 = true;
	gooff = false;
	goon = false;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	if (!moving){
		if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)){
			incrx = -1;
			incrz = 0;
			dir = 0;
			--backgone;
			gooff = true;
			//game over if backgone < -3
			moving = true;
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_UP)){
			incrx = 1;
			incrz = 0;
			dir = 2;
			moving = true;
			if (backgone < 0) {
				++backgone;
			}
			else goon = true;
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) ){
			incrx = 0;
			incrz = 1;
			dir = 1;
			moving = true;
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)){
			incrx = 0;
			incrz = -1;
			dir = 3;
			moving = true;
		}
		if (moving){
			incr = 0;
		}
	}
	if (moving){
		incr++;
		if (incr >= 20){
			if (gooff && backgone == 0){
				gooff = false;
			}
			posx += incrx;
			posz += incrz;
			incrx = 0;
			incrz = 0;
			incr = 0;
			moving = false;
		}
		if (incr == 10){
			if (!anim1){
				model = player1;
				anim1 = true;
			}
			else{
				model = player2;
				anim1 = false;
			}
		}
	}
	if (goon && backgone == 0){
		level->update();
		goon = false;
	}
}

void Scene::render(ShaderProgram &texProgram, glm::mat4 viewMatrix)
{
	glm::mat4 modelMatrix;
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	//viewMatrix = glm::lookAt(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));


	// Render level
	modelMatrix = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", viewMatrix);
	level->render(texProgram,viewMatrix);

	// Render loaded model
	float scaleFactor = 1.f / model->getHeight();
	glm::vec3 centerModelBase = model->getCenter() - glm::vec3(0.f, -model->getHeight() / 2.f, 0.f);

	modelMatrix = glm::mat4(1.0f);
	
	//modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 0.5f * fabs(sinf(3.f * currentTime / 1000.f)), 0.f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(posx + incrx*incr*0.05f, sinf(incr*0.05f*PI) - 0.5f, 5.f + posz + incrz*incr*0.05f));
	modelMatrix = glm::rotate(modelMatrix,PI + dir * PI/2, glm::vec3(0,1,0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
	modelMatrix = glm::translate(modelMatrix, -centerModelBase);

	texProgram.setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
	model->render(texProgram);
}

glm::vec3 Scene::getPosPlayer(){
	if (gooff){
		return glm::vec3(posx + incrx - backgone, sinf(incr*0.05f*PI) - 0.5f, 5.f + posz + incrz*incr*0.05f);
	}
	return glm::vec3(posx + incrx*incr*0.05f , sinf(incr*0.05f*PI) - 0.5f, 5.f + posz + incrz*incr*0.05f);
	
}




