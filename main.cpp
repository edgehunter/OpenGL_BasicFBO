#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

#include "shader_utils.h"

void Initialize();
void InitGL();
void InitProgram();
void InitBuffer();
void InitFBO();
void Loop();
void Shutdown();
void OnWindowResize(GLFWwindow* window, int width, int height);

GLFWwindow* window;
int screenWidth = 640;
int screenHeight = 480;
GLuint vs, fs;
GLuint program;
GLuint attribute_vp;
GLuint vao;
GLuint vbo;
GLuint fbo;
GLuint color_texture;

int main() {
  Initialize();

  Loop();

  Shutdown();

  return 0;
}

void Initialize() {
  InitGL();
  InitProgram();
  InitBuffer();
  InitFBO();
}

void InitGL() {
  glfwInit();
  window = glfwCreateWindow(screenWidth, screenHeight, "FBO Demo", NULL, NULL);

  glfwMakeContextCurrent(window);
  glewInit();

  glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
}

void InitProgram() {
  program = create_program("basicshader.vs.glsl", "basicshader.fs.glsl", vs, fs);

  attribute_vp = get_attrib(program, "vp");
}

void InitBuffer() {
  float points[] = {
    0.0f,  0.5f,  0.0f,
    0.5f, -0.5f,  0.0f,
    -0.5f, -0.5f,  0.0f
  };

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);
  glEnableVertexAttribArray(attribute_vp);
  glVertexAttribPointer (attribute_vp, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

}

void InitFBO() {
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  glGenTextures(1, &color_texture);
  glBindTexture(GL_TEXTURE_2D, color_texture);
  glTexStorage2D(GL_TEXTURE_2D, 9, GL_RGBA8, 512, 512);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_texture, 0);

  static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, draw_buffers);
}

void Loop() {

  glUseProgram(program);

  while (!glfwWindowShouldClose(window)) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);

    glfwPollEvents();
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(window, 1);
    }

  }
}

void Shutdown() {

  glUseProgram(0);

  glDetachShader(program, vs);
  glDetachShader(program, fs);

  glDeleteShader(fs);
  glDeleteShader(vs);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glDeleteProgram(program);

  glfwTerminate();
}

// a call-back function
void OnWindowResize(GLFWwindow* window, int width, int height) {
  screenWidth = width;
  screenHeight = height;
  glViewport(0, 0, screenWidth, screenHeight);
}
