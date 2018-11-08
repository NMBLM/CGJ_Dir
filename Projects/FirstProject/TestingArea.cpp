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
//
//#include "GL/glew.h"
//#include "GL/freeglut.h"
//
//#define CAPTION "Loading World"
//
//int WinX = 640, WinY = 480;
//int WindowHandle = 0;
//unsigned int FrameCount = 0;
//
//#define VERTICES 0
//#define TEXCOORDS 1
//#define NORMALS 2
//bool TexcoordsLoaded, NormalsLoaded;
//
//GLuint VaoId, ProgramId;
//GLint ModelMatrix_UId, ViewMatrix_UId, ProjectionMatrix_UId;
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
///////////////////////////////////////////////////////////////////////// SHADERs
//
//const std::string read(const std::string& filename)
//{
//	std::ifstream ifile(filename);
//	std::string shader_string, line;
//	while (std::getline(ifile, line)) {
//		shader_string += line + "\n";
//	}
//	return shader_string;
//}
//
//const GLuint checkCompilation(const GLuint shader_id, const std::string& filename)
//{
//	GLint compiled;
//	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
//	if (compiled == GL_FALSE) {
//		GLint length;
//		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
//		GLchar* const log = new char[length];
//		glGetShaderInfoLog(shader_id, length, &length, log);
//		std::cerr << "[" << filename << "] " << std::endl << log;
//		delete log;
//	}
//	return compiled;
//}
//
//void checkLinkage(const GLuint program_id) {
//	GLint linked;
//	glGetProgramiv(program_id, GL_LINK_STATUS, &linked);
//	if (linked == GL_FALSE) {
//		GLint length;
//		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &length);
//		GLchar* const log = new char[length];
//		glGetProgramInfoLog(program_id, length, &length, log);
//		std::cerr << "[LINK] " << std::endl << log << std::endl;
//		delete log;
//	}
//}
//
//const GLuint addShader(const GLuint program_id, const GLenum shader_type, const std::string& filename)
//{
//	const GLuint shader_id = glCreateShader(shader_type);
//	const std::string scode = read(filename);
//	const GLchar* code = scode.c_str();
//	glShaderSource(shader_id, 1, &code, 0);
//	glCompileShader(shader_id);
//	checkCompilation(shader_id, filename);
//	glAttachShader(program_id, shader_id);
//	return shader_id;
//}
//
//void createShaderProgram(std::string& vs_file, std::string& fs_file)
//{
//	ProgramId = glCreateProgram();
//
//	GLuint VertexShaderId = addShader(ProgramId, GL_VERTEX_SHADER, vs_file);
//	GLuint FragmentShaderId = addShader(ProgramId, GL_FRAGMENT_SHADER, fs_file);
//
//	glBindAttribLocation(ProgramId, VERTICES, "inPosition");
//	if (TexcoordsLoaded)
//		glBindAttribLocation(ProgramId, TEXCOORDS, "inTexcoord");
//	if (NormalsLoaded)
//		glBindAttribLocation(ProgramId, NORMALS, "inNormal");
//
//	glLinkProgram(ProgramId);
//	checkLinkage(ProgramId);
//
//	glDetachShader(ProgramId, VertexShaderId);
//	glDetachShader(ProgramId, FragmentShaderId);
//	glDeleteShader(VertexShaderId);
//	glDeleteShader(FragmentShaderId);
//
//	ModelMatrix_UId = glGetUniformLocation(ProgramId, "ModelMatrix");
//	ViewMatrix_UId = glGetUniformLocation(ProgramId, "ViewMatrix");
//	ProjectionMatrix_UId = glGetUniformLocation(ProgramId, "ProjectionMatrix");
//
//	checkOpenGLError("ERROR: Could not create shaders.");
//}
//
//void destroyShaderProgram()
//{
//	glUseProgram(0);
//	glDeleteProgram(ProgramId);
//
//	checkOpenGLError("ERROR: Could not destroy shaders.");
//}
//
///////////////////////////////////////////////////////////////////////// MESH
//
//typedef struct {
//	GLfloat x, y, z;
//} Vertex;
//
//typedef struct {
//	GLfloat u, v;
//} Texcoord;
//
//typedef struct {
//	GLfloat nx, ny, nz;
//} Normal;
//
//std::vector <Vertex> Vertices, vertexData;
//std::vector <Texcoord> Texcoords, texcoordData;
//std::vector <Normal> Normals, normalData;
//
//std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;
//
//void parseVertex(std::stringstream& sin)
//{
//	Vertex v;
//	sin >> v.x >> v.y >> v.z;
//	vertexData.push_back(v);
//}
//
//void parseTexcoord(std::stringstream& sin)
//{
//	Texcoord t;
//	sin >> t.u >> t.v;
//	texcoordData.push_back(t);
//}
//
//void parseNormal(std::stringstream& sin)
//{
//	Normal n;
//	sin >> n.nx >> n.ny >> n.nz;
//	normalData.push_back(n);
//}
//
//void parseFace(std::stringstream& sin)
//{
//	std::string token;
//	for (int i = 0; i < 3; i++) {
//		std::getline(sin, token, '/');
//		if (token.size() > 0) vertexIdx.push_back(std::stoi(token));
//		std::getline(sin, token, '/');
//		if (token.size() > 0) texcoordIdx.push_back(std::stoi(token));
//		std::getline(sin, token, ' ');
//		if (token.size() > 0) normalIdx.push_back(std::stoi(token));
//	}
//}
//
//void parseLine(std::stringstream& sin)
//{
//	std::string s;
//	sin >> s;
//	if (s.compare("v") == 0) parseVertex(sin);
//	else if (s.compare("vt") == 0) parseTexcoord(sin);
//	else if (s.compare("vn") == 0) parseNormal(sin);
//	else if (s.compare("f") == 0) parseFace(sin);
//}
//
//void loadMeshData(std::string& filename)
//{
//	std::ifstream ifile(filename);
//	std::string line;
//	std::stringstream ss;
//	while (std::getline(ifile, line)) {
//		ss = std::stringstream(line);
//		parseLine(ss);
//	}
//	TexcoordsLoaded = (texcoordIdx.size() > 0);
//	NormalsLoaded = (normalIdx.size() > 0);
//}
//
//void processMeshData()
//{
//	for (unsigned int i = 0; i < vertexIdx.size(); i++) {
//		unsigned int vi = vertexIdx[i];
//		Vertex v = vertexData[vi - 1];
//		Vertices.push_back(v);
//		if (TexcoordsLoaded) {
//			unsigned int ti = texcoordIdx[i];
//			Texcoord t = texcoordData[ti - 1];
//			Texcoords.push_back(t);
//		}
//		if (NormalsLoaded) {
//			unsigned int ni = normalIdx[i];
//			Normal n = normalData[ni - 1];
//			Normals.push_back(n);
//		}
//	}
//}
//
//void freeMeshData()
//{
//	vertexData.clear();
//	texcoordData.clear();
//	normalData.clear();
//	vertexIdx.clear();
//	texcoordIdx.clear();
//	normalIdx.clear();
//}
//
//const void createMesh(std::string& filename)
//{
//	loadMeshData(filename);
//	processMeshData();
//	freeMeshData();
//}
//
///////////////////////////////////////////////////////////////////////// VAOs & VBOs
//
//void createBufferObjects()
//{
//	GLuint VboVertices, VboTexcoords, VboNormals;
//
//	glGenVertexArrays(1, &VaoId);
//	glBindVertexArray(VaoId);
//	{
//		glGenBuffers(1, &VboVertices);
//		glBindBuffer(GL_ARRAY_BUFFER, VboVertices);
//		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);
//		glEnableVertexAttribArray(VERTICES);
//		glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
//
//		if (TexcoordsLoaded)
//		{
//			glGenBuffers(1, &VboTexcoords);
//			glBindBuffer(GL_ARRAY_BUFFER, VboTexcoords);
//			glBufferData(GL_ARRAY_BUFFER, Texcoords.size() * sizeof(Texcoord), &Texcoords[0], GL_STATIC_DRAW);
//			glEnableVertexAttribArray(TEXCOORDS);
//			glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Texcoord), 0);
//		}
//		if (NormalsLoaded)
//		{
//			glGenBuffers(1, &VboNormals);
//			glBindBuffer(GL_ARRAY_BUFFER, VboNormals);
//			glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(Normal), &Normals[0], GL_STATIC_DRAW);
//			glEnableVertexAttribArray(NORMALS);
//			glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), 0);
//		}
//	}
//	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glDeleteBuffers(1, &VboVertices);
//	glDeleteBuffers(1, &VboTexcoords);
//	glDeleteBuffers(1, &VboNormals);
//
//	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
//}
//
//void destroyBufferObjects()
//{
//	glBindVertexArray(VaoId);
//	glDisableVertexAttribArray(VERTICES);
//	glDisableVertexAttribArray(TEXCOORDS);
//	glDisableVertexAttribArray(NORMALS);
//	glDeleteVertexArrays(1, &VaoId);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
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
//	0.0f,  1.0f,  0.0f,  0.0f,
//	0.0f,  0.0f,  1.0f,  0.0f,
//	0.0f,  0.0f,  0.0f,  1.0f
//}; // Column Major
//
//// Eye(5,5,5) Center(0,0,0) Up(0,1,0)
//const Matrix ViewMatrix1 = {
//	0.70f, -0.41f,  0.58f,  0.00f,
//	0.00f,  0.82f,  0.58f,  0.00f,
//   -0.70f, -0.41f,  0.58f,  0.00f,
//	0.00f,  0.00f, -8.70f,  1.00f
//}; // Column Major
//
//// Eye(-5,-5,-5) Center(0,0,0) Up(0,1,0)
//const Matrix ViewMatrix2 = {
//   -0.70f, -0.41f, -0.58f,  0.00f,
//	0.00f,  0.82f, -0.58f,  0.00f,
//	0.70f, -0.41f, -0.58f,  0.00f,
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
//	glBindVertexArray(VaoId);
//	glUseProgram(ProgramId);
//
//	glUniformMatrix4fv(ModelMatrix_UId, 1, GL_FALSE, ModelMatrix);
//	glUniformMatrix4fv(ViewMatrix_UId, 1, GL_FALSE, ViewMatrix2);
//	glUniformMatrix4fv(ProjectionMatrix_UId, 1, GL_FALSE, ProjectionMatrix2);
//	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)Vertices.size());
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
//	FrameCount = 0;
//	glutTimerFunc(1000, timer, 0);
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
//	GLenum result = glewInit();
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
//	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
//
//	glutInitWindowSize(WinX, WinY);
//	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
//	WindowHandle = glutCreateWindow(CAPTION);
//	if (WindowHandle < 1) {
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
//	//	createMesh(std::string("../../assets/models/blender_2.79/cube_vn.obj"));
//	//	createMesh(std::string("../../assets/models/blender_2.79/cube_vtn.obj"));
//	//	createMesh(std::string("../../assets/models/blender_2.79/torus_vn.obj"));
//	//	createMesh(std::string("../../assets/models/blender_2.79/torus_smooth_vn.obj"));
//	std::string mesh = std::string("src/cube.obj");
//	createMesh(mesh);
//	//	createMesh(std::string("../../assets/models/blender_2.79/utah_teapot_vtn.obj"));
//	//	createMesh(std::string("../../assets/models/blender_2.79/standford_bunny_vn.obj"));
//	std::string vs = std::string("cube_vs.glsl");
//	std::string fs = std::string("cube_fs.glsl");
//	createShaderProgram(vs,fs);
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