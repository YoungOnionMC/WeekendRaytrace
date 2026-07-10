#pragma once

#include "hittable.h"

#include <vector>
#include <memory>

class hittable_list : public hittable {
public:
	std::vector<std::shared_ptr<hittable>> objects;

	hittable_list() {}
	hittable_list(std::shared_ptr<hittable> object) { add(object); }

	void clear() {
		objects.clear();
	}

	void add(std::shared_ptr<hittable> object) {
		objects.push_back(object);
	}

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		hit_record temp_rec;
		bool hit_anything = false;
		double closest = ray_t.max;

		for (const auto& object : objects) {
			if (object->hit(r, interval(ray_t.min, closest), temp_rec)) {
				hit_anything = true;
				closest = temp_rec.t;
				rec = temp_rec;
			}
		}

		return hit_anything;
	}
};
