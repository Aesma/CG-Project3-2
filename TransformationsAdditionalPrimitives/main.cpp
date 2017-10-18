#include "scene_parser.h"
#include "image.h"
#include "group.h"
#include "hit.h"
#include "light.h"
#include "OrthographicCamera.h"
#include "perspectivecamera.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

int main(int argc, char*argv[])
{
	char *input_file = NULL;
	int width = 100;
	int height = 100;
	char *output_file = NULL;
	float depth_min = 0;
	float depth_max = 1;
	char *depth_file = NULL;

	char *normal_file = NULL;
	bool shade_back = false;
	// sample command line:
	// raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-input")) {
			i++; assert(i < argc);
			input_file = argv[i];
		}
		else if (!strcmp(argv[i], "-size")) {
			i++; assert(i < argc);
			width = atoi(argv[i]);
			i++; assert(i < argc);
			height = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-output")) {
			i++; assert(i < argc);
			output_file = argv[i];
		}
		else if (!strcmp(argv[i], "-depth")) {
			i++; assert(i < argc);
			depth_min = atof(argv[i]);
			i++; assert(i < argc);
			depth_max = atof(argv[i]);
			i++; assert(i < argc);
			depth_file = argv[i];
		}else if (!strcmp(argv[i], "-normals")){
			i++; assert(i < argc);
			normal_file = argv[i];
		}else if (!strcmp(argv[i], "-shade_back")){
			shade_back = true;
		}
		else {
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}

	SceneParser *sp = new SceneParser(input_file);
	int num_lights = sp->getNumLights();
	Group *group = sp->getGroup();
	Camera *camera = sp->getCamera();
	Vec3f background_color = sp->getBackgroundColor();
	Vec3f ambient_light = sp->getAmbientLight();

	int square = width > height ? width : height;
	Image* image = new Image(square, square);
	image->SetAllPixels(background_color);
	Image* depth_image = new Image(square, square);
	depth_image->SetAllPixels(Vec3f(0, 0, 0));
	Image* normal_image = new Image(square, square);
	normal_image->SetAllPixels(Vec3f(0, 0, 0));

	float precalc = depth_max - depth_min;
	if (precalc == 0)
	{
		precalc = 1.0f;
	}

	for (int i = 0; i < square; i++)
		for (int j = 0; j < square; j++)
		{
			float x = (float)i * 2.0f / square - 1.0f;
			float y = (float)j * 2.0f / square - 1.0f;
			Ray ray = camera->generateRay(Vec2f(x, y));
			Hit* hit = new Hit(INT_MAX, sp->getMaterial(0), Vec3f(0.0f, 0.0f, 0.0f));
			float tmin = 0;
			if (group->intersect(ray, *hit, tmin)){
				Vec3f color = hit->getMaterial()->getDiffuseColor();
				Vec3f pixel = Vec3f(color.x() * ambient_light.x(), color.y() * ambient_light.y(), color.z() * ambient_light.z());
				Vec3f normal = hit->getNormal();
				if (shade_back){
					if (normal.Dot3(ray.getDirection()) > 0){
						normal = normal * -1.0f;
					}
				}
				if (normal.Dot3(ray.getDirection()) <= 0){
					Vec3f intersect_point = ray.getOrigin() + hit->getT() * ray.getDirection();
					for (int p = 0; p < num_lights; p++)
					{
						Light* light = sp->getLight(p);
						Vec3f light_direction;
						Vec3f light_color;
						light->getIllumination(intersect_point, light_direction, light_color);
						float d = light_direction.Dot3(normal);
						if (d < 0){
							d = 0;
						}
						pixel += d * Vec3f(light_color.x() * color.x(), light_color.y() * color.y(), light_color.z() * color.z());
						image->SetPixel(i, j, pixel);
						float t = hit->getT();
						if (t < depth_min){
							t = depth_min;
						}
						if (t > depth_max){
							t = depth_max;
						}
						float volumn = (depth_max - t) / precalc;
						depth_image->SetPixel(i, j, Vec3f(volumn, volumn, volumn));
						Vec3f normal_fab = Vec3f(fabs(normal.x()), fabs(normal.y()), fabs(normal.z()));
						normal_image->SetPixel(i, j, normal_fab);
					}
				}
				else{
					image->SetPixel(i, j, Vec3f(0.0f, 0.0f, 0.0f));
				}
			}
		}

	if (output_file != NULL){
		image->SaveTGA(output_file);
	}
	if (depth_file != NULL){
		depth_image->SaveTGA(depth_file);
	}
	if (normal_file != NULL){
		normal_image->SaveTGA(normal_file);
	}

	return 0;
}