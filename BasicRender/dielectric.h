#pragma once
#include "material.h"

namespace sparrow {
	class Dielectric :public Material {
	public:
		double refIdx;

		Dielectric(Float ri) :refIdx(ri) {}
		virtual bool scatter(const RRay& rIn, const HitRecord& rec, Color& attenuation, RRay& scattered, Float& pdf)
			const override {
			attenuation = Color(1.0, 1.0, 1.0);
			Float etai = rec.frontFace ? (1.0 / refIdx) : refIdx;
			Vector3f unitDirection = Normalize(rIn.direction());

			Float cosTheta = fmin(Dot(-unitDirection, rec.normal), 1.0);
			Float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
			if (etai * sinTheta > 1.0) {
				Vector3f reflected = Reflect(unitDirection, rec.normal);
				scattered = RRay(rec.p, reflected);
				return true;
			}
			
			double reflectProb = Schlick(cosTheta, etai);
			if (RandomFloat() < reflectProb) {
				Vector3f reflected = Reflect(unitDirection, rec.normal);
				scattered = RRay(rec.p, reflected);
				return true;
			}
			Vector3f refracted = Refract(unitDirection, rec.normal, etai);
			scattered = RRay(rec.p, refracted);
			return true;
		}
		/*virtual bool scatter(const RRay& rIn, const HitRecord& rec, Color& attenuation, RRay& scattered)
			const override {
			attenuation = Color(1.0, 1.0, 1.0);
			Float etai = rec.frontFace ? (1.0 / refIdx) : refIdx;
			Vector3f unitDirection = Normalize(rIn.direction());

			Float cosTheta = fmin(Dot(-unitDirection, rec.normal), 1.0);
			Float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
			if (etai * sinTheta > 1.0) {
				Vector3f reflected = Reflect(unitDirection, rec.normal);
				scattered = RRay(rec.p, reflected);
				return true;
			}

			double reflectProb = Schlick(cosTheta, etai);
			if (RandomFloat() < reflectProb) {
				Vector3f reflected = Reflect(unitDirection, rec.normal);
				scattered = RRay(rec.p, reflected);
				return true;
			}
			Vector3f refracted = Refract(unitDirection, rec.normal, etai);
			scattered = RRay(rec.p, refracted);
			return true;
		}*/
	};
}
