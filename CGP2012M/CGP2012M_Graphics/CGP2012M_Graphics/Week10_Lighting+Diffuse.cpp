//#include <cstdio>
//#include <cstdlib>
#include <iostream>
#include <vector>

//include shape, shader header files
#include "GLerror.h"
#include "SDL_Start.h"
#include "Triangle_T.h"
#include "Circle.h"
#include "CircleTexture.h"
#include "Square.h"
#include "Camera.h"
#include "Cube.h"
#include "Model.h"
#include "ModelLoaderClass.h"
#ifndef TEXTURECLASS_H
#define TEXTURECLASS_H
#ifndef SHADERCLASS_H
#define SHADERCLASS_H

// // GLEW - OpenGL Extension Wrangler - http://glew.sourceforge.net/
// // NOTE: include before SDL.h
#ifndef GLEW_H
#define GLEW_H
//#include <GL/glew.h>
#include "windows.h"

// SDL - Simple DirectMedia Layer - https://www.libsdl.org/
#ifndef SDL_H
#define SDL_H
#include "SDL.h"
#include "SDL_image.h"
//#include "SDL_mixer.h"
//#include "SDL_ttf.h"

// // - OpenGL Mathematics - https://glm.g-truc.net/
#define GLM_FORCE_RADIANS // force glm to use radians
// // NOTE: must do before including GLM headers
// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//***************
//variables
SDL_Event event;
SDL_Window *win;
bool windowOpen = true;
bool isFullScreen = false;
float bubbleSpeed = -0.001f;
float radius;
//screen boundaries for collision tests
float bX_r = 2.0f;
float bX_l = -2.0f;
float bY_t = 1.0f;
float bY_b = -1.0f;
//screen centre
float centreX = 0.0f;
float centreY = 0.0f;
//window aspect/width/height
int w;
int h;
float aspect;
int left;
int newwidth;
int newheight;

//transform matrices
glm::mat4 modelMatrix;
glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;
glm::mat4 normalMatrix;

glm::mat4 translate;
glm::mat4 rotate;
glm::mat4 scale;
glm::mat4 backgroundTranslate;
glm::mat4 backgroundScale;
glm::vec3 b_scaleFactor;
glm::mat4 modelRotate;
glm::mat4 modelScale;
glm::mat4 modelTranslate;
float angle = 0;

//loac camera variables
glm::vec3 camPos;
glm::vec3 camTarget;

bool flag = true;

glm::vec3 lightCol;
glm::vec3 lightPosition;
glm::vec3 viewPosition;
float ambientIntensity;

//**************
//function prototypes
//CircleTexture updatePositions(CircleTexture c);
void handleInput();


std::vector<Model*> bubbles;
Model* player;

int main(int argc, char *argv[]) {
	//start and initialise SDL
	SDL_Start sdl;
	SDL_GLContext context = sdl.Init();
	win = sdl.win;

	SDL_GetWindowSize(win, &w, &h);
	glViewport(0, 0, w, h);
	aspect = (float)w / (float)h;

	//error class
	GLerror glerr;
	int errorLabel;

	//GLEW initialise
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	//register debug callback
	if (glDebugMessageCallback)
	{
		std::cout << "Registering OpenGL Debug callback function" << std::endl;
		glDebugMessageCallback(glerr.openglCallbackFunction, &errorLabel);
		glDebugMessageControl(GL_DONT_CARE,
			GL_DONT_CARE,
			GL_DONT_CARE,
			0,
			NULL,
			true);
	}

	//*****************************************************
	//OpenGL specific data

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//*********************
//create texture collection
//create textures - space for 4, but only using 2
	Texture texArray[4];
	//background texture
	texArray[0].load("..//..//Assets//Textures//space.png");
	texArray[0].setBuffers();
	texArray[1].load("..//..//Assets//Textures//small-bubble.png");
	texArray[1].setBuffers();

	texArray[2].load("..//..//Assets//Textures//carbon-fibre-seamless-texture.jpg");
	texArray[2].setBuffers();


	//objects
	//create background square
	Square background;


	//create circles
	float randValue, randValue2;
	srand(time(0));

	player = new Model();
	ModelImport modelLoader;

	//Load all the stuff into the model (verts, etc)
	modelLoader.LoadOBJ2("..//..//Assets//Models//blenderCube.obj", player->vertices, player->texCoords, player->normals, player->indices);

	player->texture = &texArray[2];
	player->scale = glm::scale(player->scale, glm::vec3(0.2f));

	player->setBuffers();

	const int bubblesAmount = 5;

	for (int i = 0; i < bubblesAmount; i++)
	{
		//Generate number between -1 and 1
		float rx = ((rand() / (float)RAND_MAX) - 0.5f) * 2.0f;
		float ry = ((rand() / (float)RAND_MAX) - 0.5f) * 2.0f;

		float randomScale = 0.5f;// (rand() / (float)RAND_MAX);
		
		//Make a new model with nothing in it
		auto bubble = new Model();

		//Load all the stuff into the model (verts, etc)
		modelLoader.LoadOBJ2("..//..//Assets//Models//blenderSphere.obj", bubble->vertices, bubble->texCoords, bubble->normals, bubble->indices);

		//Add the random position to this bubble's position
		bubble->translate = glm::translate(bubble->translate, glm::vec3(rx, ry, 0));
		bubble->scale = glm::scale(bubble->scale, glm::vec3(randomScale * 0.5f));

		//Set their velocity to something that will make them move randomly
		bubble->velocityX = ((rand() / (float)RAND_MAX) - 0.5f) * 0.1f;
		bubble->velocityY = ((rand() / (float)RAND_MAX) - 0.5f) * 0.1f;

		//Set the texture
		bubble->texture = &texArray[1];

		//Set the buffers and all that stuff
		bubble->setBuffers();

		//Add it to the list of bubbles
		bubbles.push_back(bubble);
	}


	//create model loader

	

	errorLabel = 0;


	errorLabel = 2;


	//OpenGL buffers
	background.setBuffers();

	errorLabel = 3;
	//*****************************************
	//set uniform variables
	int transformLocation;
	int modelLocation;
	int viewLocation;
	int projectionLocation;
	int importModelLocation;
	int importViewLocation;
	int importProjectionLocation;
	int backgroundColourLocation;
	int ambientIntensityLocation;
	int modelColourLocation;
	int modelAmbientLocation;
	int lightColLocation;
	int normalMatrixLocation;
	int lightPositionLocation;
	int viewPositionLocation;

	Camera::setupCamera();

	GLuint currentTime = 0;
	GLuint lastTime = 0;
	GLuint elapsedTime = 0;

	//lighting for the model
	//Light position setting
	lightPosition = glm::vec3(1.0f, 0.0f, 0.5f);
	//light colour setting
	// Candle:  r:1.0 g:0.57 b:0.16
	// 100W bulb: r:1.0 g:0.84 b:0.66
	// direct sunlight: r:1.0 g:1.0 b:0.98
	glm::vec3 lightColour = glm::vec3(1.0f, 1.0f, 0.98f);

	//light for the background
	//light distance setting
	ambientIntensity = 1.0f;
	lightCol = glm::vec3(1.0f, 1.0f, 0.98f);

	//initialise transform matrices 
	//orthographic (2D) projection
	//projectionMatrix = glm::ortho(0.0f, 4.0f, 0.0f, 3.0f, -1.0f, 100.0f);
	//perspective (3D) projection
	
	//initialise view matrix to '1'

	backgroundScale = glm::mat4(1.0f);
	backgroundTranslate = glm::mat4(1.0f);
	modelScale = glm::mat4(1.0f);
	modelRotate = glm::mat4(1.0f);
	modelTranslate = glm::mat4(1.0f);

	//once only scale to background, and translate to centre
	b_scaleFactor = { 80.0f, 70.0f, 1.0f };
	backgroundScale = glm::scale(backgroundScale, glm::vec3(b_scaleFactor));
	backgroundTranslate = glm::translate(backgroundTranslate, glm::vec3(0.0f, 0.0f, -5.0f));


	errorLabel = 4;

	//*****************************
	//'game' loop
	while (windowOpen)
	{
		//*************************
		//****************************
		// OpenGL calls.
		
		//rebuilds the view and projection matrices
		Camera::update();

		glClearColor(1.0f, 1.0f, 1.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//process inputs
		handleInput();

		//time
		currentTime = SDL_GetTicks();
		elapsedTime = currentTime - lastTime;
		lastTime = currentTime;

		//update camera matrix
		//camera only moves side to side, formards and backwards (no rotation)
		// set position, target, up direction

		errorLabel = 5;

		//background
		glUseProgram(background.shaderProgram);
		//set background lighting
		backgroundColourLocation = glGetUniformLocation(background.shaderProgram, "uLightColour");
		glProgramUniform3fv(background.shaderProgram, backgroundColourLocation, 1, glm::value_ptr(lightCol));
		//light distance
		ambientIntensityLocation = glGetUniformLocation(background.shaderProgram, "uAmbientIntensity");
		glProgramUniform1f(background.shaderProgram, ambientIntensityLocation, ambientIntensity);

		//set background image
		modelLocation = glGetUniformLocation(background.shaderProgram, "uModel");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(backgroundTranslate*backgroundScale));
		viewLocation = glGetUniformLocation(background.shaderProgram, "uView");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(Camera::viewMatrix));
		projectionLocation = glGetUniformLocation(background.shaderProgram, "uProjection");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(Camera::projectionMatrix));
		glBindTexture(GL_TEXTURE_2D, texArray[0].texture);
		background.render();


		for (int i = 0; i < bubblesAmount; i++)
		{
			bubbles[i]->updatePositions();
			bubbles[i]->render();
		}

		player->updatePlayer();
		player->render();

		float tilt = sin(SDL_GetTicks() * 0.01);

		Camera::position = glm::vec3(player->translate[3]) - glm::vec3(0, 0, -4);
		Camera::target = glm::vec3(player->translate[3]);


		//set to wireframe so we can see the circles
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		SDL_GL_SwapWindow(sdl.win);

	}//end loop

	//****************************
	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	SDL_GL_DeleteContext(context);

	SDL_Quit();
	return 0;
}


void checkForKeyPresses()
{
	/*if (keyState[SDL_SCANCODE_W])
	{
	}*/
}

void handleInput()
{
	//*****************************
		//SDL handled input
		//Any input to the program is done here

	checkForKeyPresses();

	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			windowOpen = false;
		}
		/*if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_w)
			{
				player->velocityY = 0.1f;
			}
		}*/
		if (event.type == SDL_WINDOWEVENT)
		{
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				std::cout << "Window resized w:" << w << " h:" << h << std::endl;
				SDL_GetWindowSize(win, &w, &h);
				newwidth = h * aspect;
				left = (w - newwidth) / 2;
				glViewport(left, 0, newwidth, h);
				break;
				

			default:
				break;
			}
		}
		
	}
}
#endif
#endif
#endif
#endif