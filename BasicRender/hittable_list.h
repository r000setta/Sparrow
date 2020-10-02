#pragma once

#include <memory>
#include <vector>

#include "hittable.h"

using std::shared_ptr;
using std::make_shared;

namespace sparrow {
	class HittableList :public Hittable {
	public:
		std::vector<shared_ptr<Hittable>> objects;
	public:
		HittableList() {}
		HittableList(shared_ptr<Hittable> object) {
			add(object);
		}

		void add(shared_ptr<Hittable> obj) {
			objects.emplace_back(obj);
		}
		void clear() {
			objects.clear();
		}
		virtual bool hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const override;
	};
}