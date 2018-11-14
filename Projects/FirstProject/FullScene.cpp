///////////////////////////////////////////////////////////////////////
//
// Using quaternions to rotate in 3D.
//
// Assignment: 1. Create a class for Quaternions.
//             2. Create a scene with a camera rotating around an 
//                object at the origin and pointing towards it. 
//                Do NOT use "gluLookAt" to create the ViewMatrix, 
//                use rotation matrices!
//             3. Gimbal lock mode ON: use Translation + Rotation 
//                matrices (e.g. Euler angles, Rodrigues’ formula).
//             4. Gimbal lock mode OFF: use Quaternions to produce a 
//                transformation matrix and avoid gimbal lock.
//             5. Switch between modes by pressing the 'g' key.
//
// (c) 2013-18 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <string>

#include "vector.h"
#include "matrix.h"
#include "ShaderProgram.h"
#include "camera.h"
#include "KeyBuffer.h"
#include "Mesh.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

#define CAPTION "Hello Modern 3D World"

using namespace engine;

int WinX = 640, WinY = 640;
int WindowHandle = 0;
unsigned int FrameCount = 0;


const GLuint UBO_BP = 0;
const float PI = 3.14159265f;
const GLsizeiptr matrixSize = sizeof(GLfloat[16]);

GLuint VboId[1]; // shared matrice
FixedCamera* camera;
ShaderProgram* dfault;
ShaderProgram* prog;
Mesh* mesh;



float lastFrame = 0.0f;
float delta = 0.0f;
int lastMouseY = WinX / 2;
int lastMouseX = WinY / 2;
float k = 0.0f;

mat4 projectionMatrix = MatrixFactory::createPerspectiveProjectionMatrix(30, (float)WinX / (float)WinY, 1, 10);
mat4 otherProjectionMatrix = MatrixFactory::createOrtographicProjectionMatrix(-2, 2, -2, 2, 1, 10);

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


void mouse_wheel_input(int button, int dir, int x, int y) {
	camera->zoom(dir, delta);
}
void mouse_input(int x, int y) {

	int xoffset = lastMouseX - x;
	int yoffset = lastMouseY - y;
	lastMouseX = x;
	lastMouseY = y;
	float sen = 0.25f;
	camera->cameraLookAround(xoffset * sen, yoffset * sen, delta);

}

void keyPress(unsigned char key, int x, int y) {
	KeyBuffer::instance()->pressKey(key);
	if (KeyBuffer::instance()->isKeyDown('g') || KeyBuffer::instance()->isKeyDown('G')) camera->gimbalLockSwitch();
	if (KeyBuffer::instance()->isKeyDown('p') || KeyBuffer::instance()->isKeyDown('P')) {
		mat4 temp = projectionMatrix;
		projectionMatrix = otherProjectionMatrix;
		otherProjectionMatrix = temp;
	}

}

void keyRelease(unsigned char key, int x, int y) {
	KeyBuffer::instance()->releaseKey(key);
}

void update() {
	if (KeyBuffer::instance()->isKeyDown('a')) camera->cameraMoveLeft(delta);
	if (KeyBuffer::instance()->isKeyDown('A')) camera->cameraMoveLeft(delta);

	if (KeyBuffer::instance()->isKeyDown('d')) camera->cameraMoveRight(delta);
	if (KeyBuffer::instance()->isKeyDown('D')) camera->cameraMoveRight(delta);

	if (KeyBuffer::instance()->isKeyDown('s')) camera->cameraMoveBack(delta);
	if (KeyBuffer::instance()->isKeyDown('S')) camera->cameraMoveBack(delta);

	if (KeyBuffer::instance()->isKeyDown('w')) camera->cameraMoveForward(delta);
	if (KeyBuffer::instance()->isKeyDown('W')) camera->cameraMoveForward(delta);

	if (KeyBuffer::instance()->isKeyDown('q')) camera->cameraRollLeft(delta);
	if (KeyBuffer::instance()->isKeyDown('Q')) camera->cameraRollLeft(delta);

	if (KeyBuffer::instance()->isKeyDown('e')) camera->cameraRollRight(delta);
	if (KeyBuffer::instance()->isKeyDown('E')) camera->cameraRollRight(delta);

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


/////////////////////////////////////////////////////////////////////// SHADERs

void createShaderProgram()
{
	prog = new ShaderProgram();
	prog->attachShader(GL_VERTEX_SHADER,"vertex","cube_vs_shared.glsl");
	prog->attachShader(GL_FRAGMENT_SHADER, "fragment","cube_fs_extra.glsl");

	prog->bindAttribLocation(VERTICES, "inPosition");
	if (mesh->TexcoordsLoaded)
		prog->bindAttribLocation(TEXCOORDS, "inTexcoord");
	if (mesh->NormalsLoaded)
		prog->bindAttribLocation(NORMALS, "inNormal");
	prog->link();

	prog->uniformBlockBinding(prog->uniformBlockIndex("SharedMatrices"), UBO_BP);

	prog->detachShader("vertex");
	prog->detachShader("fragment");


	checkOpenGLError("ERROR: Could not create shaders.");

}
void destroyShaderProgram()
{
	glUseProgram(0);
	glDeleteProgram(prog->id);

	checkOpenGLError("ERROR: Could not destroy shaders.");
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

void createBufferObjects()
{
	mesh->createBufferObjects();

	glGenBuffers(1, VboId);

	glBindBuffer(GL_UNIFORM_BUFFER, VboId[0]);
	{
		glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat[16]) * 2, 0, GL_STREAM_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, VboId[0]);
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	checkOpenGLError("ERROR: Could not create VAOs, VBOs and UBOs.");
}

void destroyBufferObjects()
{
	mesh->destroyBufferObjects();
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

/////////////////////////////////////////////////////////////////////// SCENE

void drawScene()
{

	mat4 ViewMatrix = camera->ViewMatrix();

	glBindBuffer(GL_UNIFORM_BUFFER, VboId[0]);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, matrixSize, ViewMatrix.data());
	glBufferSubData(GL_UNIFORM_BUFFER, matrixSize, matrixSize, projectionMatrix.data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	// uniforms
	glUseProgram(prog->id);
	glUniform1f(prog->UniformLocation("k"), k);
	glUseProgram(0);

	mesh->draw(MatrixFactory::createIdentityMatrix4(), prog);


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
	update();
	float currentFrame = (float)glutGet(GLUT_ELAPSED_TIME);
	delta = ((float)currentFrame - (float)lastFrame) / 100;
	lastFrame = (float)currentFrame;
	k = k + delta / 2;
	//k = fmod(k + delta / 2, 6.29f);
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
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyRelease);
	glutMotionFunc(mouse_input);
	glutMouseWheelFunc(mouse_wheel_input);
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
}

void setupGLEW()
{
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
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
	glutInitContextFlags(GLUT_DEBUG);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if (WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void setupCamera() {
	camera = new FixedCamera(vec3(0, 0, 5), vec3(0, 0, 0), vec3(0, 1, 0));

}

void createMesh() {
	//mesh = new Mesh( std::string("src/cubeT.obj"));
	//mesh = new Mesh( std::string("src/TangramPieceTriangle.obj"));
	//mesh = new Mesh( std::string("src/TangramPieceSquare.obj"));
	//mesh = new Mesh( std::string("src/TangramPieceParallelogram.obj"));
	mesh = new Mesh(std::string("src/duck.obj"));

}
void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	setupCamera();
	setupCallbacks();
	createMesh();
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
