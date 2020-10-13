#include <cstdlib>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <iostream>
#include <map>
#include <string>
#include "LoadShaders.h"
#include "vgl.h"
#include "vmath.h"

#define BUFFER_OFFSET(x) ((const void*) (x))

GLuint programID;
GLuint vertexBuffers[10];
GLuint arrayBuffers[10];
GLuint elementBuffers[10];
float rotationAngle;
bool elements;
int nbrTriangles = 0;
int materialToUse = 0;
std::map<std::string, GLuint> locationMap;

void Init(std::string, std::string);
int GlutStartUp(int& argCount, char* argValues[], std::string windowTitle = "No Title", int width = 500, int height = 500);
GLuint BuildProgram(std::string, std::string);
void SetAttributes(float lineWidth = 1.0, GLenum face = GL_FRONT_AND_BACK, GLenum fill = GL_FILL);
void BuildObjects();
void GetLocations();
void Timer(int);
void Display();
void Keypress(unsigned char, int, int);

int main(int argCount, char* argValues[])
{
	GlutStartUp(argCount, argValues, "My Test of New Routines");
	Init("project0.vert", "project0.frag");
	glutDisplayFunc(Display);
	glutTimerFunc(1000 / 30, Timer, 1); 
	glutKeyboardFunc(Keypress);
	glutMainLoop();
}

void Init(std::string vertexShader, std::string fragmentShader)
{
	SetAttributes(1.0f, GL_FRONT_AND_BACK, GL_FILL);
	programID = BuildProgram(vertexShader, fragmentShader);
	BuildObjects();
	GetLocations();
}

int GlutStartUp(int& argCount, char* argValues[], std::string title, int width, int height)
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
		std::cerr << "Glew init failed " << glewErrCode << std::endl;
		exit(EXIT_FAILURE);
	}
	return windowID;
}

GLuint BuildProgram(std::string vertexShaderName, std::string fragmentShaderName)
{
	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, vertexShaderName.c_str() },
		{ GL_FRAGMENT_SHADER, fragmentShaderName.c_str() },
		{ GL_NONE, NULL }
	};

	auto program = LoadShaders(shaders);
	if(program == 0)
	{
		std::cerr << "GLSL Program didn't load.  Error \n" << std::endl
			<< "Vertex Shader = " << vertexShaderName << std::endl
			<< "Fragment Shader = " << fragmentShaderName << std::endl;
	}

	glUseProgram(program);
	return program;
}

void SetAttributes(float lineWidth, GLenum face, GLenum fill)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glLineWidth(lineWidth);
	glPolygonMode(face, fill);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
}

void BuildObjects()
{
	// Define the cube faces and colors
	auto scale = 0.5f;
	auto frontBottomLeft =	vmath::vec3(-1.f, -1.f, -1.f) * scale;
	auto frontBottomRight =	vmath::vec3(1.f, -1.f, -1.f) * scale;
	auto frontTopRight =	vmath::vec3(1.f, 1.f, -1.f) * scale;
	auto frontTopLeft =		vmath::vec3(-1.f, 1.f, -1.f) * scale;
	auto frontFaceColor1 =	vmath::vec4(1.f, 1.f, 1.f, 1.f);
	auto frontFaceColor2 =	vmath::vec4(0.f, 0.f, 1.f, 1.f);

	auto rightBottomLeft =	vmath::vec3(1.f, -1.f, -1.f) * scale;
	auto rightBottomRight =	vmath::vec3(1.f, -1.f, 1.f) * scale;
	auto rightTopRight =	vmath::vec3(1.f, 1.f, 1.f) * scale;
	auto rightTopLeft =		vmath::vec3(1.f, 1.f, -1.f) * scale;
	auto rightFaceColor1 =	vmath::vec4(0.f, 1.f, 1.f, 1.f);
	auto rightFaceColor2 =	vmath::vec4(1.f, 1.f, 0.f, 1.f);

	auto backBottomLeft =	vmath::vec3(1.f, -1.f, 1.f) * scale;
	auto backBottomRight =	vmath::vec3(-1.f, -1.f, 1.f) * scale;
	auto backTopRight =		vmath::vec3(-1.f, 1.f, 1.f) * scale;
	auto backTopLeft =		vmath::vec3(1.f, 1.f, 1.f) * scale;
	auto backFaceColor1 =	vmath::vec4(1.f, 0.412f, 0.76f, 1.f);
	auto backFaceColor2 =	vmath::vec4(0.75f, 0.75f, 0.75f, 1.f);

	auto leftBottomLeft =	vmath::vec3(-1.f, -1.f, -1.f) * scale;
	auto leftBottomRight =	vmath::vec3(-1.f, -1.f, 1.f) * scale;
	auto leftTopRight =		vmath::vec3(-1.f, 1.f, 1.f) * scale;
	auto leftTopLeft =		vmath::vec3(-1.f, 1.f, -1.f) * scale;
	auto leftFaceColor1 =	vmath::vec4(1.f, 0.f, 0.f, 1.f);
	auto leftFaceColor2 =	vmath::vec4(0.f, 1.f, 0.f, 1.f);

	auto topBottomLeft =	vmath::vec3(-1.f, 1.f, 1.f) * scale;
	auto topBottomRight =	vmath::vec3(1.f, 1.f, 1.f) * scale;
	auto topTopRight =		vmath::vec3(1.f, 1.f, -1.f) * scale;
	auto topTopLeft =		vmath::vec3(-1.f, 1.f, -1.f) * scale;
	auto topFaceColor1 =	vmath::vec4(1.f, 0.f, 1.f, 1.f);
	auto topFaceColor2 =	vmath::vec4(1.f, 0.584f, 0.f, 1.f);

	auto bottomBottomLeft =	vmath::vec3(-1.f, -1.f, -1.f) * scale;
	auto bottomBottomRight =vmath::vec3(1.f, -1.f, -1.f) * scale;
	auto bottomTopRight =	vmath::vec3(1.f, -1.f, 1.f) * scale;
	auto bottomTopLeft =	vmath::vec3(-1.f, -1.f, 1.f) * scale;
	auto bottomFaceColor1 = vmath::vec4(0.294f, 0.f, 0.51f, 1.f);
	auto bottomFaceColor2 = vmath::vec4(1.f, 0.843f, 0.f, 1.f);

	// Vertex mesh
	GLfloat vertices[] =
	{
		// Front
		frontBottomLeft[0], frontBottomLeft[1], frontBottomLeft[2], 1.0f,
		frontBottomRight[0], frontBottomRight[1], frontBottomRight[2], 1.0f,
		frontTopLeft[0], frontTopLeft[1], frontTopLeft[2], 1.0f,
		frontTopLeft[0], frontTopLeft[1], frontTopLeft[2], 1.0f,
		frontBottomRight[0], frontBottomRight[1], frontBottomRight[2], 1.0f,
		frontTopRight[0], frontTopRight[1], frontTopRight[2], 1.0f,

		// Right
		rightBottomLeft[0], rightBottomLeft[1], rightBottomLeft[2], 1.0f,
		rightBottomRight[0], rightBottomRight[1], rightBottomRight[2], 1.0f,
		rightTopLeft[0], rightTopLeft[1], rightTopLeft[2], 1.0f,
		rightTopLeft[0], rightTopLeft[1], rightTopLeft[2], 1.0f,
		rightBottomRight[0], rightBottomRight[1], rightBottomRight[2], 1.0f,
		rightTopRight[0], rightTopRight[1], rightTopRight[2], 1.0f,

		// Back
		backBottomLeft[0], backBottomLeft[1], backBottomLeft[2], 1.0f,
		backBottomRight[0], backBottomRight[1], backBottomRight[2], 1.0f,
		backTopLeft[0], backTopLeft[1], backTopLeft[2], 1.0f,
		backTopLeft[0], backTopLeft[1], backTopLeft[2], 1.0f,
		backBottomRight[0], backBottomRight[1], backBottomRight[2], 1.0f,
		backTopRight[0], backTopRight[1], backTopRight[2], 1.0f,

		// Left
		leftBottomLeft[0], leftBottomLeft[1], leftBottomLeft[2], 1.0f,
		leftBottomRight[0], leftBottomRight[1], leftBottomRight[2], 1.0f,
		leftTopLeft[0], leftTopLeft[1], leftTopLeft[2], 1.0f,
		leftTopLeft[0], leftTopLeft[1], leftTopLeft[2], 1.0f,
		leftBottomRight[0], leftBottomRight[1], leftBottomRight[2], 1.0f,
		leftTopRight[0], leftTopRight[1], leftTopRight[2], 1.0f,

		// Top
		topBottomLeft[0], topBottomLeft[1], topBottomLeft[2], 1.0f,
		topBottomRight[0], topBottomRight[1], topBottomRight[2], 1.0f,
		topTopLeft[0], topTopLeft[1], topTopLeft[2], 1.0f,
		topTopLeft[0], topTopLeft[1], topTopLeft[2], 1.0f,
		topBottomRight[0], topBottomRight[1], topBottomRight[2], 1.0f,
		topTopRight[0], topTopRight[1], topTopRight[2], 1.0f,

		// Bottom
		bottomBottomLeft[0], bottomBottomLeft[1], bottomBottomLeft[2], 1.0f,
		bottomBottomRight[0], bottomBottomRight[1], bottomBottomRight[2], 1.0f,
		bottomTopLeft[0], bottomTopLeft[1], bottomTopLeft[2], 1.0f,
		bottomTopLeft[0], bottomTopLeft[1], bottomTopLeft[2], 1.0f,
		bottomBottomRight[0], bottomBottomRight[1], bottomBottomRight[2], 1.0f,
		bottomTopRight[0], bottomTopRight[1], bottomTopRight[2], 1.0f
	};

	// Per-vertex color
	GLfloat colors[] =
	{
		// Front
		frontFaceColor1[0], frontFaceColor1[1], frontFaceColor1[2], frontFaceColor1[3],
		frontFaceColor1[0], frontFaceColor1[1], frontFaceColor1[2], frontFaceColor1[3],
		frontFaceColor1[0], frontFaceColor1[1], frontFaceColor1[2], frontFaceColor1[3],
		frontFaceColor2[0], frontFaceColor2[1], frontFaceColor2[2], frontFaceColor2[3],
		frontFaceColor2[0], frontFaceColor2[1], frontFaceColor2[2], frontFaceColor2[3],
		frontFaceColor2[0], frontFaceColor2[1], frontFaceColor2[2], frontFaceColor2[3],

		// Right
		rightFaceColor1[0], rightFaceColor1[1], rightFaceColor1[2], rightFaceColor1[3],
		rightFaceColor1[0], rightFaceColor1[1], rightFaceColor1[2], rightFaceColor1[3],
		rightFaceColor1[0], rightFaceColor1[1], rightFaceColor1[2], rightFaceColor1[3],
		rightFaceColor2[0], rightFaceColor2[1], rightFaceColor2[2], rightFaceColor2[3],
		rightFaceColor2[0], rightFaceColor2[1], rightFaceColor2[2], rightFaceColor2[3],
		rightFaceColor2[0], rightFaceColor2[1], rightFaceColor2[2], rightFaceColor2[3],

		// Back
		backFaceColor1[0], backFaceColor1[1], backFaceColor1[2], backFaceColor1[3],
		backFaceColor1[0], backFaceColor1[1], backFaceColor1[2], backFaceColor1[3],
		backFaceColor1[0], backFaceColor1[1], backFaceColor1[2], backFaceColor1[3],
		backFaceColor2[0], backFaceColor2[1], backFaceColor2[2], backFaceColor2[3],
		backFaceColor2[0], backFaceColor2[1], backFaceColor2[2], backFaceColor2[3],
		backFaceColor2[0], backFaceColor2[1], backFaceColor2[2], backFaceColor2[3],

		// Left
		leftFaceColor1[0], leftFaceColor1[1], leftFaceColor1[2], leftFaceColor1[3],
		leftFaceColor1[0], leftFaceColor1[1], leftFaceColor1[2], leftFaceColor1[3],
		leftFaceColor1[0], leftFaceColor1[1], leftFaceColor1[2], leftFaceColor1[3],
		leftFaceColor2[0], leftFaceColor2[1], leftFaceColor2[2], leftFaceColor2[3],
		leftFaceColor2[0], leftFaceColor2[1], leftFaceColor2[2], leftFaceColor2[3],
		leftFaceColor2[0], leftFaceColor2[1], leftFaceColor2[2], leftFaceColor2[3],

		// Top
		topFaceColor1[0], topFaceColor1[1], topFaceColor1[2], topFaceColor1[3],
		topFaceColor1[0], topFaceColor1[1], topFaceColor1[2], topFaceColor1[3],
		topFaceColor1[0], topFaceColor1[1], topFaceColor1[2], topFaceColor1[3],
		topFaceColor2[0], topFaceColor2[1], topFaceColor2[2], topFaceColor2[3],
		topFaceColor2[0], topFaceColor2[1], topFaceColor2[2], topFaceColor2[3],
		topFaceColor2[0], topFaceColor2[1], topFaceColor2[2], topFaceColor2[3],

		// Bottom
		bottomFaceColor1[0], bottomFaceColor1[1], bottomFaceColor1[2], bottomFaceColor1[3],
		bottomFaceColor1[0], bottomFaceColor1[1], bottomFaceColor1[2], bottomFaceColor1[3],
		bottomFaceColor1[0], bottomFaceColor1[1], bottomFaceColor1[2], bottomFaceColor1[3],
		bottomFaceColor2[0], bottomFaceColor2[1], bottomFaceColor2[2], bottomFaceColor2[3],
		bottomFaceColor2[0], bottomFaceColor2[1], bottomFaceColor2[2], bottomFaceColor2[3],
		bottomFaceColor2[0], bottomFaceColor2[1], bottomFaceColor2[2], bottomFaceColor2[3]
	};
	
	glGenVertexArrays(1, vertexBuffers);
	glBindVertexArray(vertexBuffers[0]);

	nbrTriangles = 12;
	glGenBuffers(1, &(arrayBuffers[0]));
	glBindBuffer(GL_ARRAY_BUFFER, arrayBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

	auto vPosition = glGetAttribLocation(programID, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	auto vColor = glGetAttribLocation(programID, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));
}

void GetLocations()
{
	GLint numberBlocks;
	char uniformName[1024];
	int nameLength;
	GLint size;
	GLenum type;
	glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &numberBlocks);
	for(auto blockIndex = 0; blockIndex < numberBlocks; blockIndex++)
	{
		glGetActiveUniform(programID, blockIndex, 1024, &nameLength, &size, &type, uniformName);
		std::cout << uniformName << std::endl;
		locationMap[std::string(uniformName)] = blockIndex;
	}
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// needed
	auto currentMatrix = vmath::rotate(rotationAngle, 0.0f, 1.0f, 0.0f);

	glUniformMatrix4fv(locationMap["modelingTransform"], 1, GL_TRUE, currentMatrix);

	glBindVertexArray(vertexBuffers[0]);
	glBindBuffer(GL_ARRAY_BUFFER, arrayBuffers[0]);
	glDrawArrays(GL_TRIANGLES, 0, nbrTriangles * 3);

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