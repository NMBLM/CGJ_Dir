///////////////////////////////////////////////////////////////////////
//
// Assignment 1 consists in the following:
//
// - Update your graphics drivers to their latest versions.
// - Download the libraries: Glew and FreeGlut for your system.
// - Create a project to compile, link and run the code provided in this 
//   section in your favourite programming environment 
//   (course will use VS2017 Community Edition).
// - Verify what OpenGL contexts your computer can support, a minimum of 
//   OpenGL 3.3 support is required for this course.
//
// Further suggestions to verify your understanding of the concepts explored:
// - Change the program so display is called at 30 FPS.
// - Create an abstract class for an OpenGL application.
//
// (c)2013-18 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <string>

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "../vector.h"
#include "../matrix.h"

/*
#define CAPTION "Hello Blank World"

int WinX = 50, WinY = 50;
int WindowHandle = 0;
unsigned int FrameCount = 0;
unsigned int TimerMillis = 1000;

/////////////////////////////////////////////////////////////////////// ERRORS

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

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
}

void display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Draw something
	glutSwapBuffers();
}

void idle()
{
	// Update your scene
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
}

void updateFPS()
{
	std::ostringstream oss;
	unsigned int FPS = FrameCount * 1000 / TimerMillis;
	oss << CAPTION << ": " << FPS << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
}

void timer(int value)
{
	updateFPS();
	glutTimerFunc(TimerMillis, timer, 0);
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks()
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
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
	glViewport(0, 0, WinX, WinY);
}

void setupGLEW()
{
	glewExperimental = GL_TRUE;
	// Allow extension entry points to be loaded even if the extension isn't 
	// present in the driver's extensions string.
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
	// You might get GL_INVALID_ENUM when loading GLEW.
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	//glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	//glutInitContextFlags(GLUT_DEBUG);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if (WindowHandle < 1) {
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
}

int main(int argc, char* argv[])
{
	
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);

}
*/
///////////////////////////////////////////////////////////////////////

int properties()
{
	
	engine::vec3 v1 = engine::vec3(1.0f, 1.0f, 1.0f);
	engine::vec3 v2 = engine::vec3(1.0f, 0.0f, 1.0f);
	engine::vec3 v3 = engine::vec3(1.0f, 0.0f, -1.0f);
	engine::vec3 x  = engine::vec3(1.0f, 0.0f, 0.0f);
	engine::vec3 y  = engine::vec3(0.0f, 1.0f, 0.0f);
	engine::vec3 z  = engine::vec3(0.0f, 0.0f, 1.0f);
	engine::vec3 zl = engine::vec3(0.0f);
	engine::vec3 zz = engine::vec3(0.0f);

	std::cout << "v1 =  " << v1 << std::endl;
	std::cout << "v2 =  " << v2 << std::endl;
	std::cout << "v3 =  " << v3 << std::endl;
	std::cout << "v1 * 2 =  " << v1 * 2 << std::endl;
	std::cout << "2 * v1 =  " << 2 * v1 << std::endl;
	std::cout << "v2 * v3 =  " << v2.dot(v3) << std::endl;
	zl = x.cross( y);
	std::cout << "zl =  " << zl << std::endl;
	std::cout << "z =  " << z << std::endl;
	if (zl == z) {
		std::cout << "True"<< std::endl;
	}
	else {
		std::cout << "False" << std::endl;
	}
	zz = y.cross(x);
	std::cout << "zz =  " << zz << std::endl;
	std::cout << "z =  " << z << std::endl;
	if (zz == z) {
		std::cout << "True" << std::endl;
	}
	else {
		std::cout << "False" << std::endl;
	}
	v1 += v2;
	std::cout << "v1 += v2  " << v1 << std::endl;
	std::cout << "Identity and dual " << std::endl;
	engine::mat3 m1 = engine::MatrixFactory::createIdentityMatrix3();
	std::cout << m1;
	engine::mat3 m2 = engine::MatrixFactory::createDualMatrix(v1);
	std::cout << m2;
	std::cout << m2.mat[2] << std::endl;
	std::cout << m2.data()[2] << std::endl;
	engine::mat3 m3 = engine::mat3(2, 0, 0, 0, 2, 0, 0, 0, 2);
	std::cout << "inv 3 " << std::endl;
	std::cout <<  m3.inverse();
	std::cout << m3 * m3.inverse();
	std::cout << m3.inverse() * m3;
	engine::mat4 mm = engine::MatrixFactory::createIdentityMatrix4() * engine::MatrixFactory::createIdentityMatrix4();
	std::cout << mm;
	std::cout << "trans " << std::endl;

	engine::mat4 t = engine::MatrixFactory::createTranslationMatrix(2, 4, 6);
	std::cout << t;
	std::cout << "normal " << std::endl << engine::MatrixFactory::createNormalMatrix(t);
	std::cout << "rot " << std::endl;
	engine::mat3 rot = engine::MatrixFactory::createRotationMatrix3(3.14159265f / 2.0f, engine::vec3(1,0,0));
	std::cout << rot;
	std::cout << rot * x << std::endl;
	std::cout << rot * y << std::endl;
	std::cout << rot * z << std::endl;

	engine::mat2 m4 = engine::mat2(2, 1, 4, 4);
	std::cout << "2 det " << std::endl;
	std::cout << m4.determinant() << std::endl;

	std::cout << "inv 2 " << std::endl;
	std::cout << m4.inverse() << std::endl;
	std::cout << m4 * m4.inverse() << std::endl;
	std::cout << m4.inverse() * m4 << std::endl;
}