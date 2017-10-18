#include "plane.h"

Plane::Plane(Vec3f &normal, float d, Material *m)
{
	this->material = m;
	this->normal = normal;
	this->normal.Normalize();
	this->d = d;
}

bool Plane::intersect(const Ray &r, Hit &h, float tmin)
{
	bool result = false;
	if (normal.Dot3(r.getDirection()) == 0)
	{
		result = false;
	}
	float t = (d - normal.Dot3(r.getOrigin())) / normal.Dot3(r.getDirection());
	if (t > tmin)
	{
		if (t < h.getT())
		{
			result = true;
			h.set(t, material, normal, r);
		}
	}
	return result;
}