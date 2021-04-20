#include <iostream>
#include <stdlib.h>
#include <cmath>

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "shader/shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI 3.14

namespace {
	const int window_width = 800;
	const int window_height = 600;

	const int max_depth = 8;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
void draw_triangle(glm::vec3* vertices, Shader shader, glm::mat4 mvp);
glm::vec3 get_translation_vector(glm::vec3* vertices, int index);
void draw_sierpinski_triangle(glm::vec3* vertices, Shader shader, glm::mat4 in_model, int depth);

int main(int argc, char** argv)
{
	srand(time(NULL));

	if (!glfwInit())
	{
		std::cout << "GLFW initialization failed" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "triangle", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, window_width, window_height);
	glEnable(GL_DEPTH_TEST);

	Shader shader("shaders/triangle.vert", "shaders/triangle.frag");

	/* float vertices[] */
	/* { */
	/* 	0.0f, 0.25f, 0.0f, 1.0f, 0.2f, 0.2f, // top point */
	/* 	-0.25f, -0.25f, 0.0f, 0.2f, 1.0f, 0.2f, // left point */
	/* 	0.25f, -0.25f, 0.0f, 0.2f, 0.2f, 1.0f // right point */
	/* }; */

	glm::vec3 vertices[]
	{
		glm::vec3(0.0f, 0.95f, 0.0f),
		glm::vec3(-0.45f, 0.05f, 0.0f),
		glm::vec3(0.45f, 0.05f, 0.0f)
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	/* glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float))); */
	/* glEnableVertexAttribArray(1); */

	shader.use();

	while(!glfwWindowShouldClose(window))
	{
		int triangle_depth = 5;

		process_input(window);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		draw_sierpinski_triangle(vertices, shader, model, 8);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glBindVertexArray(0);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VAO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{ 
		glfwSetWindowShouldClose(window, true);
	}
}

void draw_triangle(glm::vec3* vertices, Shader shader, glm::mat4 mvp)
{
	shader.setMat4("mvp", mvp);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

glm::vec3 get_translation_vector(glm::vec3* vertices, int index)
{
	glm::vec3 translation_vector = glm::vec3(0.0f);

	if (index == 0)
	{
		translation_vector.x = (vertices[1].x + vertices[2].x - vertices[0].x)/2;
		translation_vector.y = (vertices[1].y + vertices[2].y - vertices[0].y)/2;
	}
	else if (index == 1)
	{
		translation_vector.x = (vertices[0].x + vertices[2].x - vertices[1].x)/2;
		translation_vector.y = (vertices[0].y + vertices[2].y - vertices[1].y)/2;
	}
	else if (index == 2)
	{
		translation_vector.x = (vertices[0].x + vertices[1].x - vertices[2].x)/2;
		translation_vector.y = (vertices[0].y + vertices[1].y - vertices[2].y)/2;
	}
	return translation_vector;
}

void draw_sierpinski_triangle(glm::vec3* vertices, Shader shader, glm::mat4 in_model, int depth)
{
	if (depth == 0)
	{
		glm::mat4 model = glm::mat4(1.0f);
		draw_triangle(vertices, shader, model);
		return;
	}


	for (int i = 0; i < 3; i++)
	{
		glm::mat4 model = in_model;
		model = glm::translate(model, get_translation_vector(vertices, i));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
		/* for (int j = 0; j < depth - 1; j++) */
		/* { */
		/* 	model = glm::translate(model, get_translation_vector(vertices, i)); */
		/* 	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f)); */
		/* } */
		/* glm::vec3 color = glm::vec3((float)(rand() % 1000) / 1000); */
		/* std::cout << color.x << std::endl; */
		/* shader.setVec3("uColor", color); */
		draw_triangle(vertices, shader, model);
		draw_sierpinski_triangle(vertices, shader, model, depth - 1);
	}
}

