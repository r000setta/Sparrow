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
		Float u;
		Float v;
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

	class Translate :public Hittable {
	public:
		shared_ptr<Hittable> ptr;
		Vector3f offset;
	public:
		Translate(shared_ptr<Hittable> p, const Vector3f& displacement)
			:ptr(p), offset(displacement) {}
		virtual bool hit(const RRay& r, double tmin, double tmax, HitRecord& rec) const override;
	};

	class RotateY :public Hittable {
	public:
		shared_ptr<Hittable> ptr;
		Float sinTheta;
		Float cosTheta;
		bool hasBox;
	};
}
