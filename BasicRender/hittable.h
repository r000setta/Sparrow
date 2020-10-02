#pragma once

#include "ray.h"

namespace sparrow {
	struct HitRecord {
		Point3f p;
		Vector3f normal;
		Float t;
		bool frontFace;

		inline void setFrontNormal(const RRay& r, const Vector3f& outward) {
			frontFace = Dot(r.direction(), outward) < 0;
			normal = frontFace ? outward : -outward;
		}
	};

	class Hittable {
	public:
		virtual bool hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const = 0;
	};
}
