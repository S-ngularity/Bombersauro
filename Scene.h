#ifndef SCENE
#define SCENE

#include <list>

#include <glm/mat4x4.hpp>

class GlObject
{
	private:
		std::list<GlObject*> objectList;

	public:
		// Render
		void render(glm::mat4 &cameraMatrix)
		{
			for(GlObject *o : objectList)
				o->render(cameraMatrix);
		}
		
		void addChild(GlObject *c)
		{
			objectList.push_back(c);
		}
};

#endif