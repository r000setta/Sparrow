#pragma once

#include "ray.h"
#include "material.h"
#include <memory>

using std::shared_ptr;

namespace sparrow {
	class Material;

	struct HitRecord {
		Point3f p;
		Vector3f normal;
		Float t;
		shared_ptr<Material> matPtr;
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
