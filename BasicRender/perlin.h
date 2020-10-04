#pragma once

#include "geometry.h"

namespace sparrow {
	class Perlin {
	public:
		Perlin() {
			ranFloat = new Float[pointCount];
			for (int i = 0; i < pointCount; ++i) {
				ranFloat[i] = RandomFloat();
			}
			permX = perlinGen();
			permY = perlinGen();
			permZ = perlinGen();
		}

		~Perlin() {
			delete[] ranFloat;
			delete[] permX;
			delete[] permY;
			delete[] permZ;
		}

		Float noise(const Point3f& p) const {
			auto u = p.x - floor(p.x);
			auto v = p.y - floor(p.y);
			auto w = p.z - floor(p.z);

			u = u * u * (3 - 2 * u);
			v = v * v * (3 - 2 * v);
			w = w * w * (3 - 2 * w);

			auto i = static_cast<int>(floor(p.x));
			auto j = static_cast<int>(floor(p.y));
			auto k = static_cast<int>(floor(p.z));
			Float c[2][2][2];

			for (int di = 0; di < 2; ++di) {
				for (int dj = 0; dj < 2; ++dj) {
					for (int dk = 0; dk < 2; ++dk) {
						c[di][dj][dk] = ranFloat[
								permX[(i + di) & 255] ^
								permY[(j + di) & 255] ^
								permZ[(k + dk) & 255]
						];
					}
				}
			}
			return TrilinearInterp(c, u, v, w);
		}
	private:
		static const int pointCount = 256;
		Float* ranFloat;
		int* permX;
		int* permY;
		int* permZ;

		static int* perlinGen() {
			auto p = new int[pointCount];
			for (int i = 0; i < Perlin::pointCount; ++i) {
				p[i] = i;
			}
			ppermute(p, pointCount);
			return p;
		}

		static void ppermute(int* p, int n) {
			for (int i = n - 1; i > 0; i--) {
				int target = RandomInt(0, i);
				std::swap(p[i], p[target]);
			}
		}

		static Float TrilinearInterp(Float c[2][2][2], Float u, Float v, Float w) {
			auto uu = u * u * (3 - 2 * u);
			auto vv = v * v * (3 - 2 * v);
			auto ww = w * w * (3 - 2 * w);
			auto accum = 0.0;

			for (int i = 0; i < 2; i++)
				for (int j = 0; j < 2; j++)
					for (int k = 0; k < 2; k++) {
						Vector3f weightV(u - i, v - j, w - k);
						auto tmp = Vector3f(c[i][j][k], c[i][j][k], c[i][j][k]);
						accum += (i * uu + (1 - i) * (1 - uu))
							* (j * vv + (1 - j) * (1 - vv))
							* (k * ww + (1 - k) * (1 - ww))
							* Dot(tmp, weightV);
					}

			return accum;
		}
	};
}