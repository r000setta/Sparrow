#pragma once

#include "material.h"

namespace sparrow {
	class Metal :public Material {
	public:
		Color albedo;
	public:
		Metal(const Color& a) :albedo(a) {}
		virtual bool scatter(const RRay& rIn, const HitRecord& rec, Color& attenuation, RRay& scattered)
			const override {
			Vector3f reflected = Reflect(Normalize(rIn.direction()), rec.normal);
			scattered = RRay(rec.p, reflected);
			attenuation = albedo;
			return (Dot(scattered.direction(), rec.normal) > 0);
		}
	};
}