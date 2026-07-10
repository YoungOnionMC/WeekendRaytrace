#pragma once

#include <cmath>
#include <limits>
#include <cstdlib>
#include <random>
#include "color.h"
#include "ray.h"
#include "vec3.h"

inline constexpr double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}