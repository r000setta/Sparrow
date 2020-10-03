#pragma once

#include "material.h"

namespace sparrow {
	class Lambertian :public Material {
	public:
		Color albedo;
	public:
		Lambertian(const Color& a) :albedo(a) {}
		virtual bool scatter(const RRay& rIn, const HitRecord& rec, Color& attenuation, RRay& scattered)
			const override {
			Vector3f scatterDirection = rec.normal + RandomUnitVector<Float>();
			scattered = RRay(rec.p, scatterDirection, rIn.time());
			attenuation = albedo;
			return true;
		}
	};
}