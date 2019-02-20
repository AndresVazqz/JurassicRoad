#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE


#include <string>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "AssimpModel.h"


using namespace std;


// Class Level renders a very simple room with textures


class Level
{

public:
	// Levels can only be created inside an OpenGL context
	static Level *createLevel(const glm::vec3 &levelSize, ShaderProgram &program, const string &floorFile, const string &wallFile);

	Level(const glm::vec3 &levelSize, ShaderProgram &program, const string &floorFile, const string &wallFile);
	~Level();

	void update();

	void render(ShaderProgram &program, glm::mat4 viewMatrix);
	void free();

private:
	void prepareArrays(ShaderProgram &program);

private:
	GLuint vao[2];
	GLuint vbo[2];
	GLint posLocation[2], texCoordLocation[2], normLocation[2];
	glm::ivec2 map;
	glm::vec3 size;
	vector<AssimpModel*> models;
	vector<vector <AssimpModel*> > modelFloorMap;
	vector<vector <AssimpModel*> > modelObstacleMap;
	vector < vector <bool> >obstacleMap;
	vector <int> biomeMap;
	Texture floor, wall;
	int step;
};


#endif // _LEVEL_INCLUDE


