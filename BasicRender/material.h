#pragma once

#include "hittable.h"

namespace sparrow {
	struct HitRecord;

	class Material {
	public:
		virtual bool scatter(const RRay& rIn, const HitRecord& rec, Color& attenuation, RRay& scattered)
			const = 0;
	};
}
