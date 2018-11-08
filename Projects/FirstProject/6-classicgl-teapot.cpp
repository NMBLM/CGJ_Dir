/////////////////////////////////////////////////////////////////////////
////
//// Example:
//// Classic OpenGL Programming: drawing a Teapot using freeglut.
//// Compatibility mode.
////
//// (c)2013-2018 by Carlos Martinho
////
/////////////////////////////////////////////////////////////////////////
//
//#include <iostream>
//#include <sstream>
//#include <string>
//
//#include "GL/glew.h"
//#include "GL/freeglut.h"
//
//#define CAPTION "Hello Old World"
//
//int WinX = 640, WinY = 480;
//int WindowHandle = 0;
//unsigned int FrameCount = 0;
//
///////////////////////////////////////////////////////////////////////// ERRORS
//
//static std::string errorType(GLenum type)
//{
//	switch (type) {
//	case GL_DEBUG_TYPE_ERROR:				return "error";
//	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	return "deprecated behavior";
//	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "undefined behavior";
//	case GL_DEBUG_TYPE_PORTABILITY:			return "portability issue";
//	case GL_DEBUG_TYPE_PERFORMANCE:			return "performance issue";
//	case GL_DEBUG_TYPE_MARKER:				return "stream annotation";
//	case GL_DEBUG_TYPE_OTHER_ARB:			return "other";
//	default:								exit(EXIT_FAILURE);
//	}
//}
//
//static std::string errorSource(GLenum source)
//{
//	switch (source) {
//	case GL_DEBUG_SOURCE_API:				return "API";
//	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "window system";
//	case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "shader compiler";
//	case GL_DEBUG_SOURCE_THIRD_PARTY:		return "third party";
//	case GL_DEBUG_SOURCE_APPLICATION:		return "application";
//	case GL_DEBUG_SOURCE_OTHER:				return "other";
//	default:								exit(EXIT_FAILURE);
//	}
//}
//
//static std::string errorSeverity(GLenum severity)
//{
//	switch (severity) {
//	case GL_DEBUG_SEVERITY_HIGH:			return "high";
//	case GL_DEBUG_SEVERITY_MEDIUM:			return "medium";
//	case GL_DEBUG_SEVERITY_LOW:				return "low";
//	case GL_DEBUG_SEVERITY_NOTIFICATION:	return "notification";
//	default:								exit(EXIT_FAILURE);
//	}
//}
//
//static void error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
//	const GLchar *message, const void *userParam)
//{
//	std::cerr << "ERROR:" << std::endl;
//	std::cerr << "  source:     " << errorSource(source) << std::endl;
//	std::cerr << "  type:       " << errorType(type) << std::endl;
//	std::cerr << "  severity:   " << errorSeverity(severity) << std::endl;
//	std::cerr << "  debug call: " << std::endl << message << std::endl << std::endl;
//}
//
//void setupErrors()
//{
//	glEnable(GL_DEBUG_OUTPUT);
//	glDebugMessageCallback(error, 0);
//	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
//	// params: source, type, severity, count, ids, enabled
//}
//
//static bool isOpenGLError() {
//	bool isError = false;
//	GLenum errCode;
//	const GLubyte *errString;
//	while ((errCode = glGetError()) != GL_NO_ERROR) {
//		isError = true;
//		errString = gluErrorString(errCode);
//		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
//	}
//	return isError;
//}
//
//static void checkOpenGLError(std::string error)
//{
//	if (isOpenGLError()) {
//		std::cerr << error << std::endl;
//		exit(EXIT_FAILURE);
//	}
//}
//
//
///////////////////////////////////////////////////////////////////////// CALLBACKS
//
//void cleanup()
//{
//}
//
//void setViewProjectionMatrix()
//{
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(60.0, (GLdouble)WinX/(GLdouble)WinY, 1.0, 5.0);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	gluLookAt(2.0f,0.75f,2.0f, 0.0f,0.75f,0.0f, 0.0f,1.0f,0.0f);
//}
//
//void drawAxis()
//{
//	glLineWidth(5.0f); // deprecated
//	glPushMatrix();
//		glTranslatef(0.0f,0.0f,0.0f);
//		glScalef(1.0f,1.0f,1.0f);
//		glColor3f(0.9f,0.0f,0.0f);
//		glBegin(GL_LINES);
//			glVertex3f(0.0f,0.0f,0.0f);
//			glVertex3f(1.0f,0.0f,0.0f);
//		glEnd();
//		glColor3f(0.0f,0.9f,0.0f);
//		glBegin(GL_LINES);
//			glVertex3f(0.0f,0.0f,0.0f);
//			glVertex3f(0.0f,1.0f,0.0f);
//		glEnd();
//		glColor3f(0.0f,0.0f,0.9f);
//		glBegin(GL_LINES);
//			glVertex3f(0.0f,0.0f,0.0f);
//			glVertex3f(0.0f,0.0f,1.0f);
//		glEnd();
//	glPopMatrix();
//	glLineWidth(1.0f);
//}
//
//void drawTeapot()
//{
//	glColor3f(0.9f,0.9f,0.9f);
//	glPushMatrix();
//		glTranslatef(0.0f, 0.75f, 0.0f);
//		glutWireTeapot(1.0);
//	glPopMatrix();
//}
//
//void display()
//{
//	++FrameCount;
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	setViewProjectionMatrix();
//	drawAxis();
//	drawTeapot();
//	glutSwapBuffers();
//
//	checkOpenGLError("ERROR: Could not draw scene.");
//}
//
//void idle()
//{
//	glutPostRedisplay();
//}
//
//void reshape(int w, int h)
//{
//	WinX = w;
//	WinY = h;
//	glViewport(0, 0, WinX, WinY);
//}
//
//void timer(int value)
//{
//	std::ostringstream oss;
//	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
//	std::string s = oss.str();
//	glutSetWindowTitle(s.c_str());
//    FrameCount = 0;
//    glutTimerFunc(1000, timer, 0);
//}
//
///////////////////////////////////////////////////////////////////////// SETUP
//
//void setupCallbacks() 
//{
//	glutCloseFunc(cleanup);
//	glutDisplayFunc(display);
//	glutIdleFunc(idle);
//	glutReshapeFunc(reshape);
//	glutTimerFunc(0,timer,0);
//	setupErrors();
//}
//
//void checkOpenGLInfo()
//{
//	const GLubyte *renderer = glGetString(GL_RENDERER);
//	const GLubyte *vendor = glGetString(GL_VENDOR);
//	const GLubyte *version = glGetString(GL_VERSION);
//	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
//	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
//	std::cerr << "OpenGL version " << version << std::endl;
//	std::cerr << "GLSL version " << glslVersion << std::endl;
//}
//
//void setupOpenGL()
//{
//	checkOpenGLInfo();
//	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LEQUAL);
//	glDepthMask(GL_TRUE);
//	glDepthRange(0.0, 1.0);
//	glClearDepth(1.0);
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//	glFrontFace(GL_CCW);
//}
//
//void setupGLEW() {
//	glewExperimental = GL_TRUE;
//	GLenum result = glewInit() ; 
//	if (result != GLEW_OK) { 
//		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
//		exit(EXIT_FAILURE);
//	} 
//	GLenum err_code = glGetError();
//}
//
//void setupGLUT(int argc, char* argv[])
//{
//	glutInit(&argc, argv);
//	
//	glutInitContextVersion(3, 3);
//	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
//	//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
//	//glutInitContextProfile(GLUT_CORE_PROFILE);
//
//	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
//	
//	glutInitWindowSize(WinX, WinY);
//	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
//	WindowHandle = glutCreateWindow(CAPTION);
//	if(WindowHandle < 1) {
//		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
//		exit(EXIT_FAILURE);
//	}
//}
//
//void init(int argc, char* argv[])
//{
//	setupGLUT(argc, argv);
//	setupGLEW();
//	setupOpenGL();
//	setupCallbacks();
//}
//
//int main(int argc, char* argv[])
//{
//	init(argc, argv);
//	glutMainLoop();	
//	exit(EXIT_SUCCESS);
//}
//
/////////////////////////////////////////////////////////////////////////