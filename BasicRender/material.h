#pragma once

#include "hittable.h"

namespace sparrow {
	struct HitRecord;

	class Material {
	public:
		virtual bool scatter(const RRay& rIn, const HitRecord& rec, Color& attenuation, RRay& scattered)
			const = 0;
	};

	inline Float Schlick(Float cosine, Float refIdx) {
		auto r0 = (1 - refIdx) / (1 + refIdx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
}
