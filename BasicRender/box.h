#pragma once

#include "aarect.h"
#include "hittable_list.h"

namespace sparrow {
	class Box :public Hittable {
	public:
		Point3f boxMin;
		Point3f boxMax;
		HittableList sides;
	public:
		Box() {}
		Box(const Point3f& p0, const Point3f& p1, shared_ptr<Material> mat);
		virtual bool hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const override;
	};
}