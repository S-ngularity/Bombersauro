#ifndef MAINWINDOW
#define MAINWINDOW

#include "SdlClasses/SdlWindow.h"

#include "GlCamera.h"
#include "Geometry/GlObject.h"


#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Map;
class EventCodes;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

class MainWindow : public SdlWindow
{
	private:
		Map* worldMap;

		GlCamera camera;

		GlObject *mapObject;

		// UiEvent handlers
		void contentsChanged(EventCodes &c);

		bool handleInternalSdlEvent(SDL_Event& event);

		// UI creation
		void createGui();
		
		void initMapObject();

	public:
		MainWindow(Map* m);
		~MainWindow();

		void initScene();

		void tick();

		void renderScene();
};

#endif
