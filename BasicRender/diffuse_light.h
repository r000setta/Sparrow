#pragma once
#include "texture.h"
#include "material.h"

using std::make_shared;

namespace sparrow {
	class DiffuseLight :public Material {
	public:
		shared_ptr<Texture> emit;
	public:
		DiffuseLight(shared_ptr<Texture> a) :emit(a) {}
		DiffuseLight(Color c) :emit(make_shared<SolidColor>(c)) {}
		virtual bool scatter(const RRay& rIn, const HitRecord& rec, Color& attenuation, RRay& scattered)
			const override {
			return false;
		}

		virtual Color emitted(Float u, Float v, const Point3f& p) const override {
			return emit->value(u, v, p);
		}
	};
}