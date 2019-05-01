#pragma once
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Model
{
public:
	//vertex/texture coord/normal containers for the model
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;

	//hello

	/*int a = 5;
	int b = 10;
	int c = 5;*/
	
	//interleaved vertex, texture, normal data
	//more efficient than separate VBO's
	std::vector<GLfloat> modelData;

	//set up vertex buffer object
	GLuint VBO;
	//set up vertex array object
	GLuint VAO;
	//set up index buffer object
	//GLuint EBO;
	//shader for the model
	Shader vSh, fSh;
	GLuint shaderProgram;

	float bX_r = 2.0f;
	float bX_l = -2.0f;
	float bY_t = 1.0f;
	float bY_b = -1.0f;

	glm::mat4 translate = glm::mat4(1);
	glm::mat4 rotate = glm::mat4(1);
	glm::mat4 scale = glm::mat4(1);

	Texture* texture;
	
	float velocityX = 0.0f;
	float velocityY = 0.0f;
	float velocityZ = 0.0f;

	//constructor
	Model() 
	{
		//shaders
		//vSh.shaderFileName("..//..//Assets//Shaders//shader_Projection_basicLight.vert");
		//fSh.shaderFileName("..//..//Assets//Shaders//shader_Projection_basicLight.frag");
		vSh.shaderFileName("..//..//Assets//Shaders//shader_projection_lighting_AD.vert");
		fSh.shaderFileName("..//..//Assets//Shaders//shader_projection_lighting_AD.frag");

		vSh.getShader(1);
		fSh.getShader(2);

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vSh.shaderID);
		glAttachShader(shaderProgram, fSh.shaderID);
		glLinkProgram(shaderProgram);

		glDeleteShader(vSh.shaderID);
		glDeleteShader(fSh.shaderID);
	};



	void setBuffers()
	{
		////interleave the vertex/texture/normal data
		for (int i = 0; i < indices.size(); i+=3)
		{
			modelData.push_back(vertices[indices[i]].x);
			modelData.push_back(vertices[indices[i]].y);
			modelData.push_back(vertices[indices[i]].z);
			modelData.push_back(texCoords[indices[i+1]].x);
			modelData.push_back(texCoords[indices[i+1]].y);
			modelData.push_back(normals[indices[i+2]].x);
			modelData.push_back(normals[indices[i+2]].y);
			modelData.push_back(normals[indices[i+2]].z);
	
		}
		std::cout << sizeof(modelData[0]) << std::endl;
		std::cout << modelData.size() << std::endl;
		//
		//OpenGL buffers
		glGenBuffers(1, &VBO);
		// Initialization code using Vertex Array Object (VAO) (done once (unless the object frequently changes))
		glGenVertexArrays(1, &VAO);
		//initialise the index buffer
		//glGenBuffers(1, &this->EBO);

		// Bind Vertex Array Object
		glBindVertexArray(VAO);
		// Copy our vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//buffer size from vector: vector size (number of elements) * size of one element
		glBufferData(GL_ARRAY_BUFFER, modelData.size()*sizeof(modelData[0]) , &modelData[0], GL_STATIC_DRAW);
		// Then set our vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		//set the texture attribute pointer
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		//set normal attrib pointer
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		//set up the EBO

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), &indices[0], GL_STATIC_DRAW);
		
		//Unbind the VAO
		glBindVertexArray(0);
	}

	void updatePlayer()
	{
		translate = glm::translate(translate, glm::vec3(velocityX, velocityY, 0.0f));
	}

	void updatePositions()
	{
		//float r = radius * 2;

		//x += velocityX;
		//y += velocityY;
		//z += velocityZ;

		translate = glm::translate(translate, glm::vec3(velocityX, velocityY, 0.0f));

		//Gets the position of this model
		glm::vec3 position = glm::vec3(translate[3]);

		//get the scale
		float radius = glm::vec3(scale[0]).x;

		if (position.x > (bX_r - radius) || position.x < (bX_l + radius))
			velocityX *= -1.0f;

		if (position.y > (bY_t + radius) || position.y < (bY_b - radius))
			velocityY *= -1.0f;


		/*if (x > (bX_r - r) || x < (bX_l + r))
			velocityX *= -1.0f;

		if (y > (bY_t + r) || y < (bY_b - r))
			velocityY *= -1.0f;*/
	}

	void passAllUniforms()
	{
		////set .obj model
		glUseProgram(shaderProgram);
		//lighting uniforms
		//get and set light colour and position uniform
		
		glm::vec3 lightColour = glm::vec3(1, 1, 1);
		glm::vec3 lightPosition = glm::vec3(0, 1, 4);

		int lightColLocation = glGetUniformLocation(shaderProgram, "lightCol");
		glUniform3fv(lightColLocation, 1, glm::value_ptr(lightColour));


		int lightPositionLocation = glGetUniformLocation(shaderProgram, "lightPos");
		glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition));


		//rotation
		int importModelLocation = glGetUniformLocation(shaderProgram, "uModel");
		glUniformMatrix4fv(importModelLocation, 1, GL_FALSE, glm::value_ptr(translate*rotate*scale));

		int importViewLocation = glGetUniformLocation(shaderProgram, "uView");
		glUniformMatrix4fv(importViewLocation, 1, GL_FALSE, glm::value_ptr(Camera::viewMatrix));

		int importProjectionLocation = glGetUniformLocation(shaderProgram, "uProjection");
		glUniformMatrix4fv(importProjectionLocation, 1, GL_FALSE, glm::value_ptr(Camera::projectionMatrix));

		//set the normal matrix to send to the vertex shader
		//Light calculations take place in model-view space
		//So we calculate the normal matrix in that space
		auto normalMatrix = glm::transpose(glm::inverse(translate*rotate*scale * Camera::viewMatrix));

		int normalMatrixLocation = glGetUniformLocation(shaderProgram, "uNormalMatrix");
		glUniformMatrix4fv(normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));

		glBindTexture(GL_TEXTURE_2D, texture->texture);
	}

	void render()
	{
		//Pass all the uniforms (light location, colour, model, normal, etc)
		passAllUniforms();

		//draw the square 
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size()*sizeof(vertices));
		glBindVertexArray(0);
	}

};