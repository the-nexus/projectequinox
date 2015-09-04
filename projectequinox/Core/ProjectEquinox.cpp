#include <iostream>
#include <fstream>
#include <string>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "ProjectEquinox.h"
#include "ResourceInitializer.h"
#include "../IO/FontManager.h"

using namespace std;
ProjectEquinox* ProjectEquinox::instance = NULL;


ProjectEquinox::ProjectEquinox(){
	width = 1280;
	height = 720;
	camYaw = 0.0;
	camPitch = 0.0;

	lastRightClickX = 0;
	lastRightClickY = 0;
	rightMouseDown = false;

	posX = 0.0;
	posY = 0.0;
	speed = 0.001;
	shader = 0;
	isRunning = false;
	keys = new bool[4];
	for (int i = 0; i < 4; i++){
		keys[i] = false;
	}

	lastFpsUpdate = 0;
	fpsUpdateDelay = 100;

	wireframeMode = false;
}

ProjectEquinox::~ProjectEquinox() {

}

ProjectEquinox* ProjectEquinox::getInstance(){
	if (instance == nullptr)
		instance = new ProjectEquinox();
	return instance;
}

string ProjectEquinox::fixDecimalText(string text, int decimalCount) {
	int dotPos = text.find(".");
	if (dotPos != string::npos && text.length() > (unsigned int)(dotPos + decimalCount + 1))
		return text.substr(0, dotPos + decimalCount + 1);
	else
		return text;
}

void ProjectEquinox::initializeWindow(int argc, char** argv){
	if (!isRunning){
		//Initialize GLUT
		glutInit(&argc, argv);

		//Setup the memory buffers for the display
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
		//Set the window size
		glutInitWindowSize(width, height);
		//Create the window with a specific title
		glutCreateWindow("Project Epsilon");
		//Bind the two fuctions above to respond when necessary
		glutReshapeFunc(ProjectEquinox::updateViewport);
		glutDisplayFunc(ProjectEquinox::refreshWindow);

		//FullScreen
		//glutGameModeString("800×600:32@60");
		//glutEnterGameMode();

		//Mouse input
		glutMouseFunc(mouseClickEvent);
		glutPassiveMotionFunc(mouseMovementEvent);
		glutMotionFunc(mouseMovementEvent);
		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

		//Keyboard input
		glutKeyboardFunc(keyDownEvent);
		glutKeyboardUpFunc(keyUpEvent);

		//Initialize the entry points of the OpenGL Driver to be able to call all the functions in the API
		GLenum err = glewInit();
		if (err != GLEW_OK){
			fprintf(stderr, "GLEW failed to initialize");
			exit(1);
		}

		initialize();
		loop(0);
		isRunning = true;
		glutMainLoop();
	}
}

void ProjectEquinox::release(){
	delete planet;
	cout << "Application terminated" << endl;
}

void ProjectEquinox::refreshWindow(){

}

void ProjectEquinox::mouseClickEvent(int button, int state, int x, int y)
{
	instance->processMouseClick(button, state, x, y);
}

void ProjectEquinox::mouseMovementEvent(int x, int y){
	instance->processMouseMovement(x, y);
}

void ProjectEquinox::keyDownEvent(unsigned char key, int x, int y){
	instance->processKey(key, true);
}

void ProjectEquinox::keyUpEvent(unsigned char key, int x, int y){
	instance->processKey(key, false);
}

void ProjectEquinox::updateViewport(int width, int height){
	double w = instance->width;
	double h = instance->height;
	//glViewport(0, 0, (int)w, (int)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, w / h, 0.01, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}

void ProjectEquinox::setViewToOrtho() {
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, width, height, 0, 1, -1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void ProjectEquinox::setViewToPerspective() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
}

int ProjectEquinox::loadShaders(char* vertFileName, char* fragFileName){
	ifstream vertIfs, fragIfs;

	vertIfs.open(vertFileName);
	fragIfs.open(fragFileName);
	if (!vertIfs){
		cout << "Vertex Shader file failed to open" << endl;
		return 0;
	}
	if (!fragIfs){
		cout << "Fragment Shader file failed to open" << endl;
		return 0;
	}
	string vertSrc = string(istreambuf_iterator<char>(vertIfs), istreambuf_iterator<char>());
	string fragSrc = string(istreambuf_iterator<char>(fragIfs), istreambuf_iterator<char>());
	int vertShader = glCreateShader(GL_VERTEX_SHADER);
	int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	vertIfs.close();
	fragIfs.close();
	const char* vertSources[] = { vertSrc.c_str() };
	const char* fragSources[] = { fragSrc.c_str() };

	glShaderSource(vertShader, 1, vertSources, NULL);
	glCompileShader(vertShader);

	glShaderSource(fragShader, 1, fragSources, NULL);
	glCompileShader(fragShader);

	int compileStatus;
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE){
		cout << "Vertex Shader file failed to compile" << endl;
		GLint logLength;
		glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];
		glGetShaderInfoLog(vertShader, logLength, NULL, infoLog);

		cout << infoLog << std::endl;
		delete infoLog;
		return 0;
	}
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE){
		cout << "Fragment Shader file failed to compile" << endl;
		GLint logLength;
		glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];
		glGetShaderInfoLog(fragShader, logLength, NULL, infoLog);

		cout << infoLog << std::endl;

		delete infoLog;
		return 0;
	}

	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);

	glLinkProgram(shaderProgram);

	GLint linkStatus;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE){
		cout << "Program failed linking" << endl;
		GLint logLength;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];

		glGetProgramInfoLog(shaderProgram, logLength, NULL, infoLog);

		cout << infoLog << std::endl;

		delete infoLog;
		return 0;
	}
	cout << "Shader was successfully compiled" << endl;
	return shaderProgram;
}

void ProjectEquinox::loop(int lastTime){

	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	if (lastTime == 0)
		lastTime = currentTime;

	glutTimerFunc(ProjectEquinox::getInstance()->getDelay(), ProjectEquinox::loop, currentTime);

	int timeDelta = currentTime - lastTime;
	ProjectEquinox::getInstance()->updateView();
	ProjectEquinox::getInstance()->updateScene(timeDelta);
	ProjectEquinox::getInstance()->renderScene();
}

int ProjectEquinox::getDelay(){
	return DELAY;
}

int ProjectEquinox::getWidth(){
	return width;
}

int ProjectEquinox::getHeight(){
	return height;
}

void ProjectEquinox::processMouseClick(int button, int state, int x, int y){
	if (button == GLUT_RIGHT_BUTTON){
		if (state == GLUT_DOWN)
		{
			lastRightClickX = x;
			lastRightClickY = y;
			rightMouseDown = true;
		}
		else{
			rightMouseDown = false;
		}
	}
	else if (button == 3 && state == GLUT_UP) {
		camZoom -= 0.05;
		if (camZoom < 0.25)
			camZoom = 0.25;
	}
	else if (button == 4 && state == GLUT_UP) {
		camZoom += 0.05;
		if (camZoom > 2.0)
			camZoom = 2.0;
	}
}

void ProjectEquinox::processMouseMovement(int x, int y){
	if (rightMouseDown){
		camYaw += lastRightClickX - x;
		camPitch += lastRightClickY - y;
		lastRightClickX = x;
		lastRightClickY = y;

		if (camYaw >= 360.0)
			camYaw -= 360.0;
		else if (camYaw < 0.0)
			camYaw += 360.0;

		if (camPitch >= 90)
			camPitch = 90.0;
		else if (camPitch <= 0.0)
			camPitch = 0.0;
	}
}

void ProjectEquinox::processKey(unsigned char key, bool isDown){
	switch (key){
	case 'w':
		keys[0] = isDown;
		break;
	case 'a':
		break;
	case 's':
		keys[2] = isDown;
		break;
	case 'd':
		break;
	case 'r':
		camPitch = 45.0;
		camYaw = 45.0;
		camZoom = 1.0;
		break;
	case 'g':
		if (!isDown) {
			int seed;
			cout << "Enter a seed: ";
			cin >> seed;
			int res;
			cout << "Enter a resolution: ";
			cin >> res;
			planet->generate(seed, res);
		}
		break;
	case ' ':
		if (!isDown) {
			wireframeMode = !wireframeMode;
			lblWireframeVal->setText(std::to_string(wireframeMode));
		}
		break;
	case 'v':
		if(!isDown)
			shader = loadShaders("../bin/res/shaders/shader.vert", "../bin/res/shaders/shader.frag");
		break;
	}
}

void ProjectEquinox::initialize(){
	//OpenGL
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//Resources
	shader = loadShaders("../bin/res/shaders/shader.vert", "../bin/res/shaders/shader.frag");
	FontManager::getInstance()->addFont("Press Start", "../bin/res/textures/fonts/pressStart.png");

	//Camera
	camPitch = 45;
	camYaw = 45;
	camZoom = 1.0;

	//Terragen
	planet = new Planet();
	planet->generate(0, 256);

	//UI
	lblFps = new TextLabel(FontManager::getInstance()->getFont(0), 16.0, "FPS: ", 10.0, 10.0);
	lblWireframe = new TextLabel(FontManager::getInstance()->getFont(0), lblFps->getSize(), "Wireframe Mode: ", lblFps->getX(), lblFps->getY() + lblFps->getSize() + 10.0);
	lblHeightmap = new TextLabel(FontManager::getInstance()->getFont(0), lblFps->getSize(), "Terrain Height: ", lblFps->getX(), lblWireframe->getY() + lblWireframe->getSize() + 10.0);

	lblFpsVal = new TextLabel(FontManager::getInstance()->getFont(0), lblFps->getSize(), "", lblFps->getWidth(), lblFps->getY());
	lblWireframeVal = new TextLabel(FontManager::getInstance()->getFont(0), lblWireframe->getSize(), std::to_string(wireframeMode), lblWireframe->getWidth(), lblWireframe->getY());
	lblHeightmapVal = new TextLabel(FontManager::getInstance()->getFont(0), lblHeightmap->getSize(), fixDecimalText(std::to_string(planet->getHeight()), 2), lblHeightmap->getWidth(), lblHeightmap->getY());

	cout << "Finished initialization!" << endl;
}

void ProjectEquinox::updateScene(int timeDelta){

	//UI
	lblFps->update(timeDelta);
	lblFpsVal->update(timeDelta);
	lastFpsUpdate += timeDelta;
	if (lastFpsUpdate > fpsUpdateDelay) {
		double fps = 1000.0 / (double)timeDelta;
		string fpsText = std::to_string(fps);
		lblFpsVal->setText(fixDecimalText(fpsText, 2));
		lastFpsUpdate %= fpsUpdateDelay;
	}
	lblWireframe->update(timeDelta);
	lblWireframeVal->update(timeDelta);
	lblHeightmap->update(timeDelta);
	lblHeightmapVal->update(timeDelta);
	if (false && keys[0]) {
		double heightVal = planet->getHeight() + 1.0;
		planet->setHeight(heightVal);
		string heightmapText = std::to_string(heightVal);
		lblHeightmapVal->setText(fixDecimalText(heightmapText, 2));
	}

	if (false && keys[2]) {
		double heightVal = planet->getHeight() - 1.0;
		planet->setHeight(heightVal);
		string heightmapText = std::to_string(heightVal);
		lblHeightmapVal->setText(fixDecimalText(heightmapText, 2));
	}
	//Terragen
	planet->update(timeDelta);
}

void ProjectEquinox::updateView(){
	glLoadIdentity();
	glTranslated(0.0, 0.0, -100.0 * camZoom);
	glRotated(camPitch, 1.0, 0.0, 0.0);
	glRotated(camYaw, 0.0, 1.0, 0.0);
}

void ProjectEquinox::renderScene(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	
	//glUseProgram(shader);

	glUniform1i(glGetUniformLocation(shader, "useTex"), 0);

	glPushMatrix();

	glColor3d(1.0, 1.0, 1.0);

	glBegin(GL_LINES);
	{
		glColor3d(1.0, 0.0, 0.0);
		glVertex3d(-100.0, 0.0, 0.0);
		glVertex3d(100.0, 0.0, 0.0);

		glColor3d(0.0, 1.0, 0.0);
		glVertex3d(0.0, -100.0, 0.0);
		glVertex3d(0.0, 100.0, 0.0);

		glColor3d(0.0, 0.0, 1.0);
		glVertex3d(0.0, 0.0, -100.0);
		glVertex3d(0.0, 0.0, 100.0);
	}
	glEnd();

	if(wireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUniform1i(glGetUniformLocation(shader, "useTex"), 1);
	glUniform1i(glGetUniformLocation(shader, "heightmap"), 0);
	glUniform1f(glGetUniformLocation(shader, "height"), (float)(planet->getHeight()));
	glActiveTexture(GL_TEXTURE0);
	//planet->getHeightmap()->bind();
	planet->draw();

	glUniform1i(glGetUniformLocation(shader, "useTex"), 0);

	glUseProgram(0);

	glPopMatrix();

	//UI
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	lblFps->draw();
	lblFpsVal->draw();
	lblWireframe->draw();
	lblWireframeVal->draw();
	//lblHeightmap->draw();
	//lblHeightmapVal->draw();

	updateView();

	if(glutGetWindow() != 0)
		glutSwapBuffers();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

int main(int argc, char** argv){
	ProjectEquinox* program = ProjectEquinox::getInstance();
	program->initializeWindow(argc, argv);
	program->release();

	return 0;
}