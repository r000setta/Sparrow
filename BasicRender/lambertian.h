#pragma once

#include "material.h"
#include "texture.h"
#include "onb.h"
#include "pdf.h"

namespace sparrow {
	class Lambertian :public Material {
	public:
		shared_ptr<Texture> albedo;
	public:
		Lambertian(const Color& a) :albedo(make_shared<SolidColor>(a)) {}
		Lambertian(shared_ptr<Texture> a) :albedo(a) {}

		virtual bool scatter(const RRay& rIn, const HitRecord& rec, Color& attenuation, RRay& scattered,Float& pdf)
			const override {
			ONB uvw;
			uvw.BuildFromW(rec.normal);
			auto direction = uvw.local(RandomCosineDirection());
			scattered = RRay(rec.p, Normalize(direction), rIn.time());
			attenuation = albedo->value(rec.u, rec.v, rec.p);
			pdf = Dot(uvw.w(), scattered.direction()) / Pi;
			/*Vector3f scatterDirection = rec.normal + RandomUnitVector<Float>();
			scattered = RRay(rec.p, scatterDirection, rIn.time());
			attenuation = albedo->value(rec.u, rec.v, rec.p);*/
			return true;
		}

		/*virtual bool scatter(const RRay& rIn, const HitRecord& rec, Color& attenuation, RRay& scattered)
			const override {
			Vector3f scatterDirection = rec.normal + RandomUnitVector<Float>();
			scattered = RRay(rec.p, scatterDirection, rIn.time());
			attenuation = albedo->value(rec.u, rec.v, rec.p);
			return true;
		}*/

		virtual Float scatteringPDF(const RRay& irIn, const HitRecord& rec, const RRay& scattered) const {
			auto cosine = Dot(rec.normal, Normalize(scattered.direction()));
			return cosine < 0 ? 0 : cosine / Pi;
		}
	};
}