#include "GlCamera.h"

#include "Events/EventAggregator.h"
#include "Events/CustomEvents/WindowResized.h"

GlCamera::GlCamera(int w, int h) : x(0), y(0), z(0), lookAtX(1), lookAtY(1), lookAtZ(1)
{
	projection = glm::perspective(glm::radians(CAMERA_FOV), w/(float)h, CAMERA_NEAR, CAMERA_FAR);

	EventAggregator::Instance()->getEvent<WindowResized>().subscribe(
															[&](WindowResized &e){ adjustProjection(e.w, e.h); });
}

GlCamera::~GlCamera()
{

}

void GlCamera::adjustProjection(int w, int h)
{
	projection = glm::perspective(glm::radians(CAMERA_FOV), w/(float)h, CAMERA_NEAR, CAMERA_FAR);
}

glm::mat4 GlCamera::getView()
{
	return glm::lookAt(	glm::vec3(x, y, z), 
						glm::vec3(lookAtX, lookAtY, lookAtZ),
						glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 GlCamera::getProj()
{
	return projection;
}

void GlCamera::setPos(double newX, double newY, double newZ)
{
	x = newX;
	y = newY;
	z = newZ;
}

void GlCamera::lookAtPos(double newX, double newY, double newZ)
{
	lookAtX = newX;
	lookAtY = newY;
	lookAtZ = newZ;
}
