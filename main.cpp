#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader_utils.h"

void Initialize();
void InitGL();
void InitProgramFBO();
void InitProgramScreen();
void InitBuffer();
void InitFBO();
void Loop();
void RenderToFBO();
void RenderToScreen();
void Shutdown();
void OnWindowResize(GLFWwindow* window, int width, int height);

GLFWwindow* window;
int screenWidth = 640;
int screenHeight = 480;
GLuint render2FBOProgram;
GLuint render2ScreenProgram;
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
  InitProgramFBO();
  InitProgramScreen();
  InitProgramFBO();
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

void InitProgramFBO() {
  GLuint vs;
  GLuint fs;
  render2FBOProgram = create_program("vs.glsl", "fbo.fs.glsl", vs, fs);
  glDeleteShader(vs);
  glDeleteShader(fs);
}

void InitProgramScreen() {
  GLuint vs;
  GLuint fs;
  render2ScreenProgram = create_program("vs.glsl", "screen.fs.glsl", vs, fs);
  glDeleteShader(vs);
  glDeleteShader(fs);
}

void InitBuffer() {
  //define the square made up of 2 triangles
  static const GLfloat points[] = {
    //x     y      z       texcoord u and v
    -0.5f,  0.5f,  0.0f,   0.0f, 0.0f,
    0.5f,  0.5f,  0.0f,   1.0f, 0.0f,
    0.5f, -0.5f,  0.0f,   1.0f, 1.0f,

    0.5f, -0.5f,  0.0f,   1.0f, 1.0f,
    -0.5f, -0.5f,  0.0f,   0.0f, 1.0f,
    -0.5f,  0.5f,  0.0f,   0.0f, 0.0f
  };

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  //create buffer for points
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  //tell opengl how to find the coordinate data
  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLubyte*)NULL);
  glEnableVertexAttribArray(0);

  //tell opengl how to find the texcoord data
  glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
}

void InitFBO() {

  //create a framebuffer
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  //create a texture as the backing store for the framebuffer
  glGenTextures(1, &color_texture);
  glBindTexture(GL_TEXTURE_2D, color_texture);
  glTexStorage2D(GL_TEXTURE_2D, 9, GL_RGBA8, 512, 512); //1 = mipmap levels

  //mip map filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  //attach the texture as the color attachment of the framebuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_texture, 0);

  //tell opengl to draw into the color attachment
  static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, draw_buffers);
}

void Loop() {

  //glBindVertexArray(vao);
  //glBindBuffer(GL_ARRAY_BUFFER, vbo);

  while (!glfwWindowShouldClose(window)) {

    RenderToFBO();

    RenderToScreen();

    glfwSwapBuffers(window);

    glfwPollEvents();
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
      glfwSetWindowShouldClose(window, 1);
    }

  }
}

void RenderToFBO() {
  static const GLfloat green[] = { 0.0f, 0.1f, 0.0f, 1.0f };

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glViewport(0, 0, 512, 512); //set view port to texture size
  glClearBufferfv(GL_COLOR, 0, green);
  glUseProgram(render2FBOProgram);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderToScreen() {
  static const GLfloat blue[] = { 0.0f, 0.0f, 0.3f, 1.0f };

  glViewport(0, 0, screenWidth, screenHeight);
  glClearBufferfv(GL_COLOR, 0, blue);
  glBindTexture(GL_TEXTURE_2D, color_texture);
  glUseProgram(render2ScreenProgram);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Shutdown() {

  glUseProgram(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glDeleteProgram(render2FBOProgram);
  glDeleteProgram(render2ScreenProgram);

  glfwTerminate();
}

// a call-back function
void OnWindowResize(GLFWwindow* window, int width, int height) {
  screenWidth = width;
  screenHeight = height;
  glViewport(0, 0, screenWidth, screenHeight);
}
