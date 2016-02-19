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
	camDir = vector<double>();
	camDir.push_back(0.0);
	camDir.push_back(0.0);
	camDir.push_back(1.0);

	lastRightClickX = 0;
	lastRightClickY = 0;
	rightMouseDown = false;

	shader = 0;
	isRunning = false;
	keys = new bool[4];
	for (int i = 0; i < 4; i++){
		keys[i] = false;
	}

	lastFpsUpdate = 0;
	fpsUpdateDelay = 100;

	lightPosition = vector<double>();
	lightPosition.push_back(0.0);
	lightPosition.push_back(0.0);
	lightPosition.push_back(100.0);

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
		glutSpecialFunc(specialKeyDownEvent);
		glutSpecialUpFunc(specialKeyUpEvent);

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

void ProjectEquinox::specialKeyDownEvent(int key, int x, int y) {
	instance->processSpecialKey(key, true);
}

void ProjectEquinox::specialKeyUpEvent(int key, int x, int y) {
	instance->processSpecialKey(key, false);
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

		if (camPitch > 90)
			camPitch = 90.0;
		else if (camPitch < -90.0)
			camPitch = -90.0;

		double camPitchRad = camPitch / 180.0 * PI;
		double camYawRad = camYaw / 180.0 * PI;

		camDir[2] = sin(camPitchRad);
		camDir[0] = cos(camPitchRad) * sin(camYawRad);
		camDir[1] = cos(camPitchRad) * sin(camYawRad);
	}
}

void ProjectEquinox::processKey(unsigned char key, bool isDown){
	switch (key){
	case 'w':
		if (!isDown)
			planet->subdivide();
		break;
	case 'a':
		break;
	case 's':
		if (!isDown)
			planet->unsubdivide();
		break;
	case 'd':
		break;
	case 'r':
		camPitch = 45.0;
		camYaw = 45.0;
		camZoom = 1.0;
		break;
	case 'g':
		if (false /*!isDown*/ ) {
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
		if(false /*!isDown*/)
			shader = loadShaders("../bin/res/shaders/shader.vert", "../bin/res/shaders/shader.frag");
		break;
	default:
		break;
	}
}

void ProjectEquinox::processSpecialKey(int key, bool isDown) {
	switch (key) {
	case GLUT_KEY_F3:
		if (!isDown) {
			displayHelpKeyList = !displayHelpKeyList;
		}
		break;
	default:
		break;
	}
}

void ProjectEquinox::initialize(){
	//OpenGL
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//Resources
	shader = loadShaders("../executable/res/shaders/shader.vert", "../executable/res/shaders/shader.frag");
	FontManager::getInstance()->addFont("Press Start", "../executable/res/textures/fonts/pressStart.png");

	//Camera
	camPitch = 45;
	camYaw = 45;
	camZoom = 1.0;

	//Terragen
	planet = new Planet();
	planet->generate(0, 256);

	//UI
	lblFps = new TextLabel(FontManager::getInstance()->getFont(0), 8.0, "FPS: ", 10.0, 10.0);
	lblWireframe = new TextLabel(FontManager::getInstance()->getFont(0), lblFps->getSize(), "Wireframe Mode: ", lblFps->getX(), lblFps->getY() + lblFps->getSize() + 10.0);
	lblPolyDrawn = new TextLabel(FontManager::getInstance()->getFont(0), lblFps->getSize(), "Poly count: ", lblFps->getX(), lblWireframe->getY() + lblWireframe->getSize() + 10.0);

	lblFpsVal = new TextLabel(FontManager::getInstance()->getFont(0), lblFps->getSize(), "", lblFps->getWidth(), lblFps->getY());
	lblWireframeVal = new TextLabel(FontManager::getInstance()->getFont(0), lblWireframe->getSize(), std::to_string(wireframeMode), lblWireframe->getWidth(), lblWireframe->getY());
	lblPolyDrawnVal = new TextLabel(FontManager::getInstance()->getFont(0), lblPolyDrawn->getSize(), fixDecimalText(std::to_string(planet->getHeight()), 2), lblPolyDrawn->getWidth(), lblPolyDrawn->getY());

	displayHelpKeyList = false;
	double maxWidthLblHelp = 0.0;
	lblHelpKeyList = new TextLabel(FontManager::getInstance()->getFont(0), lblFps->getSize(), "--- Keys --- (F3 to hide/show)", 0, 10);
	maxWidthLblHelp = (maxWidthLblHelp < lblHelpKeyList->getWidth() ? lblHelpKeyList->getWidth() : maxWidthLblHelp);
	lblHelpKeyW = new TextLabel(FontManager::getInstance()->getFont(0), lblFps->getSize(), "W : Increase terrain polygon count", 0, lblHelpKeyList->getY() + lblHelpKeyList->getSize() + 10.0);
	maxWidthLblHelp = (maxWidthLblHelp < lblHelpKeyW->getWidth() ? lblHelpKeyW->getWidth() : maxWidthLblHelp);
	lblHelpKeyS = new TextLabel(FontManager::getInstance()->getFont(0), lblFps->getSize(), "S : Decrease terrain polygon count", 0, lblHelpKeyW->getY() + lblHelpKeyW->getSize() + 10.0);
	maxWidthLblHelp = (maxWidthLblHelp < lblHelpKeyS->getWidth() ? lblHelpKeyS->getWidth() : maxWidthLblHelp);
	lblHelpKeyR = new TextLabel(FontManager::getInstance()->getFont(0), lblFps->getSize(), "R : Reset camera", 0, lblHelpKeyS->getY() + lblHelpKeyS->getSize() + 10.0);
	maxWidthLblHelp = (maxWidthLblHelp < lblHelpKeyR->getWidth() ? lblHelpKeyR->getWidth() : maxWidthLblHelp);
	lblHelpKeySpace = new TextLabel(FontManager::getInstance()->getFont(0), lblFps->getSize(), "Space : Toggle wireframe mode", 0, lblHelpKeyR->getY() + lblHelpKeyR->getSize() + 10.0);
	maxWidthLblHelp = (maxWidthLblHelp < lblHelpKeySpace->getWidth() ? lblHelpKeySpace->getWidth() : maxWidthLblHelp);
	lblHelpKeyMouseRight = new TextLabel(FontManager::getInstance()->getFont(0), lblFps->getSize(), "Mouse Right : Camera movement (orbit)", 0, lblHelpKeySpace->getY() + lblHelpKeySpace->getSize() + 10.0);
	maxWidthLblHelp = (maxWidthLblHelp < lblHelpKeyMouseRight->getWidth() ? lblHelpKeyMouseRight->getWidth() : maxWidthLblHelp);
	lblHelpKeyMouseWheel = new TextLabel(FontManager::getInstance()->getFont(0), lblFps->getSize(), "Mouse Wheel : Camera zoom (in/out)", 0, lblHelpKeyMouseRight->getY() + lblHelpKeyMouseRight->getSize() + 10.0);
	maxWidthLblHelp = (maxWidthLblHelp < lblHelpKeyMouseWheel->getWidth() ? lblHelpKeyMouseWheel->getWidth() : maxWidthLblHelp);

	double newXPosLblHelp = this->width - maxWidthLblHelp - 10.0;
	lblHelpKeyList->setPosition(newXPosLblHelp, lblHelpKeyList->getY());
	lblHelpKeyW->setPosition(newXPosLblHelp, lblHelpKeyW->getY());
	lblHelpKeyS->setPosition(newXPosLblHelp, lblHelpKeyS->getY());
	lblHelpKeyR->setPosition(newXPosLblHelp, lblHelpKeyR->getY());
	lblHelpKeySpace->setPosition(newXPosLblHelp, lblHelpKeySpace->getY());
	lblHelpKeyMouseRight->setPosition(newXPosLblHelp, lblHelpKeyMouseRight->getY());
	lblHelpKeyMouseWheel->setPosition(newXPosLblHelp, lblHelpKeyMouseWheel->getY());

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
	lblPolyDrawn->update(timeDelta);
	lblPolyDrawnVal->update(timeDelta);

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
	int polyCount = 0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glColor3d(1.0, 1.0, 1.0);

	glPushMatrix();

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
	
	//glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat lightpos[] = {(float)lightPosition[0], (float)lightPosition[1], (float)lightPosition[2], (float)0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	glUseProgram(shader);

	glUniform1i(glGetUniformLocation(shader, "useTex"), 0);

	if(wireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glUniform1f(glGetUniformLocation(shader, "planetRadius"), (float)(planet->getRadius()));
	glUniform1f(glGetUniformLocation(shader, "planetHeight"), (float)(planet->getHeight()));
	glActiveTexture(GL_TEXTURE0);
	//planet->getHeightmap()->bind();
	polyCount += planet->draw();

	glUniform1i(glGetUniformLocation(shader, "useTex"), 0);

	glUseProgram(0);

	glPopMatrix();


	glDisable(GL_LIGHT0);
	//glDisable(GL_LIGHTING);

	//UI
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	lblFps->draw();
	lblFpsVal->draw();
	lblWireframe->draw();
	lblWireframeVal->draw();
	lblPolyDrawn->draw();
	lblPolyDrawnVal->setText("" + to_string(polyCount));
	lblPolyDrawnVal->draw();

	lblHelpKeyList->draw();
	if (displayHelpKeyList) {
		lblHelpKeyW->draw();
		lblHelpKeyS->draw();
		lblHelpKeyR->draw();
		lblHelpKeySpace->draw();
		lblHelpKeyMouseRight->draw();
		lblHelpKeyMouseWheel->draw();
	}

	updateView();

	if(glutGetWindow() != 0)
		glutSwapBuffers();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

double ProjectEquinox::compareWithCamDir(std::vector<double> dir) {
	return (dir[0] * camDir[0] + dir[1] * camDir[1] + dir[2] * camDir[2]) / (camDir[0] * camDir[0] + camDir[1] * camDir[1] + camDir[2] * camDir[2]);
}

int main(int argc, char** argv){
	ProjectEquinox* program = ProjectEquinox::getInstance();
	program->initializeWindow(argc, argv);
	program->release();

	return 0;
}