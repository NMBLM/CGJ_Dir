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
#include "shape.h"
#include "GL/glew.h"
#include "GL/freeglut.h"

#define CAPTION "Hello Modern 2D World"

using namespace engine;

int WinX = 640, WinY = 640;
int WindowHandle = 0;
unsigned int FrameCount = 0;

#define VERTICES 0
#define COLORS 1
Shader shaders;
Triangle *triangle;
Square *square;
Parallelogram *parallelogram;

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


void createBufferObjects()
{
	// TRIANGLE START
	triangle = new Triangle();

	// TRIANGLE END


	// SQUARE START
	square = new Square();
	// SQUARE END

	// PARALLELOGRAM START
	parallelogram = new Parallelogram();
	// PARALLELOGRAM END

	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
	// Triangle
	triangle->destroyBuffers();
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs. Triangle");

	//Square
	square->destroyBuffers();

	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs. Square");

	//Parallelogram
	parallelogram->destroyBuffers();

	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs. Parallelogram");
}

const float PI = 3.14159265f;

/////////////////////////////////////////////////////////////////////// SCENE SWORD 

// Model Matrix
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

const mat4 sq78 =	MatrixFactory::createTranslationMatrix(0.0f, -0.14f -0.2f, 0.0f) * // '.14f is half the side of the square
					MatrixFactory::createRotationMatrix4(PI / 4, vec4(0, 0, 1, 1)) * //rotate 45 degrees
					MatrixFactory::createTranslationMatrix(-0.2f, 0.0f, 0.0f); // center in the origin

const mat4 tr9 =	MatrixFactory::createTranslationMatrix(0.8f * 0.707f / 2, -0.2f - 0.28f , 0.0f) * // 0.8f * 0.707f / 2  is the center of the hypotenuse to the origin
					MatrixFactory::createScaleMatrix4(0.707f, 0.707f, 0) *
					MatrixFactory::createRotationMatrix4(-PI, vec4(0, 0, 1, 1));

// COLORS
const GLfloat red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
const GLfloat green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
const GLfloat blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
const GLfloat cyan[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
const GLfloat magenta[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
const GLfloat yellow[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
const GLfloat white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat orange[4] = { 1.0f, 0.2f, 0.0f, 1.0f };
const GLfloat purple[4] = { 0.4f, 0.0f, 0.4f, 1.0f };

void drawScene()
{
	triangle->draw(tr1, red, shaders);
	triangle->draw(tr2, green, shaders);
	triangle->draw(tr3, blue, shaders);
	triangle->draw(tr6, cyan, shaders);
	triangle->draw(tr9, magenta, shaders);
	square->draw(sq78, yellow, shaders);
	parallelogram->draw(pl45, white, shaders);

	checkOpenGLError("ERROR: Could not draw scene.");

}

/////////////////////////////////////////////////////////////////////// SCENE ORIGINAL

// Model Matrix

//const mat4 mvtocnr = MatrixFactory::createTranslationMatrix(0.6f, 0.6f, 0.0f);

const mat4 mvtocnr = MatrixFactory::createIdentityMatrix4();

const mat4 otr1 =	mvtocnr *
					MatrixFactory::createTranslationMatrix(0.4f, 0.4f, 0.0f) *
					MatrixFactory::createRotationMatrix4(PI , vec4(0, 0, 1, 1));

const mat4 otr2 =	mvtocnr *
					MatrixFactory::createTranslationMatrix(-0.4f, 0.4f, 0.0f) *
					MatrixFactory::createRotationMatrix4(-PI / 2, vec4(0, 0, 1, 1));

const mat4 otr3 =	mvtocnr *
					MatrixFactory::createTranslationMatrix(0.4f, 0.0f, 0.0f) *
					MatrixFactory::createScaleMatrix4(0.707f, 0.707f, 0) *
					MatrixFactory::createRotationMatrix4(-PI/2 - PI/4, vec4(0, 0, 1, 1)); // -90 degrees - 45 degrees

const mat4 opl45 =	mvtocnr *
					MatrixFactory::createTranslationMatrix(-0.4f, -0.4f, 0.0f);

const mat4 otr6 =	mvtocnr *
					MatrixFactory::createTranslationMatrix(0.4f, 0.0f, 0.0f) *
					MatrixFactory::createScaleMatrix4(0.5f, 0.5f, 0) *
					MatrixFactory::createRotationMatrix4(PI / 2, vec4(0, 0, 1, 1));

const mat4 osq78 =	mvtocnr *
					MatrixFactory::createIdentityMatrix4();

const mat4 otr9 =	mvtocnr *
					MatrixFactory::createTranslationMatrix(-0.8f * 0.5f / 2, -0.4f * 0.5f, 0.0f) *	// 0.8f * 0.5f / 2  is the center of the hypotenuse to the origin						   
					MatrixFactory::createScaleMatrix4(0.5f, 0.5f, 0);								// and -0.4f * 0.5f is the height of the triangle

void drawSceneOriginal()
{

	triangle->draw(otr1, red, shaders);
	triangle->draw(otr2, orange, shaders);
	triangle->draw(otr3, yellow, shaders);
	triangle->draw(otr6, green, shaders);
	triangle->draw(otr9, cyan, shaders);
	square->draw(osq78, purple, shaders);
	parallelogram->draw(opl45, blue, shaders);


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
	//drawSceneOriginal();
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
