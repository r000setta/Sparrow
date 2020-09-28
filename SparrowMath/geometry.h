#pragma once
#include "sparrow.h"

namespace sparrow {
	template<typename T>
	inline bool isNaN(const T x) {
		return std::isnan(x);
	}

	template<>
	inline bool isNaN(const int x) {
		return false;
	}

	template<typename T>
	class Vector2 {
	public:
		T x, y;
	public:
		Vector2() :x(0), y(0) {}
		Vector2(T xx, T yy) :x(xx), y(yy) { DCHECK(!HasNaNs()); }
		bool HasNaNs() const {
			return isNaN(x) || isNaN(y);
		}

		Vector2<T> operator+(const Vector2<T>& v) const {
			DCHECK(!v.HasNaNs());
			return Vector2(x + v.x, y + v.y);
		}

		Vector2<T> operator+=(const Vector2<T>& v) {
			DCHECK(!v.HasNaNs());
			x += v.x;
			y += v.y;
			return *this;
		}

		Vector2<T> operator-(const Vector2<T>& v) const {
			DCHECK(!v.HasNaNs());
			return Vector2(x - v.x, y - v.y);
		}

		Vector2<T> operator-=(const Vector2<T>& v) {
			DCHECK(!v.HasNaNs());
			x -= v.x;
			y -= v.y;
			return *this;
		}

		bool operator==(const Vector2<T>& v) const { return x == v.x && y == v.y; }
		bool operator!=(const Vector2<T>& v) const { return x != v.x || y != v.y; }

		template<typename U>
		Vector2<T> operator*(U f) const {
			return Vector2<T>(f * x, f * y);
		}

		template<typename U>
		Vector2<T>& operator *= (U f) {
			DCHECK(!isNaN(f));
			x *= f;
			y *= f;
			return *this;
		}

		template<typename U>
		Vector2<T> operator/(U f) const {
			CHECK_NE(f, 0);
			Float inv = (Float)1 / f;
			return Vector2<T>(x * inv, y * inv);
		}

		template<typename U>
		Vector2<T>& operator/=(U f) {
			CHECK_NE(f, 0);
			Float inv = (Float)1 / f;
			x *= inv;
			y *= inv;
			return *this;
		}

		Float LengthSquared() const { return x * x + y * y; }
		Float Length() const { return std::sqrt(LengthSquared()); }
	};
}