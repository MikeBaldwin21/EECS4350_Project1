#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include "LoadShaders.h"
#include "vmath.h"
#include "vgl.h"
#include <map>
#include <vector>
#include "PositionInfo.h"

#define BUFFER_OFFSET(x)  ((const void*) (x))

GLuint programID;

/*
* Arrays to store the indices/names of the Vertex Array Objects and
* Buffers.  Rather than using the books enum approach I've just
* gone out and made a bunch of them and will use them as needed.
*
* Not the best choice I'm sure.
*/
GLuint vertexBuffers[10];
GLuint arrayBuffers[10];
GLuint elementBuffers[10];

/*
* Global variables
*   The location for the transformation and the current rotation
*   angle are set up as globals since multiple methods need to
*   access them.
*/
float rotationAngle;
bool elements;
int nbrTriangles, materialToUse = 0;
int startTriangle = 0, endTriangle = 12;
bool rotationOn = false;

std::map<std::string, GLuint> locationMap;
std::vector<Drawable*> objectList;

int GlutStartUp(int& argCount, char* argValues[], std::string windowTitle = "No Title", int width = 1280, int height = 720);
GLuint BuildProgram(std::string vertexShaderName, std::string fragmentShaderName);
void SetAttributes(float lineWidth = 1.0, GLenum face = GL_FRONT_AND_BACK, GLenum fill = GL_FILL);
void BuildObjects();
void GetLocations();
void Init(std::string vertexShader, std::string fragmentShader);
void Readfile(std::string filename, std::vector<float>& vertices, std::vector<unsigned int>& indices);
void Readfile2(std::string filename, std::vector<float>& vertices, std::vector<float>& normals);
void Readfile3(std::string filename, std::vector<float>& vertices, std::vector<float>& normals, std::vector<float>& texture);
void timer(int value);
void display();
void keypress(unsigned char key, int x, int y);

int main(int argCount, char* argValues[])
{
	GlutStartUp(argCount, argValues, "Objects, Angles, and Perspectives!");
	Init("project0a.vert", "project0.frag");
	glutDisplayFunc(display);
	glutTimerFunc(1000 / 30, timer, 1);
	glutKeyboardFunc(keypress);
	glUniformMatrix4fv(locationMap["projectionTransform"], 1, GL_FALSE, vmath::ortho(-6.0f, 6.0f, -6.0f, 6.0f, -20.0f, 20.0f));
	glUniformMatrix4fv(locationMap["viewingTransform"], 1, GL_FALSE, vmath::scale(1.0f));
	glutMainLoop();
}

/*
 * Routine to encapsulate some of the startup routines for GLUT.  It returns the window ID of the
 * single window that is created.
 */
int GlutStartUp(int& argCount, char* argValues[], std::string title, int width, int height)
{
	int windowID;
	GLenum glewErrCode;

	glutInit(&argCount, argValues);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	windowID = glutCreateWindow(title.c_str());

	glutInitContextVersion(3, 1);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glewErrCode = glewInit();
	if(glewErrCode != GLEW_OK)
	{
		std::cerr << "Glew init failed " << glewErrCode << std::endl;
		exit(EXIT_FAILURE);
	}
	return windowID;
}

/*
 * Use the author's routines to build the program and return the program ID.
 */
GLuint BuildProgram(std::string vertexShaderName, std::string fragmentShaderName)
{
	/*
	*  Use the Books code to load in the shaders.
	*/
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, vertexShaderName.c_str() },
		{ GL_FRAGMENT_SHADER, fragmentShaderName.c_str() },
		{ GL_NONE, NULL } };
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

/*
 * Set up the clear color, lineWidth, and the fill type for the display.
 */
void SetAttributes(float lineWidth, GLenum face, GLenum fill)
{
	/*
	* I'm using wide lines so that they are easier to see on the screen.
	* In addition, this version fills in the polygons rather than leaving it
	* as lines.
	*/
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glLineWidth(lineWidth);
	glPolygonMode(face, fill);
	glEnable(GL_DEPTH_TEST);
}

/*
 * read and/or build the objects to be displayed.  Also sets up attributes that are
 * vertex related.
 */
void BuildObjects()
{

	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f, 1.0f, -0.5f,  0.5f,  0.5f, 1.0f, -0.5f, -0.5f,  0.5f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, -0.5f,  0.5f,  0.5f, 1.0f, -0.5f,  0.5f, -0.5f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, -0.5f,  0.5f, -0.5f, 1.0f,  0.5f,  0.5f, -0.5f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f,  0.5f,  0.5f, -0.5f, 1.0f,  0.5f, -0.5f, -0.5f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f,  0.5f, -0.5f, -0.5f, 1.0f,  0.5f, -0.5f,  0.5f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f,  0.5f, -0.5f,  0.5f, 1.0f, -0.5f, -0.5f,  0.5f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, -0.5f,  0.5f,  0.5f, 1.0f,  0.5f,  0.5f,  0.5f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f,  0.5f,  0.5f,  0.5f, 1.0f,  0.5f, -0.5f,  0.5f, 1.0f,
		-0.5f,  0.5f,  0.5f, 1.0f, -0.5f,  0.5f, -0.5f, 1.0f,  0.5f,  0.5f,  0.5f, 1.0f,
		0.5f,  0.5f,  0.5f, 1.0f,  0.5f,  0.5f, -0.5f, 1.0f, -0.5f,  0.5f, -0.5f, 1.0f,
		0.5f, -0.5f,  0.5f, 1.0f,  0.5f, -0.5f, -0.5f, 1.0f,  0.5f,  0.5f, -0.5f, 1.0f,
		0.5f, -0.5f,  0.5f, 1.0f,  0.5f,  0.5f, -0.5f, 1.0f,  0.5f,  0.5f,  0.5f, 1.0f
	};

	GLfloat colors[] = {
		0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f, 1.0f
	};

	glGenVertexArrays(1, vertexBuffers);
	glBindVertexArray(vertexBuffers[0]);

	/*
	 * Test code for internal object.
	 */
	nbrTriangles = 12;
	glGenBuffers(1, &(arrayBuffers[0]));
	glBindBuffer(GL_ARRAY_BUFFER, arrayBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
	// offset in bytes   size in bytes     ptr to data    
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
	/*
	 * Set up variables into the shader programs (Note:  We need the
	 * shaders loaded and built into a program before we do this)
	 */
	GLuint vPosition = glGetAttribLocation(programID, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(programID, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));


	//	objectList.push_back(new PositionInfo(vertexBuffers[0], 
	//		arrayBuffers[0], nbrTriangles * 3, 
	//		locationMap["modelingTransform"], vmath::scale(1.0f), 
	//		vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	for(auto offset = 1.06065f; offset < 6.0f; offset += 1.06065f)
	{
		objectList.push_back(new PositionInfo(vertexBuffers[0], arrayBuffers[0], nbrTriangles * 3, locationMap["modelingTransform"], vmath::translate(offset, 0.0f, -offset), vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		objectList.push_back(new PositionInfo(vertexBuffers[0], arrayBuffers[0], nbrTriangles * 3, locationMap["modelingTransform"], vmath::translate(-offset, 0.0f, -offset), vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		objectList.push_back(new PositionInfo(vertexBuffers[0], arrayBuffers[0], nbrTriangles * 3, locationMap["modelingTransform"], vmath::translate(0.0f, offset, 0.0f), vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	}

	/*
	 * Test code for a file to read in...
	 */
	std::vector<float> filevertices, filenormals, filetextures;
	Readfile3("elephantTriangles.obj", filevertices, filenormals, filetextures);
	glGenVertexArrays(1, &(vertexBuffers[1]));
	glBindVertexArray(vertexBuffers[1]);
	glGenBuffers(1, &(arrayBuffers[1]));
	glBindBuffer(GL_ARRAY_BUFFER, arrayBuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, filevertices.size() * sizeof(float) + (filevertices.size() / 3) * 4 * sizeof(float), NULL, GL_STATIC_DRAW);
	// offset in bytes   size in bytes     ptr to data    
	glBufferSubData(GL_ARRAY_BUFFER, 0, filevertices.size() * sizeof(float), filevertices.data());
	// Create a color array since colors don't come in...
	auto filecolors = new float[filevertices.size() / 3 * 4];
	for(unsigned int currentVertex = 0; currentVertex < filevertices.size() / 3; currentVertex++)
	{
		filecolors[currentVertex * 4] = 1.0f;
		filecolors[currentVertex * 4 + 1] = 0.0f;
		filecolors[currentVertex * 4 + 2] = 0.0f;
		filecolors[currentVertex * 4 + 3] = 1.0f;
	}
	glBufferSubData(GL_ARRAY_BUFFER, filevertices.size() * sizeof(float), filevertices.size() / 3 * 4 * sizeof(float), filecolors);
	vPosition = glGetAttribLocation(programID, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	vColor = glGetAttribLocation(programID, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(filevertices.size() * sizeof(float)));

	objectList.push_back(new PositionInfo(vertexBuffers[1], arrayBuffers[1], filevertices.size() / 3, locationMap["modelingTransform"], vmath::scale(1.0f, 1.0f, 1.0f), vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f)));



	// DOG
	std::vector<float> filevertices2, filenormals2, filetextures2;
	Readfile3("DOG.obj", filevertices2, filenormals2, filetextures2);
	for(auto i = 0; i < filevertices2.size(); i++)
		filevertices2[i] -= 2.0f;
	glGenVertexArrays(1, &(vertexBuffers[2]));
	glBindVertexArray(vertexBuffers[2]);
	glGenBuffers(1, &(arrayBuffers[2]));
	glBindBuffer(GL_ARRAY_BUFFER, arrayBuffers[2]);
	glBufferData(GL_ARRAY_BUFFER, filevertices2.size() * sizeof(float) + (filevertices2.size() / 3) * 4 * sizeof(float), NULL, GL_STATIC_DRAW);
	// offset in bytes   size in bytes     ptr to data    
	glBufferSubData(GL_ARRAY_BUFFER, 0, filevertices2.size() * sizeof(float), filevertices2.data());
	// Create a color array since colors don't come in...
	auto filecolors2 = new float[filevertices2.size() / 3 * 4];
	for(unsigned int currentVertex = 0; currentVertex < filevertices2.size() / 3; currentVertex++)
	{
		filecolors2[currentVertex * 4] = 0.0f;
		filecolors2[currentVertex * 4 + 1] = 1.0f;
		filecolors2[currentVertex * 4 + 2] = 0.0f;
		filecolors2[currentVertex * 4 + 3] = 1.0f;
	}
	glBufferSubData(GL_ARRAY_BUFFER, filevertices2.size() * sizeof(float), filevertices2.size() / 3 * 4 * sizeof(float), filecolors2);
	vPosition = glGetAttribLocation(programID, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	vColor = glGetAttribLocation(programID, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(filevertices2.size() * sizeof(float)));

	objectList.push_back(new PositionInfo(vertexBuffers[2], arrayBuffers[2], filevertices2.size() / 3, locationMap["modelingTransform"], vmath::scale(1.0f, 1.0f, 1.0f), vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f)));



	// Trout
	std::vector<float> filevertices3, filenormals3, filetextures3;
	Readfile3("trout.obj", filevertices3, filenormals3, filetextures3);
	for(auto i = 0; i < filevertices3.size(); i++)
		filevertices3[i] += 2.0f;
	glGenVertexArrays(1, &(vertexBuffers[3]));
	glBindVertexArray(vertexBuffers[3]);
	glGenBuffers(1, &(arrayBuffers[3]));
	glBindBuffer(GL_ARRAY_BUFFER, arrayBuffers[3]);
	glBufferData(GL_ARRAY_BUFFER, filevertices3.size() * sizeof(float) + (filevertices3.size() / 3) * 4 * sizeof(float), NULL, GL_STATIC_DRAW);
	// offset in bytes   size in bytes     ptr to data    
	glBufferSubData(GL_ARRAY_BUFFER, 0, filevertices3.size() * sizeof(float), filevertices3.data());
	// Create a color array since colors don't come in...
	auto filecolors3 = new float[filevertices3.size() / 3 * 4];
	for(unsigned int currentVertex = 0; currentVertex < filevertices3.size() / 3; currentVertex++)
	{
		filecolors3[currentVertex * 4] = 0.0f;
		filecolors3[currentVertex * 4 + 1] = 0.0f;
		filecolors3[currentVertex * 4 + 2] = 1.0f;
		filecolors3[currentVertex * 4 + 3] = 1.0f;
	}
	glBufferSubData(GL_ARRAY_BUFFER, filevertices3.size() * sizeof(float), filevertices3.size() / 3 * 4 * sizeof(float), filecolors3);
	vPosition = glGetAttribLocation(programID, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	vColor = glGetAttribLocation(programID, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(filevertices3.size() * sizeof(float)));

	objectList.push_back(new PositionInfo(vertexBuffers[3], arrayBuffers[3], filevertices3.size() / 3, locationMap["modelingTransform"], vmath::scale(1.0f, 1.0f, 1.0f), vmath::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
}

/*
 * This fills in the locations of most of the uniform variables for the program.
 * there are better ways of handling this but this is good in going directly from
 * what we had.
 *
 * Revised to get the locations and names of the uniforms from OpenGL.  These
 * are then stored in a map so that we can look up a uniform by name when we
 * need to use it.  The map is still global but it is a little neater than the
 * version that used all the locations.  The locations are still there right now
 * in case that is more useful for you.
 *
 */
void GetLocations()
{
	/*
	 * Find out how many uniforms there are and go out there and get them from the
	 * shader program.  The locations for each uniform are stored in a global -- locationMap --
	 * for later retrieval.
	 */
	GLint numberBlocks;
	char uniformName[1024];
	int nameLength;
	GLint size;
	GLenum type;
	glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &numberBlocks);
	for(int blockIndex = 0; blockIndex < numberBlocks; blockIndex++)
	{
		glGetActiveUniform(programID, blockIndex, 1024, &nameLength, &size, &type, uniformName);
		std::cout << uniformName << std::endl;
		locationMap[std::string(uniformName)] = blockIndex;
	}
}

void Init(std::string vertexShader, std::string fragmentShader)
{
	SetAttributes(1.0f, GL_FRONT_AND_BACK, GL_FILL);
	programID = BuildProgram(vertexShader, fragmentShader);
	BuildObjects();
	GetLocations();
}

/*
* Timer routine -- when called it increments the angle by 1 degree,
* tells Glut to redraw the screen, and sets a timer for 1/30 of a
* second later.
*/
void timer(int value)
{
	if(rotationOn)
	{
		rotationAngle += 1.0f;
		if(rotationAngle >= 360.0)
			rotationAngle -= 360.0;
		glutPostRedisplay();
	}
	glutTimerFunc(1000 / 30, timer, 1);
}

/*
 * The display routine is basically unchanged at this point.
 */
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// needed
	auto currentMatrix = vmath::rotate(rotationAngle, 0.0f, 1.0f, 0.0f);

	/*
	 *  Draw everything in the objectList.  The actual objects are set up in the buildObjects function.
	 */
	for(auto d : objectList)
	{
		d->draw();
	}
	glutSwapBuffers();  // if double buffered
}

/*
* Handle keypresses -- only one recognized in this version is q for quit.
*/
void keypress(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'q':
		case 'Q':
			exit(0);
			break;
		case 'p':
		case 'P':
			glUniformMatrix4fv(locationMap["projectionTransform"], 1, GL_FALSE, vmath::perspective(60.0f, 1.0f, 1.0f, 100.0f));
			break;
		case 'o':
		case 'O':
			glUniformMatrix4fv(locationMap["projectionTransform"], 1, GL_FALSE, vmath::ortho(-6.0f, 6.0f, -6.0f, 6.0f, -100.0f, 100.0f));
			break;
		case 'r':
		case 'R':
			rotationOn = !rotationOn;
			break;
		case 'x':
		case 'X':
			glUniformMatrix4fv(locationMap["viewingTransform"], 1, GL_FALSE, vmath::lookat(vmath::vec3(8.0f, 0.0f, 0.0f), vmath::vec3(0.0f, 0.0f, 0.0f), vmath::vec3(0.0f, 1.0f, 0.0f)));
			break;
		case 'y':
		case 'Y':
			glUniformMatrix4fv(locationMap["viewingTransform"], 1, GL_FALSE, vmath::lookat(vmath::vec3(0.0f, 8.0f, 0.0f), vmath::vec3(0.0f, 0.0f, 0.0f), vmath::vec3(0.0f, 0.0f, 1.0f)));
			break;
		case 'z':
		case 'Z':
			glUniformMatrix4fv(locationMap["viewingTransform"], 1, GL_FALSE, vmath::lookat(vmath::vec3(0.0f, 0.0f, 8.0f), vmath::vec3(0.0f, 0.0f, 0.0f), vmath::vec3(0.0f, 1.0f, 0.0f)));
			break;
		default:
			// do nothing....
			break;
	}
	glutPostRedisplay();
}
