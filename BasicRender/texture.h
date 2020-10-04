#pragma once

#include "geometry.h"
#include "perlin.h"

namespace sparrow {
	class Texture {
	public:
		virtual Color value(Float u, Float v, const Point3f& p) const = 0;
	};

	class SolidColor :public Texture {
	private:
		Color colorValue;
	public:
		SolidColor() {}
		SolidColor(Color c) :colorValue(c) {}
		SolidColor(Float red, Float green, Float blue)
			:SolidColor(Color(red, green, blue)) {}
		virtual Color value(Float u, Float v, const Point3f& p) const override {
			return colorValue;
		}
	};

	class CheckerTexture :public Texture {
	public:
		shared_ptr<Texture> odd;
		shared_ptr<Texture> even;
	public:
		CheckerTexture() {}
		CheckerTexture(shared_ptr<Texture> t0, shared_ptr<Texture> t1)
			:even(t0), odd(t1) {}
		CheckerTexture(Color c1, Color c2)
			:even(make_shared<SolidColor>(c1)), odd(make_shared<SolidColor>(c2)) {}
		virtual Color value(Float u, Float v, const Point3f& p) const override {
			auto sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
			if (sines < 0)
				return odd->value(u, v, p);
			else
				return even->value(u, v, p);
		}
	};

	class NoiseTexture :public Texture {
	public:
		Perlin noise;
		Float scale = 1.0;
	public:
		NoiseTexture() {}
		NoiseTexture(Float sc) :scale(sc) {}
		virtual Color value(Float u, Float v, const Point3f& p) const override {
			return Color(1, 1, 1) * noise.noise(scale * p);
		}
	};
}