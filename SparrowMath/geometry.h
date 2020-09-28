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

		explicit Vector2(const Point2<T>& p) :x(p.x), y(p.y) {
			DCHECK(!HasNaNs());
		}

		explicit Vector2(const Point3<T>& p) :x(p.x), y(p.y) {
			DCHECK(!HasNaNs());
		}

		Vector2<T>& operator=(const Vector2<T>& v) {
			DCHECK(!v.HasNaNs());
			x = v.x;
			y = v.y;
			return *this;
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

		T operator[](int i) const {
			DCHECK(i >= 0 && i <= 1);
			if (i == 0) return x;
			return y;
		}

		T& operator[](int i) {
			DCHECK(i >= 0 && i <= 1);
			if (i == 0) return x;
			return y;
		}

		Vector2<T> operator-() const { return Vector2<T>(-x, -y); }

		Float LengthSquared() const { return x * x + y * y; }
		Float Length() const { return std::sqrt(LengthSquared()); }
	};

	template<typename T>
	inline std::ostream& operator<<(std::ostream& os, const Vector2<T>& v) {
		os << "[" << v.x << "," << v.y << "]";
		return os;
	}

	template<typename T>
	class Vector3 {
	public:
		T x, y, z;

		Vector3() :x(0), y(0), z(0) {}
		Vector3(T x, T y, T z) :x(x), y(y), z(z) { DCHECK(!HasNaNs()); }
		bool HasNaNs() const { return isNaN(x) || isNaN(y) || isNaN(z); }
		explicit Vector3(const Point3<T>& p) :x(p.x), y(p.y), z(p.z) {
			DCHECK(!HasNaNs());
		}

		Vector3(const Vector3<T>& v) {
			DCHECK(!v.HasNaNs());
			x = v.x;
			y = v.y;
			z = v.z;
		}

		Vector3<T>& operator=(const Vector3<T>& v) {
			DCHECK(!v.HasNaNs());
			x = v.x;
			y = v.y;
			z = v.z;
			return *this;
		}

		Vector3<T> operator+(const Vector3<T>& v) const {
			DCHECK(!v.HasNaNs());
			return Vector3(x + v.x, y + v.y, z + v.z);
		}

		Vector3<T>& operator+=(const Vector3<T>& v) {
			DCHECK(!v.HasNaNs());
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		Vector3<T> operator-(const Vector3<T>& v) const {
			DCHECK(!v.HasNaNs());
			return Vector3(x - v.x, y - v.y, z - v.z);
		}

		Vector3<T>& operator-=(const Vector3<T>& v) {
			DCHECK(!v.HasNaNs());
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		bool operator==(const Vector3<T>& v) const {
			return x == v.x && y == v.y && z == v.z;
		}

		bool operator!=(const Vector3<T>& v) const {
			return x != v.z || y != v.y || z != v.z;
		}

		template <typename U>
		Vector3<T> operator*(U s) const {
			return Vector3<T>(s * x, s * y, s * z);
		}
		template <typename U>
		Vector3<T>& operator*=(U s) {
			DCHECK(!isNaN(s));
			x *= s;
			y *= s;
			z *= s;
			return *this;
		}
		template <typename U>
		Vector3<T> operator/(U f) const {
			CHECK_NE(f, 0);
			Float inv = (Float)1 / f;
			return Vector3<T>(x * inv, y * inv, z * inv);
		}

		template <typename U>
		Vector3<T>& operator/=(U f) {
			CHECK_NE(f, 0);
			Float inv = (Float)1 / f;
			x *= inv;
			y *= inv;
			z *= inv;
			return *this;
		}

		T operator[](int i) const {
			DCHECK(i >= 0 && i <= 2);
			if (i == 0) return x;
			if (i == 1) return y;
			return z;
		}

		T& operator[](int i) {
			DCHECK(i >= 0 && i <= 2);
			if (i == 0) return x;
			if (i == 1) return y;
			return z;
		}

		Vector3<T> operator-() const { return Vector3<T>(-x, -y, -z); }
		Float LengthSquared() const { return x * x + y * y + z * z; }
		Float Length() const {return std::sqrt(LengthSquared()); }
	};

	template<typename T>
	inline std::ostream& operator<<(std::ostream& os, const Vector3<T>& v) {
		os << "[" << v.x << "," << v.y << "," << v.z << "]";
		return os;
	}

	using Vector2f = Vector2<Float>;
	using Vector2i = Vector2<int>;
	using Vector3f = Vector3<Float>;
	using Vector3i = Vector3<int>;

	template<typename T>
	class Point2 {
	public:
		T x, y;

		Point2() :x(0), y(0) {}
		Point2(T xx, T yy) :x(xx), y(yy) {}
		
		bool HasNaNs() const { return isNaN(x) || isNaN(y); }

		explicit Point2(const Point3<T>& p) :x(p.x), y(p.y) {
			DCHECK(!HasNaNs());
		}
		
		template<typename U>
		explicit Point2(const Point2<U>& p) {
			x = static_cast<T>(p.x);
			y = static_cast<T>(p.y);
			DCHECK(!HasNaNs());
		}

		template<typename U>
		explicit Point2(const Vector2<U>& p) {
			x = static_cast<T>(p.x);
			y = static_cast<T>(p.y);
			DCHECK(!HasNaNs());
		}

		template<typename U>
		explicit operator Vector2<U>() const {
			return Vector2<U>(x, y);
		}

		Point2(const Point2<T>& p) {
			DCHECK(!p.HasNaNs());
			x = p.x;
			y = p.y;
		}

		Point2<T>& operator=(const Point2<T>& p) {
			DCHECK(!p.HasNaNs());
			x = p.x;
			y = p.y;
			return *this;
		}

		Point2<T> operator+(const Vector2<T>& v) const {
			DCHECK(!v.HasNaNs());
			return Point2<T>(x + v.x, y + v.y);
		}

		Point2<T>& operator+=(const Vector2<T>& v) {
			DCHECK(!v.HasNaNs());
			x += v.x;
			y += v.y;
			return *this;
		}
		Vector2<T> operator-(const Point2<T>& p) const {
			DCHECK(!p.HasNaNs());
			return Vector2<T>(x - p.x, y - p.y);
		}

		Point2<T> operator-(const Vector2<T>& v) const {
			DCHECK(!v.HasNaNs());
			return Point2<T>(x - v.x, y - v.y);
		}
		Point2<T> operator-() const { return Point2<T>(-x, -y); }
		Point2<T>& operator-=(const Vector2<T>& v) {
			DCHECK(!v.HasNaNs());
			x -= v.x;
			y -= v.y;
			return *this;
		}
		Point2<T>& operator+=(const Point2<T>& p) {
			DCHECK(!p.HasNaNs());
			x += p.x;
			y += p.y;
			return *this;
		}
		Point2<T> operator+(const Point2<T>& p) const {
			DCHECK(!p.HasNaNs());
			return Point2<T>(x + p.x, y + p.y);
		}
		template <typename U>
		Point2<T> operator*(U f) const {
			return Point2<T>(f * x, f * y);
		}
		template <typename U>
		Point2<T>& operator*=(U f) {
			x *= f;
			y *= f;
			return *this;
		}
		template <typename U>
		Point2<T> operator/(U f) const {
			CHECK_NE(f, 0);
			Float inv = (Float)1 / f;
			return Point2<T>(inv * x, inv * y);
		}
		template <typename U>
		Point2<T>& operator/=(U f) {
			CHECK_NE(f, 0);
			Float inv = (Float)1 / f;
			x *= inv;
			y *= inv;
			return *this;
		}
		T operator[](int i) const {
			DCHECK(i >= 0 && i <= 1);
			if (i == 0) return x;
			return y;
		}

		T& operator[](int i) {
			DCHECK(i >= 0 && i <= 1);
			if (i == 0) return x;
			return y;
		}
		bool operator==(const Point2<T>& p) const { return x == p.x && y == p.y; }
		bool operator!=(const Point2<T>& p) const { return x != p.x || y != p.y; }
		
	};

	template<typename T>
	inline std::ostream& operator<<(std::ostream& os, const Point2<T>& v) {
		os << "[" << v.x << "," << v.y << "]";
		return os;
	}

	template<typename T>
	class Point3 {
	public:
		Point3() :x(0), y(0), z(0) {}
		Point3(T xx, T yy, T zz) :x(xx), y(yy), z(zz) {}

		bool HasNaNs() const { return isNaN(x) || isNaN(y) || isNaN(z); }

		template<typename U>
		explicit Point3(const Point3<U>& p) :x(static_cast<T>(p.x)),
			y(static_cast<T>(p.y)), z(static_cast<T>(p.z)) {
			DCHECK(!HasNaNs());
		}

		template<typename U>
		explicit operator Vector3<U>() const {
			return Vector3<U>(x, y, z);
		}

		Point3(const Point3<T>& p) {
			DCHECK(!p.HasNaNs());
			x = p.x;
			y = p.y;
			z = p.z;
		}

		Point3<T>& operator=(const Point3<T>& p) {
			DCHECK(!p.HasNaNs());
			x = p.x;
			y = p.y;
			z = p.z;
			return *this;
		}
		Point3<T> operator+(const Vector3<T>& v) const {
			DCHECK(!v.HasNaNs());
			return Point3<T>(x + v.x, y + v.y, z + v.z);
		}
		Point3<T>& operator+=(const Vector3<T>& v) {
			DCHECK(!v.HasNaNs());
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}
		Vector3<T> operator-(const Point3<T>& p) const {
			DCHECK(!p.HasNaNs());
			return Vector3<T>(x - p.x, y - p.y, z - p.z);
		}
		Point3<T> operator-(const Vector3<T>& v) const {
			DCHECK(!v.HasNaNs());
			return Point3<T>(x - v.x, y - v.y, z - v.z);
		}
		Point3<T>& operator-=(const Vector3<T>& v) {
			DCHECK(!v.HasNaNs());
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}
		Point3<T>& operator+=(const Point3<T>& p) {
			DCHECK(!p.HasNaNs());
			x += p.x;
			y += p.y;
			z += p.z;
			return *this;
		}
		Point3<T> operator+(const Point3<T>& p) const {
			DCHECK(!p.HasNaNs());
			return Point3<T>(x + p.x, y + p.y, z + p.z);
		}
		template <typename U>
		Point3<T> operator*(U f) const {
			return Point3<T>(f * x, f * y, f * z);
		}
		template <typename U>
		Point3<T>& operator*=(U f) {
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}
		template <typename U>
		Point3<T> operator/(U f) const {
			CHECK_NE(f, 0);
			Float inv = (Float)1 / f;
			return Point3<T>(inv * x, inv * y, inv * z);
		}
		template <typename U>
		Point3<T>& operator/=(U f) {
			CHECK_NE(f, 0);
			Float inv = (Float)1 / f;
			x *= inv;
			y *= inv;
			z *= inv;
			return *this;
		}
		T operator[](int i) const {
			DCHECK(i >= 0 && i <= 2);
			if (i == 0) return x;
			if (i == 1) return y;
			return z;
		}

		T& operator[](int i) {
			DCHECK(i >= 0 && i <= 2);
			if (i == 0) return x;
			if (i == 1) return y;
			return z;
		}
		bool operator==(const Point3<T>& p) const {
			return x == p.x && y == p.y && z == p.z;
		}
		bool operator!=(const Point3<T>& p) const {
			return x != p.x || y != p.y || z != p.z;
		}

		Point3<T> operator-() const { return Point3<T>(-x, -y, -z); }

		T x, y, z;
	};

	template <typename T>
	inline std::ostream& operator<<(std::ostream& os, const Point3<T>& v) {
		os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
		return os;
	}

	using Point2f = Point2<Float>;
	using Point2i = Point2<int>;
	using Point3f = Point3<Float>;
	using Point3i = Point3<int>;

	//Vector3
	template<typename T>
	Vector3<T> Abs(const Vector3<T>& v) {
		return Vector3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
	}

	template<typename T>
	inline T Dot(const Vector3<T>& v1, const Vector3<T>& v2) {
		DCHECK(!v1.HasNaNs() && !v2.HasNaNs());
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	template<typename T>
	inline T AbsDot(const Vector3<T>& v1, const Vector3<T>& v2) {
		DCHECK(!v1.HasNaNs && !v2.HasNaNs());
		return std::abs(Dot(v1, v2));
	}

	template<typename T>
	inline Vector3<T> Cross(const Vector3<T>& v1, const Vector3<T>& v2) {
		DCHECK(!v1.HasNaNs() && !v2.HasNaNs());
		return Vector3<T>((v1.y * v2.z) - (v1.z * v2.y), (v1.z * v2.x) - (v1.x * v2.z),
			(v1.x * v2.y) - (v1.y * v2.x));
	}

	template<typename T>
	inline Vector3<T> Normalize(const Vector3<T>& v) {
		return v / v.Length();
	}

	template<typename T>
	T MinComponent(const Vector3<T>& v) {
		return std::fmin(v.x, std::fmin(v.y, v.z));
	}

	template<typename T>
	T MaxComponent(const Vector3<T>& v) {
		return std::fmax(v.x, std::fmax(v.y, v.z));
	}

	//Vector2
	template<typename T>
	Vector2<T> Abs(const Vector2<T>& v) {
		return Vector2<T>(std::abs(v.x), std::abs(v.y));
	}

	template<typename T>
	inline T Dot(const Vector2<T>& v1, const Vector2<T>& v2) {
		DCHECK(!v1.HasNaNs() && !v2.HasNaNs());
		return v1.x * v2.x + v1.y * v2.y;
	}

	template<typename T>
	inline T AbsDot(const Vector2<T>& v1, const Vector2<T>& v2) {
		DCHECK(!v1.HasNaNs && !v2.HasNaNs());
		return std::abs(Dot(v1, v2));
	}

	template<typename T>
	inline Vector2<T> Normalize(const Vector2<T>& v) {
		return v / v.Length();
	}
}
