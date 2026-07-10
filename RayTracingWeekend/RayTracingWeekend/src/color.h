#pragma once

#include "vec3.h"
#include "interval.h"

using color = vec3;

inline double linear_to_gamma(double linear) {
	if (linear > 0) {
		return std::sqrt(linear);
	}
	return 0;
}

inline void write_color(std::ostream& out, const color& pixel) {
	auto r = pixel.x();
	auto g = pixel.y();
	auto b = pixel.z();

	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	static const interval intensity(0.000, 0.999);
	int red = int(256 * intensity.clamp(r));
	int green = int(256 * intensity.clamp(g));
	int blue = int(256 * intensity.clamp(b));

	out << red << ' ' << green << ' ' << blue << '\n';
}

inline color lerp(double v, color a, color b) {
	return a + v * (b - a);
}