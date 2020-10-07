#pragma once

#include "material.h"

namespace sparrow {
	class Metal :public Material {
	public:
		Color albedo;
		Float fuzz;
	public:
		Metal(const Color& a, Float f) :albedo(a), fuzz(f < 1 ? f : 1) {}
		virtual bool scatter(const RRay& rIn, const HitRecord& rec, Color& attenuation, RRay& scattered,Float& pdf)
			const override {
			Vector3f reflected = Reflect(Normalize(rIn.direction()), rec.normal);
			scattered = RRay(rec.p, reflected + fuzz * RandomInUnitSphere<Float>());
			attenuation = albedo;
			return (Dot(scattered.direction(), rec.normal) > 0);
		}

		/*virtual bool scatter(const RRay& rIn, const HitRecord& rec, Color& attenuation, RRay& scattered)
			const override {
			Vector3f reflected = Reflect(Normalize(rIn.direction()), rec.normal);
			scattered = RRay(rec.p, reflected + fuzz * RandomInUnitSphere<Float>());
			attenuation = albedo;
			return (Dot(scattered.direction(), rec.normal) > 0);
		}*/
	};
}