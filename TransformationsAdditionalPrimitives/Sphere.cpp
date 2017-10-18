#include "sphere.h"

Sphere::Sphere(Vec3f center, float radius, Material* material)
{
	this->center = center;
	this->radius = radius;
	this->material = material;
}

bool Sphere::intersect(const Ray &r, Hit &h, float tmin)
{
	bool result = false;

	Vec3f direction = r.getDirection();
	Vec3f origin = r.getOrigin();

	float a = direction.Dot3(direction);
	float b = 2.0f * direction.Dot3(origin - (center));
	float c = (origin - (center)).Length() * (origin - (center)).Length() - radius * radius;
	float delta = b * b - 4 * a * c;
	float t = INT_MAX * 1.0f;
	if (delta > 0){
		float d = sqrt(delta);
		float t1 = (-b - d) / (a * 2.0f);
		float t2 = (-b + d) / (a * 2.0f);
		if (t1 >= tmin){
			t = t1;
		}else if (t2 >= tmin){
			t = t2;
		}
		if (t < h.getT()){
			Vec3f normal = origin + direction * t - center;
			normal.Normalize();
			h.set(t, material, normal, r);
		}
		result = true;
	}
	else if (delta == 0){
		float t2 = -b * 0.5f;
		if (t >= tmin){
			t = t2;
		}
		if (t < h.getT()){
			Vec3f normal = origin + direction * t - center;
			normal.Normalize();
			h.set(t, material, normal, r);
		}
		result = true;
	}
	return result;
}

Sphere::~Sphere()
{
	
}