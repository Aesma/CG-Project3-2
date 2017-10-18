#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "camera.h"

class PerspectiveCamera :public Camera
{
public:
	PerspectiveCamera(Vec3f &center, Vec3f &direction, Vec3f &up, float angle);
	virtual Ray generateRay(Vec2f point);
	virtual float getTMin() const;
private:
	Vec3f center;
	Vec3f direction;
	Vec3f horizontal;
	Vec3f up;
	float angle;
	float distance;
};

#endif