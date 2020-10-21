#pragma once

#include "hittable.h"

namespace sparrow {
	struct HitRecord;

	class Material {
	public:
		using ptr = std::shared_ptr<Material>;
		virtual bool scatter(const RRay& rIn, const HitRecord& rec, Color& alb, RRay& scattered, Float& pdf)
			const {
			return false;
		}

		virtual bool scatter(const RRay& rIn, const HitRecord& rec, Color& alb, RRay& scattered) const {
			return false;
		}
		/*virtual bool scatter(const RRay& rIn, const HitRecord& rec, Color& attenuation, RRay& scattered)
			const {
			return false;
		}*/
		virtual Float scatteringPDF(const RRay& irIn, const HitRecord& rec, const RRay& scattered) const {
			return 0;
		}
		virtual Color emitted(Float u, Float v, const Point3f& p) const {
			return Color(0, 0, 0);
		}
	};

	inline Float Schlick(Float cosine, Float refIdx) {
		auto r0 = (1 - refIdx) / (1 + refIdx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}

	
}
