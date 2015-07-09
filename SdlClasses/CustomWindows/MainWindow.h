#ifndef MAINWINDOW
#define MAINWINDOW

#include "SdlClasses/SdlWindow.h"

#include "GlClasses/GlCamera.h"
#include "GlClasses/GlObject.h"

#include "Player.h"

#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class EventCode;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

class MainWindow : public SdlWindow
{
	private:
		GlObject *mapObject, *lua;

		// UiEvent handlers
		void contentsChanged(EventCode &c);

		bool handleInternalSdlEvent(SDL_Event& event);

		// UI creation
		void createGui();
		
		void initMapObject();

	public:
		MainWindow();
		~MainWindow();

		void initScene();

		void renderScene();
};

#endif
