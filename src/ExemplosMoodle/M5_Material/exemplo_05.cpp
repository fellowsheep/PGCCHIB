
// STB_IMAGE
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "gl_utils.h"

#include <GLFW/glfw3.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define GL_LOG_FILE "gl.log"
#include <iostream>
#include <vector>

#include "Layer.h"

using namespace std;

int g_gl_width = 480;
int g_gl_height = 480;

float PARALLAX_RATE = 0.01f;

GLFWwindow *g_window = NULL;

int loadTexture(unsigned int &texture, char *filename)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	GLfloat max_aniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	// set the maximum!
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);

	int width, height, nrChannels;

	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (nrChannels == 4)
		{
			cout << "Alpha channel" << endl;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			cout << "Without Alpha channel" << endl;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

int main()
{
	// executa instruções de log
	restart_gl_log();

	// inicia OpenGL e libs auxiliares
	start_gl();
	
	// INIT LAYERS
	vector<Layer *> layers;

	Layer *l0 = new Layer;
	l0->filename = "../src/ExemplosMoodle/M5_Material/w0.png";
	l0->z = -0.54;
	l0->offsetx = 0;
	l0->offsety = 0;
	l0->ratex = 0.0;
	l0->ratey = 0;
	layers.push_back(l0);
	loadTexture(l0->tid, l0->filename);

	Layer *l1 = new Layer;
	l1->filename = "../src/ExemplosMoodle/M5_Material/w1.png";
	l1->z = -0.53;
	l1->offsetx = 0;
	l1->offsety = 0;
	l1->ratex = 0.2;
	l1->ratey = 0;
	layers.push_back(l1);
	loadTexture(l1->tid, l1->filename);

	Layer *l2 = new Layer;
	l2->filename = "../src/ExemplosMoodle/M5_Material/w2.png";
	l2->z = -0.52;
	l2->offsetx = 0;
	l2->offsety = 0;
	l2->ratex = 0.4;
	l2->ratey = 0;

	layers.push_back(l2);
	loadTexture(l2->tid, l2->filename);

	Layer *l3 = new Layer;
	l3->filename = "../src/ExemplosMoodle/M5_Material/w3.png";
	l3->z = -0.51;
	l3->offsetx = 0;
	l3->offsety = 0;
	l3->ratex = 0.6;
	l3->ratey = 0;
	layers.push_back(l3);
	loadTexture(l3->tid, l3->filename);

	Layer *l4 = new Layer;
	l4->filename = "../src/ExemplosMoodle/M5_Material/w4.png";
	l4->z = -0.5;
	l4->offsetx = 0;
	l4->offsety = 0;
	l4->ratex = 0.8;
	l4->ratey = 0;
	layers.push_back(l4);
	loadTexture(l4->tid, l4->filename);

	// LOAD TEXTURES

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions   // texture coords
		1.0f, 0.727f, 1.0f, 1.0f,   // top right
		1.0f, -0.727f, 1.0f, 0.0f,  // bottom right
		-1.0f, -0.727f, 0.0f, 0.0f, // bottom left
		-1.0f, 0.727f, 0.0f, 1.0f,  // top left
	};
	unsigned int indices[] = {
		2, 1, 0, // first triangle
		0, 3, 2  // second triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	char vertex_shader[1024 * 256];
	char fragment_shader[1024 * 256];
	parse_file_into_str("../src/ExemplosMoodle/M5_Material/_camadas_vs.glsl", vertex_shader, 1024 * 256);
	parse_file_into_str("../src/ExemplosMoodle/M5_Material/_camadas_fs.glsl", fragment_shader, 1024 * 256);

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *p = (const GLchar *)vertex_shader;
	glShaderSource(vs, 1, &p, NULL);
	glCompileShader(vs);

	// check for compile errors
	int params = -1;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params)
	{
		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", vs);
		print_shader_info_log(vs);
		return 1; // or exit or something
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	p = (const GLchar *)fragment_shader;
	glShaderSource(fs, 1, &p, NULL);
	glCompileShader(fs);

	// check for compile errors
	glGetShaderiv(fs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params)
	{
		fprintf(stderr, "ERROR: GL shader index %i did not compile\n", fs);
		print_shader_info_log(fs);
		return 1; // or exit or something
	}

	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	glGetProgramiv(shader_programme, GL_LINK_STATUS, &params);
	if (GL_TRUE != params)
	{
		fprintf(stderr, "ERROR: could not link shader programme GL index %i\n",
				shader_programme);
		// 		print_programme_info_log( shader_programme );
		return false;
	}

	float previous = glfwGetTime();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	while (!glfwWindowShouldClose(g_window))
	{
		_update_fps_counter(g_window);
		double current_seconds = glfwGetTime();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);

		glViewport(0, 0, g_gl_width, g_gl_height);

		glUseProgram(shader_programme);

		glBindVertexArray(VAO);
		for (int i = 0; i < layers.size(); i++)
		{

			layers[i]->offsetx += layers[i]->ratex * PARALLAX_RATE;

			glUniform1f(glGetUniformLocation(shader_programme, "offsetx"), layers[i]->offsetx);
			glUniform1f(glGetUniformLocation(shader_programme, "offsety"), layers[i]->offsety);
			glUniform1f(glGetUniformLocation(shader_programme, "layer_z"), layers[i]->z);
			// bind Texture
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, layers[i]->tid);
			glUniform1i(glGetUniformLocation(shader_programme, "sprite"), 0);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		glfwPollEvents();
		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(g_window, 1);
		}
		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_UP))
		{
			PARALLAX_RATE += 0.001f;
		}
		if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_DOWN))
		{
			PARALLAX_RATE -= 0.001f;
		}
		// put the stuff we've been drawing onto the display
		glfwSwapBuffers(g_window);
	}

	// close GL context and any other GLFW resources
	glfwTerminate();
	return 0;
}
