#include "GLHeaders.h"
#include "Mesh.h"
#include "Shader.h"
#include "vgl.h"
#include "vmath.h"
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

unsigned int vaoID;
Shader shader;
Mesh troutMesh;
float rotationAngle = 0.0f;

int GlutInit(int& argCount, char* argValues[], std::string windowTitle = "No Title", int width = 500, int height = 500);
void Init(std::string, std::string);
void Free();
void Timer(int);
void Display();
void Keypress(unsigned char, int, int);

int main(int argCount, char* argValues[])
{
	GlutInit(argCount, argValues, "My Test of New Routines", 1280, 720);
	Init("project0.vert", "project0.frag");
	glutDisplayFunc(Display);
	glutTimerFunc(1000 / 30, Timer, 1);
	glutKeyboardFunc(Keypress);
	glutMainLoop();
	Free();
}

int GlutInit(int& argCount, char* argValues[], std::string title, int width, int height)
{
	glutInit(&argCount, argValues);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	auto windowID = glutCreateWindow(title.c_str());

	glutInitContextVersion(3, 1);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	auto glewErrCode = glewInit();
	if(glewErrCode != GLEW_OK)
	{
		std::cout << "Glew init failed " << glewErrCode << std::endl;
		exit(EXIT_FAILURE);
	}
	return windowID;
}

void Init(std::string vertexShader, std::string fragmentShader)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glLineWidth(1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);

	shader = Shader(vertexShader, fragmentShader);

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);
	troutMesh = Mesh("trout.obj");
}

void Free()
{
	troutMesh.Free();
	shader.Free();
	glDeleteVertexArrays(1, &vaoID);
	vaoID = -1;
}

void Timer(int value)
{
	rotationAngle += 1.0f;
	if(rotationAngle >= 360.0)
		rotationAngle -= 360.0;
	glutPostRedisplay();
	glutTimerFunc(1000 / 30, Timer, 1);
}

void Display()
{
	glBindVertexArray(vaoID);
	glUseProgram(shader.programID);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto currentMatrix = vmath::rotate(rotationAngle, 0.0f, 1.0f, 0.0f);
	glUniformMatrix4fv(shader.varLocations["modelingTransform"], 1, GL_TRUE, currentMatrix);

	auto vPosition = glGetAttribLocation(shader.programID, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, (const void*)(0));

	troutMesh.Render();

	glFlush();
}

void Keypress(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'q':
		case 'Q':
			exit(0);
			break;
		default:
			break;
	}
}