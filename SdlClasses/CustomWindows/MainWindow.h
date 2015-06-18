#ifndef NOISEWINDOW
#define NOISEWINDOW

#include "SdlClasses/SdlWindow.h"

#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

class Map;
class Noise;
class UiObject;
class EventCodes;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

class MainWindow : public SdlWindow
{
	private:
		Map* worldMap;

		// UiEvent handlers
		void contentsChanged(EventCodes &c);

		// UI creation
		void createGui();

		bool handleInternalSdlEvent(SDL_Event& event);

		float angle, deltaAngle;
		float x, y, z;
		float lx, ly, lz, deltaMove;
		float angle2,angle2Y,angleY;
		int deltaX,deltaY;

		glm::mat4 proj;

		void orientMe(float ang);
		void moveMeFlat(float i);

		bool mousePressed;
		bool boolAngle, boolMove;

	public:
		MainWindow(Map* m);
		~MainWindow();

		glm::mat4 getProj();
		glm::mat4 getView();
};

#endif
