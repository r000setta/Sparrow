#pragma once

#include "hittable.h"

namespace sparrow {
	class MovingSphere :public Hittable {
	public:
		Point3f center0, center1;
		Float time0, time1;
		Float radius;
		unsigned int material;

		MovingSphere() {}
		MovingSphere(Point3f cen0, Point3f cen1,
			Float t0, Float t1,
			Float r, unsigned int m)
			: center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), material(m) {};
		virtual bool hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const override;
		Point3f center(Float time) const {
			return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
		}
	};
}