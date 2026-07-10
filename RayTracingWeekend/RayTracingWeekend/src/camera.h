#pragma once

#include "common.h"
#include "interval.h"
#include "random.h"
#include "material.h"
#include <iostream>
#include <fstream>

class camera {
public:
	float aspect_ratio = 1.0;
	int image_width = 100;
	int samples_per_pixel = 10;
	int max_depth = 10;

	double vfov = 90;
	point3 lookfrom = point3(0, 0, 0);
	point3 lookat = point3(0, 0, -1);
	vec3 vup = vec3(0, 1, 0);

	double defocus_angle = 0;
	double focus_dist = 10;

	void renderToFile(const hittable& world) {
		init();

		std::ofstream rayTraceFile("raytrace.ppm");
		rayTraceFile << "P3\n" << image_width << ' ' << image_height << "\n255\n";

		for (size_t j = 0; j < image_height; j++) {
			std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
			for (size_t i = 0; i < image_width; i++) {
				color pixel_color(0, 0, 0);
				for (size_t sample = 0; sample < samples_per_pixel; sample++) {
					ray r = get_ray(i, j);
					pixel_color += ray_color(r, max_depth, world);
				}
				write_color(rayTraceFile, pixel_samples_scale * pixel_color);
			}
		}
		rayTraceFile.close();
	}

private:
	int    image_height;   // Rendered image height
	double pixel_samples_scale;
	point3 center;         // Camera center
	point3 pixel00_loc;    // Location of pixel 0, 0
	vec3   pixel_delta_u;  // Offset to pixel to the right
	vec3   pixel_delta_v;  // Offset to pixel below
	vec3	u, v, w;
	vec3	defocus_disk_u;
	vec3	defocus_disk_v;

	void init() {
		image_height = int(image_width / aspect_ratio);
		image_height = std::max(1, image_height);

		pixel_samples_scale = 1.0 / samples_per_pixel;

		center = lookfrom;

		// viewport dimensions
		double theta = degrees_to_radians(vfov);
		double h = std::tan(theta / 2);
		double viewport_height = 2.0 * h * focus_dist;
		double viewport_width = viewport_height * double(image_width) / double(image_height);

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
	
		// viewport edges
		vec3 viewport_u = viewport_width * u;
		vec3 viewport_v = viewport_height * -v;

		// delta vectors for pixel to pixel
		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		// location of upper left pixel
		vec3 viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
		pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

		double defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
		defocus_disk_u = u * defocus_radius;
		defocus_disk_v = v * defocus_radius;
	}

	ray get_ray(int i, int j) const {
		// Construct a camera ray originating from the defocus disk and directed at a randomly sampled point around the pixel location i, j.
		vec3 offset = sample_square();
		vec3 pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

		vec3 ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
		vec3 ray_direction = pixel_sample - ray_origin;

		return ray(ray_origin, ray_direction);
	}

	vec3 sample_square() const {
		return vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}

	point3 defocus_disk_sample() const {
		vec3 p = random_in_unit_disk();
		return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
	}

	color ray_color(const ray& r, int depth, const hittable& world) {
		if (depth <= 0) {
			return color(0, 0, 0);
		}

		hit_record rec;
		if (world.hit(r, interval(0.001, infinity), rec)) {
			ray scattered;
			color attenuation;
			if (rec.mat->scatter(r, rec, attenuation, scattered)) {
				return attenuation * ray_color(scattered, depth - 1, world);
			}
		}

		vec3 unit_dir = unit_vector(r.direction());
		double a = 0.5 * (unit_dir.y() + 1.0);
		return lerp(a, color(0.5, 0.7, 1.0), color(1.0, 1.0, 1.0));
	}

};