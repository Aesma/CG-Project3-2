#include "perspectivecamera.h"

PerspectiveCamera::PerspectiveCamera(Vec3f &center, Vec3f &direction, Vec3f &up, float angle)
{
	this->center = center;
	this->direction = direction;
	this->up = up;
	this->angle = angle;
	Vec3f::Cross3(this->horizontal, this->direction, this->up);
	Vec3f::Cross3(this->up, this->horizontal, this->direction);
	this->direction.Normalize();
	this->up.Normalize();
	this->horizontal.Normalize();
	this->distance = 1.0f / tanf(angle / 2);
}

Ray PerspectiveCamera::generateRay(Vec2f point)
{
	Vec3f dir = horizontal * point.x() + up * point.y() + (direction) * distance;
	dir.Normalize();

	return Ray(center, dir);
}

float PerspectiveCamera::getTMin() const
{
	return -1.0f * INT_MAX;
}