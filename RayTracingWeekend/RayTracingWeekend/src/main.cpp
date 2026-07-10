#include <iostream>
#include <fstream>

#include "common.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "camera.h"

inline color random_color() {
	return color(random_double(), random_double(), random_double());
}

int main() {
	hittable_list world;

	auto groundMaterial = std::make_shared<lambertian>(color(0.14, 0.14, 0.18));
	auto albedoMaterial = std::make_shared<lambertian>(color(0.2, 0.2, 0.76));
	auto metalMaterial = std::make_shared<metal>(color(0.9, 0.85, 0.83), 0.13);
	auto dielectricMaterial = std::make_shared<dielectric>(1.5);

	world.add(std::make_shared<sphere>(point3(0, -1000.0, -1), 1000, groundMaterial));

	for (int i = 0; i < 121; i++) {

		std::shared_ptr<material> mat;
		double size = random_double(0.15, 0.4);
		double choice = random_double();
		point3 pos = point3((i % 11 + (random_double() * 0.3 - 0.15)) - 5, size, (i / 11 + (random_double() * 0.3 - 0.15)) - 5);
		if (choice < 0.6) {
			color albedo = random_color();
			mat = std::make_shared<lambertian>(albedo);
		}
		else if (choice < 0.9) {
			color albedo = random_color();
			double fuzz = random_double(0, 0.4);
			mat = std::make_shared<metal>(albedo, fuzz);
		}
		else {
			mat = std::make_shared<dielectric>(1.5);
		}

		world.add(std::make_shared<sphere>(pos, size, mat));
	}

	world.add(std::make_shared<sphere>(point3(0, 1, 0), 1, albedoMaterial));
	world.add(std::make_shared<sphere>(point3(5, 1, 0), 1, dielectricMaterial));
	world.add(std::make_shared<sphere>(point3(-5, 1, 0), 1, metalMaterial));
	

	camera cam;
	cam.aspect_ratio = 16.0f / 9.0f;
	cam.image_width = 1200;
	cam.samples_per_pixel = 10;
	cam.max_depth = 5;

	cam.vfov = 60;
	cam.lookfrom = point3(-7, 2, 4);
	cam.lookat = point3(0, 0, 0);
	cam.vup = vec3(0, 1, 0);

	cam.defocus_angle = 0.6;
	cam.focus_dist = 10.0;

	cam.renderToFile(world);
}