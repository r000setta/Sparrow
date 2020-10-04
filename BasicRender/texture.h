#pragma once

#include "geometry.h"
#include "perlin.h"
#include "spstb_image.h"

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
			return Color(1, 1, 1) * 0.5 * (1.0 + noise.noise(scale * p));
		}
	};

	class ImageTexture :public Texture {
	private:
		unsigned char* data;
		int width, height;
		int bytesPerLine;
	public:
		const static int bytesPerPixel = 3;
		ImageTexture()
			:data(nullptr), width(0), height(0), bytesPerLine(0) {}
		ImageTexture(const char* filename) {
			auto componentsPP = bytesPerPixel;
			data = stbi_load(
				filename, &width, &height, &componentsPP, componentsPP);
			if (!data) {
				std::cerr << "ERROR:couldn't load texture image:" << filename << std::endl;
				width = height = 0;
			}
			bytesPerLine = bytesPerPixel * width;
		}

		~ImageTexture() {
			delete data;
		}

		virtual Color value(Float u, Float v, const Point3f& p) const override {
			if (data == nullptr)
				return Color(0, 1, 1);
			u = Clamp(u, 0.0, 1.0);
			v = 1.0 - Clamp(v, 0.0, 1.0);
			auto i = static_cast<int>(u * width);
			auto j = static_cast<int>(v * height);

			if (i >= width) i = width - 1;
			if (j >= height) j = height - 1;

			constexpr auto colorScale = 1.0 / 255.0;
			auto pixel = data + j * bytesPerLine + i * bytesPerPixel;

			return Color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
		}
	};
}