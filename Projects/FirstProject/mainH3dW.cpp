///////////////////////////////////////////////////////////////////////
//
// Assignment consists in the following:
//
// - Create the following changes to your scene:
//   - Make your TANs double-faced, so they can be seen from both sides.
//   - The new face of each TAN should share the same hue as the
//     original top face color but have a different level of saturation 
//     and brightness.
//
// - Add the following functionality:
//   - Create a View Matrix from (eye, center, up) parameters.
//   - Create an Orthographic Projection Matrix from (left-right, 
//     bottom-top, near-far) parameters.
//   - Create a Perspective Projection Matrix from (fovy, aspect,
//     nearZ, farZ) parameters.
//
// - Add the following dynamics to the application:
//   - Create a free 3D camera controlled by the mouse allowing to 
//     visualize the scene through all its angles.
//   - Change perspective from orthographic to perspective and back as
//     a response to pressing the key 'p'.
//
// (c) 2013-18 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <string>

#include "vector.h"
#include "matrix.h"
#include "program.h"
#include "shader.h"
#include "camera.h"

#include "shape.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

#define CAPTION "Hello Modern 3D World"

using namespace engine;

int WinX = 640, WinY = 640;
int WindowHandle = 0;
unsigned int FrameCount = 0;


Camera* camera;
Triangle *triangle;
Square *square;
Parallelogram *parallelogram;

GLuint VaoId, VboId[2];
Program prog;
VertexShader vShader;
FragmentShader fShader;
const GLuint UBO_BP = 0;
float lastFrame = 0.0f;
float delta = 0.0f;
int lastMouseY = WinX / 2;
int lastMouseX = WinY / 2;
// Orthographic LeftRight(-2,2) TopBottom(-2,2) NearFar(1,10)
mat4 projectionMatrix = MatrixFactory::createOrtographicProjectionMatrix(-2, 2, -2, 2, 1, 10);
// Perspective Fovy(30) Aspect(640/480) NearZ(1) FarZ(10)
mat4 otherProjectionMatrix = MatrixFactory::createPerspectiveProjectionMatrix(30, (float)WinX / (float) WinY, 1, 10);

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
void mouse_input(int x, int y) {

	int xoffset = lastMouseX - x;
	int yoffset = lastMouseY - y;
	lastMouseX = x;
	lastMouseY = y;
	float sen = 0.25f;
	camera->cameraLookAround(xoffset * sen, yoffset * sen, delta);

}
void keyboard_input(unsigned char key, int x, int y) {

	switch (key) {
	case 'a':
	case 'A':
		camera->cameraMoveLeft(delta);
		break;
	case 's':
	case 'S':
		camera->cameraMoveBack(delta);
		break;
	case 'd':
	case 'D':
		camera->cameraMoveRight(delta);
		break;
	case 'w':
	case 'W':
		camera->cameraMoveForward(delta);
		break;

	case'p':
	case'P':
		mat4 temp = projectionMatrix;
		projectionMatrix = otherProjectionMatrix;
		otherProjectionMatrix = temp;
		break;
	}

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
	prog = Program(glCreateProgram());
	vShader = VertexShader("VertexShaderCamera.glsl");
	fShader = FragmentShader("FragmentShader.glsl");

	prog.attachShader(vShader);
	prog.attachShader(fShader);

	prog.bindAttribLocation(VERTICES, "in_Position");
	prog.link();
	glUniformBlockBinding(prog.id, prog.uniformBlockIndex("SharedMatrices"), UBO_BP);

	prog.detachShader( vShader);
	prog.detachShader(fShader);

	checkOpenGLError("ERROR: Could not create shaders.");

	//vShader = VertexShader("VertexShader.glsl");
	//fShader = FragmentShader("FragmentShader.glsl");
	//prog = Program(glCreateProgram());

	//prog.attachShader(vShader);
	//prog.attachShader(fShader);

	//prog.bindAttribLocation( VERTICES, "in_Position");

	//prog.link();

	//prog.detachShader(vShader);
	//prog.detachShader(fShader);
	//checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
	glUseProgram(0);
	glDeleteProgram(prog.id);

	checkOpenGLError("ERROR: Could not destroy shaders.");
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

void createBufferObjects()
{
	triangle = new Triangle();

	square = new Square();

	parallelogram = new Parallelogram();

	checkOpenGLError("ERROR: Could not create VAOs, VBOs and UBOs.");
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

	/*glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glDeleteBuffers(2, VboId);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindVertexArray(0);*/

	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

/////////////////////////////////////////////////////////////////////// SCENE

// Model Matrix
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

const mat4 sq78 =	MatrixFactory::createTranslationMatrix(0.0f, -0.14f -0.2f, 0.0f) * // '.14f is half the side of the square
					MatrixFactory::createRotationMatrix4(PI / 4, vec4(0, 0, 1, 1)) * //rotate 45 degrees
					MatrixFactory::createTranslationMatrix(-0.2f, 0.0f, 0.0f); // center in the origin

const mat4 tr9 =	MatrixFactory::createTranslationMatrix(0.8f * 0.707f / 2, -0.2f - 0.28f , 0.0f) * // 0.8f * 0.707f / 2  is the center of the hypotenuse to the origin
					MatrixFactory::createScaleMatrix4(0.707f, 0.707f, 0) *
					MatrixFactory::createRotationMatrix4(-PI, vec4(0, 0, 1, 1));

const vec4 red = vec4( 1.0f, 0.0f, 0.0f, 1.0f );
const vec4 green = vec4( 0.0f, 1.0f, 0.0f, 1.0f );
const vec4 blue = vec4( 0.0f, 0.0f, 1.0f, 1.0f );
const vec4 cyan = vec4( 0.0f, 1.0f, 1.0f, 1.0f );
const vec4 magenta = vec4( 1.0f, 0.0f, 1.0f, 1.0f );
const vec4 yellow = vec4( 1.0f, 1.0f, 0.0f, 1.0f );
const vec4 white = vec4( 1.0f, 1.0f, 1.0f, 1.0f );
const vec4 orange = vec4( 1.0f, 0.2f, 0.0f, 1.0f );
const vec4 purple = vec4( 0.4f, 0.0f, 0.4f, 1.0f );



void drawScene()
{	

	mat4 mvp = projectionMatrix * camera->ViewMatrix();
	std::cout << "vertice 1" << mvp * tr1 * vec4(0, 0, 0, 1.0f) << std::endl;
	std::cout << "vertice 2" << mvp * tr1 * vec4(0.8f, 0, 0, 1.0f) << std::endl;
	std::cout << "vertice 3" << mvp * tr1 * vec4(0.4f, 0.4f, 0, 1.0f) << std::endl;

	triangle->draw(tr1, camera->ViewMatrix(), projectionMatrix, red, prog);
	triangle->draw(tr2, camera->ViewMatrix(), projectionMatrix, green, prog);
	triangle->draw(tr3, camera->ViewMatrix(), projectionMatrix, blue, prog);
	triangle->draw(tr6, camera->ViewMatrix(), projectionMatrix, cyan, prog);
	triangle->draw(tr9, camera->ViewMatrix(), projectionMatrix, magenta, prog);
	square->draw(sq78, camera->ViewMatrix(), projectionMatrix, yellow, prog);
	parallelogram->draw(pl45, camera->ViewMatrix(), projectionMatrix, white, prog);

	//mat4 I = MatrixFactory::createIdentityMatrix4();
	//triangle->draw(tr1, I, I, red, prog);
	//triangle->draw(tr2, I, I, green, prog);
	//triangle->draw(tr3, I, I, blue, prog);
	//triangle->draw(tr6, I, I, cyan, prog);
	//triangle->draw(tr9, I, I, magenta, prog);
	//square->draw(sq78, I, I, yellow, prog);
	//parallelogram->draw(pl45, I, I, white, prog);

	//triangle->draw(tr1, red, prog);
	//triangle->draw(tr2, green, prog);
	//triangle->draw(tr3, blue, prog);
	//triangle->draw(tr6, cyan, prog);
	//triangle->draw(tr9, magenta, prog);
	//square->draw(sq78, yellow, prog);
	//parallelogram->draw(pl45, white, prog);


	//triangle->draw(mvp * tr1, red, prog);
	//triangle->draw(mvp * tr2, green, prog);
	//triangle->draw(mvp * tr3, blue, prog);
	//triangle->draw(mvp * tr6, cyan, prog);
	//triangle->draw(mvp * tr9, magenta, prog);
	//square->draw(mvp * sq78, yellow, prog);
	//parallelogram->draw(mvp * pl45, white, prog);

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
	float currentFrame = glutGet(GLUT_ELAPSED_TIME);
	delta = ((float)currentFrame - (float)lastFrame) / 100;
	lastFrame = (float)currentFrame;
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
	glutKeyboardFunc(keyboard_input);
	glutMotionFunc(mouse_input);
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
	camera = new Camera(vec3(0, 0, 5), vec3(0, 0, 0), vec3(0, 1, 0));
}


void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	setupCamera();
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
