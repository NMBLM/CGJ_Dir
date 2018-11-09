/////////////////////////////////////////////////////////////////////////
////
////  Loading OBJ mesh from external file
////
////	Final individual assignment:
////	1.	Create classes: Scene, Camera and Mesh (that loads a mesh from
////		a Wavefront OBJ file to an indexed format) and build a small
////		scenegraph of your tangram scene (you may create more 
////		classes if needed).
////	2.	Create a ground object and couple the tangram figure to the
////		ground. Press keys to move the ground about: the tangram
////		figure must follow the ground.
////	3.	Animate between closed puzzle (initial square) and tangram
////		figure by pressing a key.
////	4.	Spherical camera interaction through mouse. It should be
////		possible to interact while animation is playing.
////
////	Team assignment:
////	Pick your team (2 elements from one same lab) for the team
////	assignment you will be working until the end of the semester,
////	and register it online.
////
//// (c) 2013-18 by Carlos Martinho
////
/////////////////////////////////////////////////////////////////////////
//
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <string>
//
//#include <vector>
//#include "program.h"
//#include "camera.h"
//#include "Shader.h"
//#include "Mesh.h"
//#include "KeyBuffer.h"
//
//#include "GL/glew.h"
//#include "GL/freeglut.h"
//
//#define CAPTION "Loading World"
//#define VERTICES 0
//#define TEXCOORDS 1
//#define NORMALS 2
//
//using namespace engine;
//int WinX = 640, WinY = 480;
//int WindowHandle = 0;
//
//unsigned int FrameCount = 0;
//const GLuint UBO_BP = 0;
//
//float lastFrame = 0.0f;
//float delta = 0.0f;
//int lastMouseY = WinX / 2;
//int lastMouseX = WinY / 2;
//float k = 0.0f;
//FixedCamera* camera;
//
//GLuint VboId[1];
//Program* prog;
//VertexShader vShader;
//FragmentShader fShader;
//Mesh* mesh;
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
//void update() {
//	if (KeyBuffer::instance()->isKeyDown('a')) camera->cameraMoveLeft(delta);
//	if (KeyBuffer::instance()->isKeyDown('A')) camera->cameraMoveLeft(delta);
//
//	if (KeyBuffer::instance()->isKeyDown('d')) camera->cameraMoveRight(delta);
//	if (KeyBuffer::instance()->isKeyDown('D')) camera->cameraMoveRight(delta);
//
//	if (KeyBuffer::instance()->isKeyDown('s')) camera->cameraMoveBack(delta);
//	if (KeyBuffer::instance()->isKeyDown('S')) camera->cameraMoveBack(delta);
//
//	if (KeyBuffer::instance()->isKeyDown('w')) camera->cameraMoveForward(delta);
//	if (KeyBuffer::instance()->isKeyDown('W')) camera->cameraMoveForward(delta);
//
//	if (KeyBuffer::instance()->isKeyDown('q')) camera->cameraRollLeft(delta);
//	if (KeyBuffer::instance()->isKeyDown('Q')) camera->cameraRollLeft(delta);
//
//	if (KeyBuffer::instance()->isKeyDown('e')) camera->cameraRollRight(delta);
//	if (KeyBuffer::instance()->isKeyDown('E')) camera->cameraRollRight(delta);
//
//}
//
//
//
///////////////////////////////////////////////////////////////////////// SHADERs
//
//void createShaderProgram()
//{
//	prog = new Program(glCreateProgram());
//	vShader = VertexShader("cube_vs.glsl");
//	fShader = FragmentShader("cube_fs.glsl");
//
//	prog->attachShader(vShader);
//	prog->attachShader(fShader);
//
//	prog->bindAttribLocation(VERTICES, "inPosition");
//	if (mesh->TexcoordsLoaded)
//		prog->bindAttribLocation(TEXCOORDS, "inTexcoord");
//	if (mesh->NormalsLoaded)
//		prog->bindAttribLocation(NORMALS, "inNormal");
//	prog->link();
//
//	prog->detachShader(vShader);
//	prog->detachShader(fShader);
//
//	checkOpenGLError("ERROR: Could not create shaders.");
//
//}
//
//void destroyShaderProgram()
//{
//	glUseProgram(0);
//	glDeleteProgram(prog->id);
//
//	checkOpenGLError("ERROR: Could not destroy shaders.");
//}
//
//
//
//void createBufferObjects()
//{
//	mesh->createBufferObjects();
//
//	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
//}
//
//void destroyBufferObjects()
//{
//	mesh->destroyBufferObjects();
//
//	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
//}
//
///////////////////////////////////////////////////////////////////////// SCENE
//
//typedef GLfloat Matrix[16];
//
//const Matrix I = {
//	1.0f,  0.0f,  0.0f,  0.0f,
//	0.0f,  1.0f,  0.0f,  0.0f,
//	0.0f,  0.0f,  1.0f,  0.0f,
//	0.0f,  0.0f,  0.0f,  1.0f
//};
//
//const Matrix ModelMatrix = {
//	1.0f,  0.0f,  0.0f,  0.0f,
//    0.0f,  1.0f,  0.0f,  0.0f,
//	0.0f,  0.0f,  1.0f,  0.0f,
//    0.0f,  0.0f,  0.0f,  1.0f
//}; // Column Major
//
//// Eye(5,5,5) Center(0,0,0) Up(0,1,0)
//const Matrix ViewMatrix1 = {
//    0.70f, -0.41f,  0.58f,  0.00f,
//	0.00f,  0.82f,  0.58f,  0.00f,
//   -0.70f, -0.41f,  0.58f,  0.00f,
//	0.00f,  0.00f, -8.70f,  1.00f
//}; // Column Major
//
//// Eye(-5,-5,-5) Center(0,0,0) Up(0,1,0)
//const Matrix ViewMatrix2 = {
//   -0.70f, -0.41f, -0.58f,  0.00f,
//	0.00f,  0.82f, -0.58f,  0.00f,
//    0.70f, -0.41f, -0.58f,  0.00f,
//	0.00f,  0.00f, -8.70f,  1.00f
//}; // Column Major
//
//// Orthographic LeftRight(-2,2) TopBottom(-2,2) NearFar(1,10)
//const Matrix ProjectionMatrix1 = {
//	0.50f,  0.00f,  0.00f,  0.00f,
//	0.00f,  0.50f,  0.00f,  0.00f,
//	0.00f,  0.00f, -0.22f,  0.00f,
//	0.00f,  0.00f, -1.22f,  1.00f
//}; // Column Major
//
//// Perspective Fovy(30) Aspect(640/480) NearZ(1) FarZ(10)
//const Matrix ProjectionMatrix2 = {
//	2.79f,  0.00f,  0.00f,  0.00f,
//	0.00f,  3.73f,  0.00f,  0.00f,
//	0.00f,  0.00f, -1.22f, -1.00f,
//	0.00f,  0.00f, -2.22f,  0.00f
//}; // Column Major
//
//void drawScene()
//{
//	glBindVertexArray(mesh->VaoId);
//	glUseProgram(prog->id);
//
//	//glUniformMatrix4fv(prog->UniformLocation("ModelMatrix"), 1, GL_FALSE, ModelMatrix);	
//	glUniformMatrix4fv(prog->UniformLocation("ViewMatrix"), 1, GL_FALSE, camera->ViewMatrix().data());
//	glUniformMatrix4fv(prog->UniformLocation("ProjectionMatrix"), 1, GL_FALSE, ProjectionMatrix2);
//	glUniform1f(prog->UniformLocation("k"), k);
//
//	mesh->draw( MatrixFactory::createIdentityMatrix4() , prog);
//
//	glUseProgram(0);
//	glBindVertexArray(0);
//
//	checkOpenGLError("ERROR: Could not draw scene.");
//}
//
///////////////////////////////////////////////////////////////////////// CALLBACKS
//
//void cleanup()
//{
//	destroyShaderProgram();
//	destroyBufferObjects();
//}
//
//void display()
//{
//	++FrameCount;
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	drawScene();
//	glutSwapBuffers();
//}
//
//void idle()
//{
//	update();
//	float currentFrame = (float)glutGet(GLUT_ELAPSED_TIME);
//	delta = ((float)currentFrame - (float)lastFrame) / 100;
//	lastFrame = (float)currentFrame;
//	k = fmod(k + delta / 2, 6.29f) ;
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
//	glutSetWindow(WindowHandle);
//	glutSetWindowTitle(s.c_str());
//    FrameCount = 0;
//    glutTimerFunc(1000, timer, 0);
//}
//
//
//void mouse_wheel_input(int button, int dir, int x, int y) {
//	camera->zoom(dir, delta);
//}
//void mouse_input(int x, int y) {
//
//	int xoffset = lastMouseX - x;
//	int yoffset = lastMouseY - y;
//	lastMouseX = x;
//	lastMouseY = y;
//	float sen = 0.25f;
//	camera->cameraLookAround(xoffset * sen, yoffset * sen, delta);
//
//}
//
//void keyPress(unsigned char key, int x, int y) {
//	KeyBuffer::instance()->pressKey(key);
//	if (KeyBuffer::instance()->isKeyDown('g') || KeyBuffer::instance()->isKeyDown('G')) camera->gimbalLockSwitch();
//
//}
//
//void keyRelease(unsigned char key, int x, int y) {
//	KeyBuffer::instance()->releaseKey(key);
//}
//
//
//
///////////////////////////////////////////////////////////////////////// SETUP
//
//void setupCallbacks() 
//{
//	glutCloseFunc(cleanup);
//	glutDisplayFunc(display);
//	glutIdleFunc(idle);
//	glutKeyboardFunc(keyPress);
//	glutKeyboardUpFunc(keyRelease);
//	glutMotionFunc(mouse_input);
//	glutMouseWheelFunc(mouse_wheel_input);
//	glutReshapeFunc(reshape);
//	glutTimerFunc(0, timer, 0);
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
//void setupGLEW() 
//{
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
//	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
//	glutInitContextProfile(GLUT_CORE_PROFILE);
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
//void setupCamera() {
//	camera = new FixedCamera(vec3(0, 0, 5), vec3(0, 0, 0), vec3(0, 1, 0));
//
//}
//void init(int argc, char* argv[])
//{
//	setupGLUT(argc, argv);
//	setupGLEW();
//	setupOpenGL();
//	setupCallbacks();
//	setupCamera();
//	//mesh = new Mesh( std::string("src/cubeT.obj"));
//	//mesh = new Mesh( std::string("src/TangramPieceTriangle.obj"));
//	//mesh = new Mesh( std::string("src/TangramPieceSquare.obj"));
//	//mesh = new Mesh( std::string("src/TangramPieceParallelogram.obj"));
//	mesh = new Mesh(std::string("src/duck.obj"));
//
//	createShaderProgram();
//	createBufferObjects();
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
