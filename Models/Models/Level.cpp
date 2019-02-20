#include <iostream>
#include <vector>
#include "Level.h"
#include "Game.h"
#include <cmath>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <time.h>
#include <random> 

using namespace std;


Level *Level::createLevel(const glm::vec3 &levelSize, ShaderProgram &program, const string &floorFile, const string &wallFile)
{
	Level *level = new Level(levelSize, program, floorFile, wallFile);

	return level;
}


Level::Level(const glm::vec3 &levelSize, ShaderProgram &program, const string &floorFile, const string &wallFile)
{

	srand(time(NULL));
	size = levelSize;
	step = 0;
	models = vector<AssimpModel*>(6);
	models[0] = new AssimpModel();
	models[0]->loadFromFile("models/ground1.obj", program);
	models[1] = new AssimpModel();
	models[1]->loadFromFile("models/Cactus.obj", program);
	models[2] = new AssimpModel();
	models[2]->loadFromFile("models/water1.obj", program);
	models[3] = new AssimpModel();
	models[3]->loadFromFile("models/WaterLily1.obj", program);
	models[4] = new AssimpModel();
	models[4]->loadFromFile("models/road1.obj", program);
	models[5] = new AssimpModel();
	models[5]->loadFromFile("models/Ptero1.obj", program);


	if(!floor.loadFromFile(floorFile.c_str(), TEXTURE_PIXEL_FORMAT_RGB))
		cout << "Could not load floor texture!!!" << endl;
	if(!wall.loadFromFile(wallFile.c_str(), TEXTURE_PIXEL_FORMAT_RGB))
		cout << "Could not load wall texture!!!" << endl;
	prepareArrays(program);

	modelFloorMap = vector < vector<AssimpModel*>>(16,vector<AssimpModel*>(15));
	modelObstacleMap = vector <vector<AssimpModel*>>(16, vector<AssimpModel*>(15));
	obstacleMap = vector<vector <bool> >(16, vector <bool>(15));
	biomeMap = vector<int>(16);
	for (int i = 0; i < modelFloorMap.size(); ++i){
		biomeMap[i] = 1 + rand() % (3);
		for (int j = 0; j < modelFloorMap[0].size(); ++j){
			modelFloorMap[i][j] = models[(biomeMap[i] - 1) * 2];
			if (j == 3 || j == 12){
				obstacleMap[i][j] = true;
			}
			else {
				obstacleMap[i][j] = 0 == rand() % (8);
			}
			if (obstacleMap[i][j]){
				modelObstacleMap[i][j] = models[(biomeMap[i]-1)*2 +1 ];
			}

			
			//modelMap[i][j]->loadFromFile("models/Ptero1.obj", program);
		}
	}

}

Level::~Level()
{
}


void Level::update(){
	int biomeNew = 1 + rand() % (3);
	vector<AssimpModel*> modelObstacleNew = vector<AssimpModel*>(15);
	vector <bool> obstacleNew = vector <bool>(15);
	vector<AssimpModel*> floorNew = vector<AssimpModel*>(15);
	for (int j = 0; j < obstacleNew.size(); ++j){
		floorNew[j] = models[(biomeNew - 1) * 2];
		if (j == 3 || j == 12){
			obstacleNew[j] = true;
		}
		else {
			obstacleNew[j] = 0 == rand() % (8);
		}
		if (obstacleNew[j]){
			modelObstacleNew[j] = models[(biomeNew - 1) * 2 + 1];
		}
	}
	biomeMap.insert(biomeMap.end(), biomeNew);
	biomeMap.erase(biomeMap.begin());
	modelFloorMap.insert(modelFloorMap.end(), floorNew);
	modelFloorMap.erase(modelFloorMap.begin());
	modelObstacleMap.insert(modelObstacleMap.end(), modelObstacleNew);
	modelObstacleMap.erase(modelObstacleMap.begin());
	obstacleMap.insert(obstacleMap.end(), obstacleNew);
	obstacleMap.erase(obstacleMap.begin());
	//modelFloorMap
	//modelObstacleMap
	//obstacleMap
	//modelMap.insert
	++step;
}

void Level::render(ShaderProgram &program, glm::mat4 viewMatrix)
{
	//prepareArrays(program);
	/*
	glEnable(GL_TEXTURE_2D);

	floor.use();
	glBindVertexArray(vao[0]);
	glEnableVertexAttribArray(posLocation[0]); 
	glEnableVertexAttribArray(normLocation[0]);
	glEnableVertexAttribArray(texCoordLocation[0]);
	glDrawArrays(GL_QUADS, 0, 4);

	wall.use();
	glBindVertexArray(vao[1]);
	glEnableVertexAttribArray(posLocation[1]);
	glEnableVertexAttribArray(normLocation[1]);
	glEnableVertexAttribArray(texCoordLocation[1]);
	glDrawArrays(GL_QUADS, 0, 4 * 4);
	*/
	float scaleFactorFloor = 1.f / 24.f;
	for (int i = 0; i < modelFloorMap.size(); ++i){
		for (int j = 0; j < 15; ++j){
			glm::mat4 modelMatrix = glm::mat4(1.f);
			modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(float(i+step),-2.f,float(j-3)));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactorFloor, scaleFactorFloor, scaleFactorFloor));
			modelMatrix = glm::translate(modelMatrix, -modelFloorMap[i][j]->getCenter());
			/*
			if (false && i == 0 && j == 0){
				cout << "Position Model ";
				for (int i = 0; i <= 3; ++i){
					for (int j = 0; j <= 3; ++j){
						cout << modelMatrix[i][j] << " ";
					}
					cout << endl;
				}
				cout << endl;
			}
			*/
			program.setUniformMatrix4f("modelview",viewMatrix * modelMatrix);
			modelFloorMap[i][j]->render(program);
			if (obstacleMap[i][j]){
				float scaleFactorObstacle;
				glm::mat4 modelMatrix = glm::mat4(1.f);
				if (biomeMap[i] == 2){
					scaleFactorObstacle = 1.f / 40.f;

					modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(float(i+step), -1.5f, float(j-3)));
				}
				else {
					scaleFactorObstacle = 1.f / 24.f;
					modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(float(i+step), -0.8f, float(j-3)));
				}
				modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactorObstacle, scaleFactorObstacle, scaleFactorObstacle));
				modelMatrix = glm::translate(modelMatrix, -modelObstacleMap[i][j]->getCenter());
				program.setUniformMatrix4f("modelview", viewMatrix * modelMatrix);
				modelObstacleMap[i][j]->render(program);
			}
		}
	}
	//glDisable(GL_TEXTURE_2D);
}

void Level::free()
{
	glDeleteBuffers(1, &vbo[0]);
	glDeleteVertexArrays(1, &vao[0]);
	
	glDeleteBuffers(1, &vbo[1]);
	glDeleteVertexArrays(1, &vao[1]);
}

void Level::prepareArrays(ShaderProgram &program)
{

	vector<float> vertices;

	// Floor
	vertices.push_back(-size.x / 2.f); vertices.push_back(-size.y / 2.f); vertices.push_back(-size.z / 2.f);
	vertices.push_back(0.f); vertices.push_back(1.f); vertices.push_back(0.f);
	vertices.push_back(0.f); vertices.push_back(0.f);

	vertices.push_back(-size.x / 2.f);  vertices.push_back(-size.y / 2.f); vertices.push_back(size.z / 2.f);
	vertices.push_back(0.f); vertices.push_back(1.f); vertices.push_back(0.f);
	vertices.push_back(0.f); vertices.push_back(size.z/4.f);

	vertices.push_back(size.x / 2.f);  vertices.push_back(-size.y / 2.f); vertices.push_back(size.z / 2.f);
	vertices.push_back(0.f); vertices.push_back(1.f); vertices.push_back(0.f);
	vertices.push_back(size.x/4.f); vertices.push_back(size.z/4.f);

	vertices.push_back(size.x / 2.f);  vertices.push_back(-size.y / 2.f); vertices.push_back(-size.z / 2.f);
	vertices.push_back(0.f); vertices.push_back(1.f); vertices.push_back(0.f);
	vertices.push_back(size.x/4.f); vertices.push_back(0.f);

	glGenVertexArrays(1, &vao[0]);
	glBindVertexArray(vao[0]);
	glGenBuffers(1, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation[0] = program.bindVertexAttribute("position", 3, 8 * sizeof(float), 0);		
	normLocation[0] = program.bindVertexAttribute("normal", 3, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	texCoordLocation[0] = program.bindVertexAttribute("texCoord", 2, 8 * sizeof(float), (void *)(6*sizeof(float)));

	vertices.clear();
	// Front wall
	vertices.push_back(-size.x/2.f); vertices.push_back(-size.y/2.f); vertices.push_back(-size.z/2.f);
	vertices.push_back(0.f); vertices.push_back(0.f); vertices.push_back(1.f);
	vertices.push_back(0.f); vertices.push_back(0.f);

	vertices.push_back(size.x / 2.f); vertices.push_back(-size.y / 2.f); vertices.push_back(-size.z / 2.f);
	vertices.push_back(0.f); vertices.push_back(0.f); vertices.push_back(1.f);
	vertices.push_back(size.x/4.f); vertices.push_back(0.f);

	vertices.push_back(size.x / 2.f); vertices.push_back(size.y / 2.f); vertices.push_back(-size.z / 2.f);
	vertices.push_back(0.f); vertices.push_back(0.f); vertices.push_back(1.f);
	vertices.push_back(size.x/4.f); vertices.push_back(size.y/4.f);

	vertices.push_back(-size.x / 2.f); vertices.push_back(size.y / 2.f); vertices.push_back(-size.z / 2.f);
	vertices.push_back(0.f); vertices.push_back(0.f); vertices.push_back(1.f);
	vertices.push_back(0.f); vertices.push_back(size.y/4.f);

	// Back wall
	vertices.push_back(-size.x / 2.f); vertices.push_back(-size.y / 2.f); vertices.push_back(size.z / 2.f);
	vertices.push_back(0.f); vertices.push_back(0.f); vertices.push_back(-1.f);
	vertices.push_back(0.f); vertices.push_back(0.f);

	vertices.push_back(-size.x / 2.f); vertices.push_back(size.y / 2.f); vertices.push_back(size.z / 2.f);
	vertices.push_back(0.f); vertices.push_back(0.f); vertices.push_back(-1.f);
	vertices.push_back(0.f); vertices.push_back(size.y/4.f);

	vertices.push_back(size.x / 2.f); vertices.push_back(size.y / 2.f); vertices.push_back(size.z / 2.f);
	vertices.push_back(0.f); vertices.push_back(0.f); vertices.push_back(-1.f);
	vertices.push_back(size.x/4.f); vertices.push_back(size.y/4.f);

	vertices.push_back(size.x / 2.f); vertices.push_back(-size.y / 2.f); vertices.push_back(size.z / 2.f);
	vertices.push_back(0.f); vertices.push_back(0.f); vertices.push_back(-1.f);
	vertices.push_back(size.x/4.f); vertices.push_back(0.f);

	// Left wall
	vertices.push_back(-size.x / 2.f); vertices.push_back(-size.y / 2.f); vertices.push_back(-size.z / 2.f);
	vertices.push_back(1.f); vertices.push_back(0.f); vertices.push_back(0.f);
	vertices.push_back(0.f); vertices.push_back(0.f);

	vertices.push_back(-size.x / 2.f); vertices.push_back(size.y / 2.f); vertices.push_back(-size.z / 2.f);
	vertices.push_back(1.f); vertices.push_back(0.f); vertices.push_back(0.f);
	vertices.push_back(0.f); vertices.push_back(size.y/4.f);

	vertices.push_back(-size.x / 2.f); vertices.push_back(size.y / 2.f); vertices.push_back(size.z / 2.f);
	vertices.push_back(1.f); vertices.push_back(0.f); vertices.push_back(0.f);
	vertices.push_back(size.z/4.f); vertices.push_back(size.y/4.f);

	vertices.push_back(-size.x / 2.f); vertices.push_back(-size.y / 2.f); vertices.push_back(size.z / 2.f);
	vertices.push_back(1.f); vertices.push_back(0.f); vertices.push_back(0.f);
	vertices.push_back(size.z/4.f); vertices.push_back(0.f);

	// Right wall
	vertices.push_back(size.x / 2.f); vertices.push_back(-size.y / 2.f); vertices.push_back(-size.z / 2.f);
	vertices.push_back(-1.f); vertices.push_back(0.f); vertices.push_back(0.f);
	vertices.push_back(0.f); vertices.push_back(0.f);

	vertices.push_back(size.x / 2.f); vertices.push_back(-size.y / 2.f); vertices.push_back(size.z / 2.f);
	vertices.push_back(-1.f); vertices.push_back(0.f); vertices.push_back(0.f);
	vertices.push_back(size.z/4.f); vertices.push_back(0.f);

	vertices.push_back(size.x / 2.f); vertices.push_back(size.y / 2.f); vertices.push_back(size.z / 2.f);
	vertices.push_back(-1.f); vertices.push_back(0.f); vertices.push_back(0.f);
	vertices.push_back(size.z/4.f); vertices.push_back(size.y/4.f);

	vertices.push_back(size.x / 2.f); vertices.push_back(size.y / 2.f); vertices.push_back(-size.z / 2.f);
	vertices.push_back(-1.f); vertices.push_back(0.f); vertices.push_back(0.f);
	vertices.push_back(0.f); vertices.push_back(size.y/4.f);

	glGenVertexArrays(1, &vao[1]);
	glBindVertexArray(vao[1]);
	glGenBuffers(1, &vbo[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 4 * 32 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation[1] = program.bindVertexAttribute("position", 3, 8 * sizeof(float), 0);
	normLocation[1] = program.bindVertexAttribute("normal", 3, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	texCoordLocation[1] = program.bindVertexAttribute("texCoord", 2, 8*sizeof(float), (void *)(6*sizeof(float)));

}






























