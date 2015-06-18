#ifndef UIOBJECT
#define UIOBJECT

#include <list>
#include <functional>

#include <SDL2/SDL.h>

class SdlTexture;

class UiObject
{
	private:
		int absoluteX, absoluteY;
		int xOffset, yOffset; // offset from parent's 0, 0

		int width, height;
		double scaleW, scaleH;
		double windowScaleW, windowScaleH;

		SDL_Renderer *renderer;
		SdlTexture *uiTexture;

		std::function<bool(SDL_Event& e)> evtHandler;
		std::function<void()> preRenderProcedure;
		std::function<void()> postRenderProcedure;

		std::list<UiObject*> childList;

	protected:
		SDL_Renderer* getRenderer();

	public:
		UiObject(SDL_Renderer *r, int xOff, int yOff, int w, int h);
		UiObject(SDL_Renderer *r, int xOff, int yOff, SdlTexture *t, std::function<bool(SDL_Event& e)> evth);
		UiObject(SDL_Renderer *r, int xOff, int yOff, int w, int h, SdlTexture *t, std::function<bool(SDL_Event& e)> evth);
		virtual ~UiObject();

		void addChild(UiObject *c);

		// Settings
		void setUiObjectTexture(SdlTexture *t);
		void setUiObjectOffset(int x, int y); // Offset from parent
		void setUiObjectSize(int w, int h);
		
		int getWidth();
		int getHeight();

		int getAbsoluteX();
		int getAbsoluteY();

		// Render
		void setPreRenderProcedure(std::function<void()> procedure);
		void setPostRenderProcedure(std::function<void()> procedure);
		void render(int parentX, int parentY);
		void renderScaled(int parentX, int parentY, double sW, double sH);

		// Events
		void setSdlEventHandler(std::function<bool(SDL_Event& e)> evth);
		bool handleSdlEvent(SDL_Event& e);

		// Mouse
		bool isMouseInside();
		static bool getRelativeMousePos(UiObject *obj, int *x, int *y);

		// Window
		void setWindowScale(double sW, double sH);
		double getWindowScaleW();
		double getWindowScaleH();
};

# endif