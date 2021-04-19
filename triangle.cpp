#include <iostream>
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

	const float triangle_height = 0.5f;
	float triangle_width;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

int main(int argc, char** argv)
{
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

	float vertices[]
	{
		0.0f, 0.25f, 0.0f, 1.0f, 0.2f, 0.2f, // top point
		-0.25f, -0.25f, 0.0f, 0.2f, 1.0f, 0.2f, // left point
		0.25f, -0.25f, 0.0f, 0.2f, 0.2f, 1.0f // right point
	};

	triangle_width = tan(30 * PI/180) * triangle_height * 2;

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glm::mat4 mvp = glm::mat4(1.0f);

	shader.use();
	shader.setMat4("mvp", mvp);

	while(!glfwWindowShouldClose(window))
	{
		process_input(window);

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

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
