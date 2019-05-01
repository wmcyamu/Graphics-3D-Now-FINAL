#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera
{
public:


	static glm::mat4 viewMatrix;
	static glm::mat4 projectionMatrix;

	static glm::vec3 position;
	static glm::vec3 target;

	//default constructor
	//Camera()
	//{
		//create camera view matrix
		//the 'lookAt' function
		//specify the position, target and up vector of the camera
		//position: 0,0,4   Target: 0,0,0   up: y axis
		//viewMatrix = glm::lookAt(cameraPosition, glm::normalize(cameraPosition - cameraTarget), cameraUp);
	//}

	/*void updateCamera()
	{
		cameraPosition = glm::vec3(camXPos, camYPos, camZPos);
		cameraTarget = glm::vec3(camXTarget, camYTarget, camZTarget);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	}*/

	static void update()
	{
		//builds a view matrix and projection matrix
		//..

		//view matrix
		//viewMatrix = glm::lookAt(position, glm::normalize(position - target), glm::vec3(0, 1, 0));
		viewMatrix = glm::lookAt(position, target, glm::vec3(0, 1, 0));

		//Get the window size
		int w, h;
		SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &w, &h);

		//projection matrix
		projectionMatrix = glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.1f, 100.0f);
	}

	static void setupCamera()
	{
		//default position
		position = glm::vec3(0, 0, 4);

		//default target
		target = glm::vec3(0);

		update();
	}

};

glm::mat4 Camera::viewMatrix       = glm::mat4(1);
glm::mat4 Camera::projectionMatrix = glm::mat4(1);

glm::vec3 Camera::position = glm::vec3(0);
glm::vec3 Camera::target = glm::vec3(0);


// 0, 0, 4 -- pos
// 0, 0, 0 -- target