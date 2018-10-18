///////////////////////////////////////////////////////////////////////
//
// Drawing two instances of a triangle in Modern OpenGL.
// A "hello world" of Modern OpenGL.
//
// Assignment : Create Shader Abstraction 
//					(e.g. check compilation/linkage for errors, read from file) 
//			  : Manage Multiple Drawable Entities (using your vertex and matrix classes)
//              Draw a set of 7 TANs (i.e. TANGRAM shapes) of different colors: 
//              (1) 3 different TAN geometric shapes at the origin:
//					- right triangle
//					- square
//					- parallelogram
//              (2) 7 TANs of different colors put together to form a shape of your choice:
//					- 2 big right triangles
//					- 1 medium right triangle
//					- 2 small right triangles
//					- 1 square
//					- 1 parallelogram;
//
// (c) 2013-18 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <string>


#include "vector.h"
#include "matrix.h"
#include "Shader.h"
#include "GL/glew.h"
#include "GL/freeglut.h"

#define CAPTION "Hello Modern 2D World"

using namespace engine;

int WinX = 400, WinY = 400;
int WindowHandle = 0;
unsigned int FrameCount = 0;

#define VERTICES 0
#define COLORS 1
//VAO = Vertex 
GLuint VaoId, VboId[2];		// Triagle buffers
GLuint SVaoId,SVboId[2];	// Square buffers
GLuint PVaoId, PVboId[2];	// Parallelogram buffers
Shader shaders;

/////////////////////////////////////////////////////////////////////// ERRORS

static std::string errorType(GLenum type)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:				return "error";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	return "deprecated behavior";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "undefined behavior";
	case GL_DEBUG_TYPE_PORTABILITY:			return "portability issue";
	case GL_DEBUG_TYPE_PERFORMANCE:			return "performance issue";
	case GL_DEBUG_TYPE_MARKER:				return "stream annotation";
	case GL_DEBUG_TYPE_OTHER_ARB:			return "other";
	default:								exit(EXIT_FAILURE);
	}
}

static std::string errorSource(GLenum source)
{
	switch (source) {
	case GL_DEBUG_SOURCE_API:				return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "window system";
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "shader compiler";
	case GL_DEBUG_SOURCE_THIRD_PARTY:		return "third party";
	case GL_DEBUG_SOURCE_APPLICATION:		return "application";
	case GL_DEBUG_SOURCE_OTHER:				return "other";
	default:								exit(EXIT_FAILURE);
	}
}

static std::string errorSeverity(GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:			return "high";
	case GL_DEBUG_SEVERITY_MEDIUM:			return "medium";
	case GL_DEBUG_SEVERITY_LOW:				return "low";
	case GL_DEBUG_SEVERITY_NOTIFICATION:	return "notification";
	default:								exit(EXIT_FAILURE);
	}
}

static void error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar *message, const void *userParam)
{
	std::cerr << "ERROR:" << std::endl;
	std::cerr << "  source:     " << errorSource(source) << std::endl;
	std::cerr << "  type:       " << errorType(type) << std::endl;
	std::cerr << "  severity:   " << errorSeverity(severity) << std::endl;
	std::cerr << "  debug call: " << std::endl << message << std::endl << std::endl;
}

void setupErrors()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(error, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
	// params: source, type, severity, count, ids, enabled
}

static bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

static void checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

/////////////////////////////////////////////////////////////////////// SHADERS


void createShaderProgram()
{
	shaders = Shader("VertexShader.glsl" , "FragmentShader.glsl");

	checkOpenGLError("ERROR: Could not create shaders.");
}
void destroyShaderProgram()
{
	glUseProgram(0);
	glDeleteProgram(shaders.ProgramId);

	checkOpenGLError("ERROR: Could not destroy shaders.");
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

typedef struct 
{
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

const Vertex Vertices[] = 
{
	{{ 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
	{{ 0.8f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
	{{ 0.4f, 0.4f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }}
};

const GLubyte Indices[] =
{
	0,1,2
};

const Vertex SquareVertices[] =
{
	{{  0.0f,  0.0f,  0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }}, //middle left
	{{  0.2f,  -0.2f,  0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }}, //bottom middle
	{{  0.4f,  0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }}, //middle right
	{{  0.2f,  0.2f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }}, //top middle

};

const GLubyte SquareIndices[] =
{
	0,1,2,2,3,0
};


const Vertex ParallelogramVertices[] =
{
	{{  0.0f,  0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }}, //bottom left
	{{  0.4f,  0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }}, //bottom right
	{{  0.6f,  0.2f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }}, //top right
	{{  0.2f,  0.2f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }}, //top left
};

const GLubyte ParallelogramIndices[] =
{
	0,1,3,1,2,3
};


void createBufferObjects()
{
	// TRIANGLE START
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	{
		glGenBuffers(2, VboId);

		glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(VERTICES);
			glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glEnableVertexAttribArray(COLORS);
			glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertices[0].XYZW));
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// TRIANGLE END

	// SQUARE START
	glGenVertexArrays(1, &SVaoId);
	glBindVertexArray(SVaoId);
	{
		glGenBuffers(2, SVboId);

		glBindBuffer(GL_ARRAY_BUFFER, SVboId[0]);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(SquareVertices), SquareVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(VERTICES);
			glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glEnableVertexAttribArray(COLORS);
			glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(SquareVertices[0].XYZW));
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SVboId[1]);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SquareIndices), SquareIndices, GL_STATIC_DRAW);
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// SQUARE END

	// PARALLELOGRAM START
	glGenVertexArrays(1, &PVaoId);
	glBindVertexArray(PVaoId);
	{
		glGenBuffers(2, PVboId);

		glBindBuffer(GL_ARRAY_BUFFER, PVboId[0]);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(ParallelogramVertices), ParallelogramVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(VERTICES);
			glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
			glEnableVertexAttribArray(COLORS);
			glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(ParallelogramVertices[0].XYZW));
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, PVboId[1]);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ParallelogramIndices), ParallelogramIndices, GL_STATIC_DRAW);
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	// PARALLELOGRAM END

	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
	// Triangle
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glDeleteBuffers(2, VboId);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs. Triangle");

	//Square
	glBindVertexArray(SVaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glDeleteBuffers(2, SVboId);
	glDeleteVertexArrays(1, &SVaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs. Square");

	//Parallelogram
	glBindVertexArray(PVaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glDeleteBuffers(2,PVboId);
	glDeleteVertexArrays(1, &PVaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs. Parallelogram");
}

/////////////////////////////////////////////////////////////////////// SCENE

const float PI = 3.14159265f;

const mat4 tr1 =	MatrixFactory::createTranslationMatrix(-0.2f, 0.8f, 0.0f) * 
					MatrixFactory::createRotationMatrix4(-PI / 2,  vec4(0, 0, 1, 1));

const mat4 tr2 =	MatrixFactory::createTranslationMatrix(-0.4f, -0.2f, 0.0f);

const mat4 tr3 =	MatrixFactory::createTranslationMatrix(0.2f, 0.0f, 0.0f) *  
					MatrixFactory::createScaleMatrix4(0.5f, 0.5f, 0) *  
					MatrixFactory::createRotationMatrix4(PI / 2,  vec4(0, 0, 1, 1));

const mat4 pl45 =	MatrixFactory::createTranslationMatrix(0.2f, 0.4f, 0.0f) * 
					MatrixFactory::createRotationMatrix4(PI / 2, vec4(0, 0, 1, 1));;

const mat4 tr6 =	MatrixFactory::createTranslationMatrix(0.0f, 0.6f, 0.0f) *  
					MatrixFactory::createScaleMatrix4(0.5f, 0.5f, 0) *  
					MatrixFactory::createRotationMatrix4(PI / 2,  vec4(0, 0, 1, 1));

const mat4 sq78 =	MatrixFactory::createTranslationMatrix(0.f, -0.4f, 0.0f);

const mat4 tr9 =	MatrixFactory::createTranslationMatrix(0.3f, -0.6f, 0.0f) *  
					MatrixFactory::createScaleMatrix4(0.75f, 0.75f, 0) *  
					MatrixFactory::createRotationMatrix4(-PI, vec4(0, 0, 1, 1));

const GLfloat red[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
const GLfloat green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
const GLfloat blue[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
const GLfloat cyan[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
const GLfloat magenta[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
const GLfloat yellow[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
const GLfloat white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

void drawScene()
{

	// TRIANGLES DRAW START
	glBindVertexArray(VaoId);
	glUseProgram(shaders.ProgramId);

	// Triangle 1
	glUniform4fv(shaders.UniformColorId, 1, red);
	glUniformMatrix4fv(shaders.UniformId, 1, GL_FALSE, tr1.data());
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

	// Triangle 2
	glUniform4fv(shaders.UniformColorId, 1, green);
	glUniformMatrix4fv(shaders.UniformId, 1, GL_FALSE, tr2.data());
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

	// Triangle 3
	glUniform4fv(shaders.UniformColorId, 1, blue);
	glUniformMatrix4fv(shaders.UniformId, 1, GL_FALSE, tr3.data());
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

	// Triangle 6
	glUniform4fv(shaders.UniformColorId, 1, cyan);
	glUniformMatrix4fv(shaders.UniformId, 1, GL_FALSE, tr6.data());
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

	// Triangle 9
	glUniform4fv(shaders.UniformColorId, 1, magenta);
	glUniformMatrix4fv(shaders.UniformId, 1, GL_FALSE, tr9.data());
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glUseProgram(0);
	glBindVertexArray(0);

	// TRIANGLES DRAW END 


	// SQUARE DRAW START
	glBindVertexArray(SVaoId);
	glUseProgram(shaders.ProgramId);

	glUniform4fv(shaders.UniformColorId, 1, yellow);
	glUniformMatrix4fv(shaders.UniformId, 1, GL_FALSE, sq78.data());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glUseProgram(0);
	glBindVertexArray(0);

	// SQUARE DRAW END 

	// PARALLELOGRAM DRAW START
	glBindVertexArray(PVaoId);
	glUseProgram(shaders.ProgramId);

	glUniform4fv(shaders.UniformColorId, 1, white);
	glUniformMatrix4fv(shaders.UniformId, 1, GL_FALSE, pl45.data());
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glUseProgram(0);
	glBindVertexArray(0);

	// PARALLELOGRAM DRAW END 

	checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
	destroyShaderProgram();
	destroyBufferObjects();
}

void display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene();
	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
}

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
    FrameCount = 0;
    glutTimerFunc(1000, timer, 0);
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks() 
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0,timer,0);
	setupErrors();
}

void checkOpenGLInfo()
{
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}

void setupOpenGL()
{
	checkOpenGLInfo();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void setupGLEW() 
{
	glewExperimental = GL_TRUE;
	GLenum result = glewInit() ; 
	if (result != GLEW_OK) { 
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	} 
	GLenum err_code = glGetError();
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);
	
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	//glutInitContextFlags(GLUT_DEBUG);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	
	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if(WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{

	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	setupCallbacks();
	createShaderProgram();
	createBufferObjects();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////
