#pragma once

#include "material.h"
#include "texture.h"

namespace sparrow {
	class Lambertian :public Material {
	public:
		shared_ptr<Texture> albedo;
	public:
		Lambertian(const Color& a) :albedo(make_shared<SolidColor>(a)) {}
		Lambertian(shared_ptr<Texture> a) :albedo(a) {}

		virtual bool scatter(const RRay& rIn, const HitRecord& rec, Color& attenuation, RRay& scattered)
			const override {
			Vector3f scatterDirection = rec.normal + RandomUnitVector<Float>();
			scattered = RRay(rec.p, scatterDirection, rIn.time());
			attenuation = albedo->value(rec.u, rec.v, rec.p);
			return true;
		}
	};
}