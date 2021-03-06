#include "Lamp.h"
#include "Game.h"
unsigned int Lamp::depthMapFBO;
const unsigned int Lamp::SHADOW_WIDTH = 1024, Lamp::SHADOW_HEIGHT = 1024;
Lamp::Lamp()
{
	throwShadows = true;
	if (!Game::isServer) {
		color = vec3(1);

	/*	for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
				SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);*/
		Game::lamps.push_back(this);
	}
}


Lamp::~Lamp()
{
	Game::lamps.erase(std::remove(Game::lamps.begin(), Game::lamps.end(), this), Game::lamps.end());
}

void Lamp::Render(mat4 view, mat4 projection)
{

}

void Lamp::SetupLampComp()
{
	unsigned int dMFBO;
	glGenFramebuffers(1, &dMFBO);
	Lamp::depthMapFBO = dMFBO;
}
