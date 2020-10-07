#pragma once

#include "geometry.h"

namespace sparrow {
	class ONB {
	public:
		Vector3f axis[3];
	public:
		ONB() {}
		inline Vector3f operator[](int i) { return axis[i]; }
		Vector3f u() const { return axis[0]; }
		Vector3f v() const { return axis[1]; }
		Vector3f w() const { return axis[2]; }
		Vector3f local(Float a, Float b, Float c) const {
			return a * u() + b * v() + c * w();
		}

		Vector3f local(const Vector3f& a) const {
			return a.x * u() + a.y * v() + a.z * w();
		}

		void BuildFromW(const Vector3f& n) {
			axis[2] = Normalize(n);
			Vector3f a = (fabs(w().x) > 0.9) ? Vector3f(0, 1, 0) : Vector3f(1, 0, 0);
			axis[1] = Normalize(Cross(w(), a));
			axis[0] = Cross(w(), v());
		}
	};
}
