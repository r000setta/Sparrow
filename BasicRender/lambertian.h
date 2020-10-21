#pragma once

#include "material.h"
#include "texture.h"
#include "onb.h"
#include "pdf.h"

namespace sparrow {
	class Lambertian :public Material {
	public:
		unsigned int albedo;
	public:
		using ptr = std::shared_ptr<Lambertian>;

		//Lambertian(const Color& a) :albedo(make_shared<SolidColor>(a)) {}
		Lambertian(unsigned int a) :albedo(a) {}

		virtual bool scatter(const RRay& rIn, const HitRecord& rec, Color& alb, RRay& scattered,Float& pdf)
			const override {
			//ONB uvw;
			//uvw.BuildFromW(rec.normal);
			//auto direction = uvw.local(RandomCosineDirection());
			//scattered = RRay(rec.p, Normalize(direction), rIn.time());
			//auto mgr = TextureMgr::getSingleton();
			//attenuation = mgr->getTexture(albedo)->value(rec.u, rec.v, rec.p);
			////attenuation = albedo->value(rec.u, rec.v, rec.p);
			//pdf = Dot(uvw.w(), scattered.direction()) / Pi;
			///*Vector3f scatterDirection = rec.normal + RandomUnitVector<Float>();
			//scattered = RRay(rec.p, scatterDirection, rIn.time());
			//attenuation = albedo->value(rec.u, rec.v, rec.p);*/
			//return true;

			/*auto direction = rec.normal + RandomUnitVector<Float>();
			scattered = RRay(rec.p, Normalize(direction), rIn.time());
			alb = TextureMgr::getSingleton()->getTexture(albedo)->value(rec.u, rec.v, rec.p);
			std::cout << "albedo:" << alb << std::endl;
			pdf = Dot(rec.normal, scattered.direction());
			return true;*/

			Vector3f scatteredDir = rec.normal + RandomUnitVector<Float>();
			scattered = RRay(rec.p, Normalize(scatteredDir), rIn.time());
			alb = TextureMgr::getSingleton()->getTexture(albedo)->value(rec.u, rec.v, rec.p);
			pdf = Dot(rec.normal, scattered.direction()) / Pi;
			//std::cout << "albedo:" << alb << std::endl;
			return true;
		}

		virtual bool scatter(const RRay& rIn, const HitRecord& rec, Color& alb, RRay& scattered) const {
			Vector3f scatterDir = rec.normal + RandomUnitVector<Float>();
			scattered = RRay(rec.p, scatterDir);
			alb = TextureMgr::getSingleton()->getTexture(albedo)->value(rec.u, rec.v, rec.p);
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