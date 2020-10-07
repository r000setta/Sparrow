#pragma once

#include "geometry.h"
#include "onb.h"
#include "hittable.h"

namespace sparrow {
	class PDF {
	public:
		virtual ~PDF() {}

		virtual Float value(const Vector3f& direction) const = 0;
		virtual Vector3f generate() const = 0;
	};

	inline Vector3f RandomCosineDirection() {
		auto r1 = RandomFloat();
		auto r2 = RandomFloat();
		auto z = sqrt(1 - r2);

		auto phi = 2 * Pi * r1;
		auto x = cos(phi) * sqrt(r2);
		auto y = sin(phi) * sqrt(r2);
		return Vector3f(x, y, z);
	}

	class ConsinePDF :public PDF {
	public:
		ONB uvw;

		ConsinePDF(const Vector3f& w) {
			uvw.BuildFromW(w);
		}
		virtual Float value(const Vector3f& direction) const override {
			auto cosine = Dot(Normalize(direction), uvw.w());
			return (cosine <= 0) ? 0 : cosine / Pi;
		}

		virtual Vector3f generate() const override {
			return uvw.local(RandomCosineDirection());
		}
	};

	class HittablePDF :public PDF {
	public:
		Point3f o;
		shared_ptr<Hittable> ptr;
	public:
		HittablePDF(shared_ptr<Hittable> p, const Point3f& origin) :ptr(p), o(origin) {}
		virtual Float value(const Vector3f& direction) const override {
			return ptr->PDFvalue(o, direction);
		}

		virtual Vector3f generate() const override{
			return ptr->Random(o);
		}
	};

	class MixturePDF :public PDF {
	public:
		shared_ptr<PDF> p[2];
	public:
		MixturePDF(shared_ptr<PDF> p0, shared_ptr<PDF> p1) {
			p[0] = p0;
			p[1] = p1;
		}

		virtual Float value(const Vector3f& direction) const override {
			return 0.5 * p[0]->value(direction) + 0.5 * p[1]->value(direction);
		}

		virtual Vector3f generate() const override {
			if (RandomFloat() < 0.5) return p[0]->generate();
			return p[1]->generate();
		}
	};
}