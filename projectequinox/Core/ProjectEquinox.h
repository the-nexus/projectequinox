
#ifndef PROJECT_EQUINOX_H
#define PROJECT_EQUINOX_H

#include <string>
#include <vector>
#include "../Graphics/Texture.h"
#include "../UI/TextLabel.h"
#include "../Terragen/Planet.h"

class ProjectEquinox{
public:
	static ProjectEquinox* getInstance();
	static void updateViewport(int width, int height);
	static void refreshWindow();
	static void loop(int lastTime);
	static void mouseClickEvent(int button, int state, int x, int y);
	static void mouseMovementEvent(int x, int y);
	static void keyDownEvent(unsigned char key, int x, int y);
	static void keyUpEvent(unsigned char key, int x, int y);

	void initializeWindow(int argc, char** argv);
	void initialize();
	int getWidth();
	int getHeight();
	void updateScene(int timeDelta);
	void updateView();
	void renderScene();
	void processMouseClick(int button, int state, int x, int y);
	void processMouseMovement(int x, int y);
	void processKey(unsigned char key, bool isDown);
	int getDelay();
	void release();

	void setViewToOrtho();
	void setViewToPerspective();
	std::string fixDecimalText(std::string text, int decimalCount);

	double compareWithCamDir(std::vector<double> dir);

private:
	ProjectEquinox();
	~ProjectEquinox();

	static ProjectEquinox* instance;

	int loadShaders(char* vertFileName, char* fragFileName);

	//Constants
	const int FPS = 60;
	const int DELAY = 1000 / FPS;
	const double PI = 3.14159;

	//Execution
	bool isRunning;

	//Inputs
	bool* keys;
	int lastRightClickX;
	int lastRightClickY;
	bool rightMouseDown;

	//View
	int width;
	int height;
	double camYaw;
	double camPitch;
	double camZoom;
	std::vector<double> camDir;

	//Graphics
	int shader;
	bool wireframeMode;
	std::vector<double> lightPosition;

	//FPS
	int lastFpsUpdate;
	int fpsUpdateDelay;

	//Terragen
	Planet* planet;

	//UI
	TextLabel* lblFps;
	TextLabel* lblWireframe;
	TextLabel* lblPolyDrawn;

	TextLabel* lblFpsVal;
	TextLabel* lblWireframeVal;
	TextLabel* lblPolyDrawnVal;

};

#endif