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

		explicit Vector3<T>(const Normal3<T>& n) : x(n.x), y(n.y), z(n.z) {
			DCHECK(!n.HasNaNs());
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

	using Color = Vector3f;

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

	template<typename T>
	class Normal3 {
	public:
		T x, y, z;
		bool HasNaNs() const { return isNaN(x) || isNaN(y) || isNaN(z); }

		Normal3() :x(0), y(0), z(0) {}
		Normal3(T xx, T yy, T zz) :x(xx), y(yy), z(zz) {
			DCHECK(!HasNaNs());
		}
		Normal3<T> operator-() const {
			return Normal3(-x, -y, -z);
		}
		Normal3<T> operator+(const Normal3<T>& n) const {
			DCHECK(!n.HasNaNs());
			return Normal3<T>(x + n.x, y + n.y, z + n.z);
		}
		Normal3<T>& operator+=(const Normal3<T>& n) {
			DCHECK(!n.HasNaNs());
			x += n.x;
			y += n.y;
			z += n.z;
			return *this;
		}
		Normal3<T> operator-(const Normal3<T>& n) const {
			DCHECK(!n.HasNaNs());
			return Normal3<T>(x - n.x, y - n.y, z - n.z);
		}

		Normal3<T>& operator-=(const Normal3<T>& n) {
			DCHECK(!n.HasNaNs());
			x -= n.x;
			y -= n.y;
			z -= n.z;
			return *this;
		}
		template<typename U>
		Normal3<T> operator*(U f) const {
			return Normal3<T>(f * x, f * y, f * z);
		}

		template <typename U>
		Normal3<T>& operator*=(U f) {
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}
		template <typename U>
		Normal3<T> operator/(U f) const {
			CHECK_NE(f, 0);
			Float inv = (Float)1 / f;
			return Normal3<T>(x * inv, y * inv, z * inv);
		}

		template <typename U>
		Normal3<T>& operator/=(U f) {
			CHECK_NE(f, 0);
			Float inv = (Float)1 / f;
			x *= inv;
			y *= inv;
			z *= inv;
			return *this;
		}
		Float LengthSquared() const { return x * x + y * y + z * z; }
		Float Length() const { return std::sqrt(LengthSquared()); }

		Normal3<T>(const Normal3<T>& n) {
			DCHECK(!n.HasNaNs());
			x = n.x;
			y = n.y;
			z = n.z;
		}

		Normal3<T>& operator=(const Normal3<T>& n) {
			DCHECK(!n.HasNaNs());
			x = n.x;
			y = n.y;
			z = n.z;
			return *this;
		}

		explicit Normal3<T>(const Vector3<T>& v) : x(v.x), y(v.y), z(v.z) {
			DCHECK(!v.HasNaNs());
		}
		bool operator==(const Normal3<T>& n) const {
			return x == n.x && y == n.y && z == n.z;
		}
		bool operator!=(const Normal3<T>& n) const {
			return x != n.x || y != n.y || z != n.z;
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
	};

	template <typename T>
	inline std::ostream& operator<<(std::ostream& os, const Normal3<T>& v) {
		os << "[ " << v.x << "," << v.y << "," << v.z << " ]";
		return os;
	}

	using Normal3f = Normal3<Float>;

	class Ray {
	public:
		Point3f o;
		Vector3f d;
		mutable Float tMax;
		Float time;
	public:
		Ray() :tMax(Infinity), time(0.f) {}
		Ray(const Point3f& o, const Vector3f& d, Float tMax = Infinity, Float time = 0.f)
			:o(o), d(d), tMax(tMax), time(time) {}
		Point3f operator()(Float t) const { return o + d * t; }
		bool HasNaNs() const { return (o.HasNaNs() || d.HasNaNs() || isNaN(tMax)); }
		friend std::ostream& operator<<(std::ostream& os, const Ray& r) {
			os << "[o=" << r.o << ",d=" << r.d << ",tMax=" << r.tMax << ",time=" << r.time << "]";
			return os;
		}
	};

	template<typename T>
	class Bounds3 {
	public:
		Point3<T> pMin, pMax;
		Bounds3() {
			T minNum = std::numeric_limits<T>::lowest();
			T maxNum = std::numeric_limits<T>::max();
			pMin = Point3<T>(maxNum, maxNum, maxNum);
			pMax = Point3<T>(minNum, minNum, minNum);
		}

		explicit Bounds3(const Point3<T>& p) :pMin(p), pMax(p) {}
		Bounds3(const Point3<T>& p1, const Point3<T>& p2) :
			pMin(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
				std::min(p1.z, p2.z)),
			pMax(std::max(p1.x, p2.x), std::max(p1.y, p2.y),
				std::max(p1.z, p2.z)) {}
		const Point3<T>& operator[](int i) const {
			DCHECK(i == 1 || i == 0);
			if (i == 0) return pMin;
			return pMax;
		}

		bool operator==(const Bounds3<T>& b) const {
			return b.pMin == pMin && b.pMax == pMax;
		}
		bool operator!=(const Bounds3<T>& b) const {
			return b.pMax != pMax || b.pMin != pMin;
		}

		Point3<T> Corner(int corner) const {
			DCHECK(corner >= 0 && corner < 8);
			return Point3<T>((*this)[(corner & 1)].x,
				(*this)[(corner & 2) ? 1 : 0].y,
				(*this)[(corner & 4) ? 1 : 0].z);
		}

		Vector3<T> Diagonal() const { return pMax - pMin; }
		T SurfaceArea() const {
			Vector3<T> d = Diagonal();
			return 2 * (d.x * d.y + d.x * d.z + d.y * d.z);
		}

		T Volume() const {
			Vector3<T> d = Diagonal();
			return d.x * d.y * d.z;
		}
		int MaximumExtent() const {
			Vector3<T> d = Diagonal();
			if (d.x > d.y && d.x > d.z)
				return 0;
			else if (d.y > d.z)
				return 1;
			else
				return 2;
		}
		Point3<T> Lerp(const Point3f& t) const {
			return Point3<T>(Lerp(t.x, pMin.x, pMax.x),
				Lerp(t.y, pMin.y, pMax.y),
				Lerp(t.z, pMin.z, pMax.z));
		}
		Vector3<T> Offset(const Point3<T>& p) const {
			Vector3<T> o = p - pMin;
			if (pMax.x > pMin.x) o.x /= pMax.x - pMin.x;
			if (pMax.y > pMin.y) o.y /= pMax.y - pMin.y;
			if (pMax.z > pMin.z) o.z /= pMax.z - pMin.z;
			return o;
		}
		void BoundingSphere(Point3<T>* center, Float* radius) const {
			*center = (pMin + pMax) / 2;
			*radius = Inside(*center, *this) ? Distance(*center, pMax) : 0;
		}
		template <typename U>
		explicit operator Bounds3<U>() const {
			return Bounds3<U>((Point3<U>)pMin, (Point3<U>)pMax);
		}
		/*bool IntersectP(const Ray& ray, Float* hitt0 = nullptr,
			Float* hitt1 = nullptr) const;
		inline bool IntersectP(const Ray& ray, const Vector3f& invDir,
			const int dirIsNeg[3]) const;*/
		friend std::ostream& operator<<(std::ostream& os, const Bounds3<T>& b) {
			os << "[ " << b.pMin << " - " << b.pMax << " ]";
			return os;
		}
	};

	using Bounds3f = Bounds3<Float>;

	//Vector3
	template <typename T, typename U>
	inline Vector3<T> operator*(U s, const Vector3<T>& v) {
		return v * s;
	}

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

	template <typename T>
	inline Vector3<T> Cross(const Vector3<T>& v1, const Normal3<T>& v2) {
		DCHECK(!v1.HasNaNs() && !v2.HasNaNs());
		double v1x = v1.x, v1y = v1.y, v1z = v1.z;
		double v2x = v2.x, v2y = v2.y, v2z = v2.z;
		return Vector3<T>((v1y * v2z) - (v1z * v2y), (v1z * v2x) - (v1x * v2z),
			(v1x * v2y) - (v1y * v2x));
	}

	template <typename T>
	inline Vector3<T> Cross(const Normal3<T>& v1, const Vector3<T>& v2) {
		DCHECK(!v1.HasNaNs() && !v2.HasNaNs());
		double v1x = v1.x, v1y = v1.y, v1z = v1.z;
		double v2x = v2.x, v2y = v2.y, v2z = v2.z;
		return Vector3<T>((v1y * v2z) - (v1z * v2y), (v1z * v2x) - (v1x * v2z));
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

	template <typename T>
	int MaxDimension(const Vector3<T>& v) {
		return (v.x > v.y) ? ((v.x > v.z) ? 0 : 2) : ((v.y > v.z) ? 1 : 2);
	}

	template <typename T>
	Vector3<T> Min(const Vector3<T>& p1, const Vector3<T>& p2) {
		return Vector3<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
			std::min(p1.z, p2.z));
	}

	template <typename T>
	Vector3<T> Max(const Vector3<T>& p1, const Vector3<T>& p2) {
		return Vector3<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y),
			std::max(p1.z, p2.z));
	}

	template<typename T>
	Vector3<T> Permute(const Vector3<T>& v, int x, int y, int z) {
		return Vector3<T>(v[x], v[y], v[z]);
	}

	//Vector2
	template<typename T>
	Vector2<T> Abs(const Vector2<T>& v) {
		return Vector2<T>(std::abs(v.x), std::abs(v.y));
	}

	template<typename T>
	inline Float Dot(const Vector2<T>& v1, const Vector2<T>& v2) {
		DCHECK(!v1.HasNaNs() && !v2.HasNaNs());
		return v1.x * v2.x + v1.y * v2.y;
	}

	template<typename T>
	inline Float AbsDot(const Vector2<T>& v1, const Vector2<T>& v2) {
		DCHECK(!v1.HasNaNs && !v2.HasNaNs());
		return std::abs(Dot(v1, v2));
	}

	template<typename T>
	inline Vector2<T> Normalize(const Vector2<T>& v) {
		return v / v.Length();
	}

	template<typename T,typename U>
	inline Vector2<T> operator*(U f, const Vector2<T>& v) {
		return v * f;
	}

	//Point
	template<typename T>
	inline Float Distance(const Point3<T>& p1, const Point3<T>& p2) {
		return (p1 - p2).Length();
	}

	template<typename T>
	inline Float DistanceSquared(const Point3<T>& p1, const Point3<T>& p2) {
		return (p1 - p2).LengthSquared();
	}

	template<typename T,typename U>
	inline Point3<T> operator*(U f, const Point3<T>& p) {
		DCHECK(!p.HasNaNs());
		return p * f;
	}

	template<typename T>
	Point3<T> Lerp(Float t, const Point3<T>& p0, const Point3<T>& p1) {
		return (1 - t) * p0 + t * p1;
	}

	template<typename T>
	Point3<T> Min(const Point3<T>& p1, const Point3<T>& p2) {
		return Point3<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
			std::min(p1.z, p2.z));
	}

	template <typename T>
	Point3<T> Max(const Point3<T>& p1, const Point3<T>& p2) {
		return Point3<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y),
			std::max(p1.z, p2.z));
	}

	template <typename T>
	Point3<T> Floor(const Point3<T>& p) {
		return Point3<T>(std::floor(p.x), std::floor(p.y), std::floor(p.z));
	}

	template <typename T>
	Point3<T> Ceil(const Point3<T>& p) {
		return Point3<T>(std::ceil(p.x), std::ceil(p.y), std::ceil(p.z));
	}

	template <typename T>
	Point3<T> Abs(const Point3<T>& p) {
		return Point3<T>(std::abs(p.x), std::abs(p.y), std::abs(p.z));
	}

	template <typename T>
	inline Float Distance(const Point2<T>& p1, const Point2<T>& p2) {
		return (p1 - p2).Length();
	}

	template <typename T>
	inline Float DistanceSquared(const Point2<T>& p1, const Point2<T>& p2) {
		return (p1 - p2).LengthSquared();
	}

	template <typename T, typename U>
	inline Point2<T> operator*(U f, const Point2<T>& p) {
		DCHECK(!p.HasNaNs());
		return p * f;
	}

	template <typename T>
	Point2<T> Floor(const Point2<T>& p) {
		return Point2<T>(std::floor(p.x), std::floor(p.y));
	}

	template <typename T>
	Point2<T> Ceil(const Point2<T>& p) {
		return Point2<T>(std::ceil(p.x), std::ceil(p.y));
	}

	template <typename T>
	Point2<T> Lerp(Float t, const Point2<T>& v0, const Point2<T>& v1) {
		return (1 - t) * v0 + t * v1;
	}

	template <typename T>
	Point2<T> Min(const Point2<T>& pa, const Point2<T>& pb) {
		return Point2<T>(std::min(pa.x, pb.x), std::min(pa.y, pb.y));
	}

	template <typename T>
	Point2<T> Max(const Point2<T>& pa, const Point2<T>& pb) {
		return Point2<T>(std::max(pa.x, pb.x), std::max(pa.y, pb.y));
	}

	template <typename T>
	Point3<T> Permute(const Point3<T>& p, int x, int y, int z) {
		return Point3<T>(p[x], p[y], p[z]);
	}

	//Normal3
	template<typename T,typename U>
	inline Normal3<T> operator*(U f, const Normal3<T>& n) {
		return Normal3<T>(f * n.x, f * n.y, f * n.z);
	}

	template <typename T>
	inline Normal3<T> Normalize(const Normal3<T>& n) {
		return n / n.Length();
	}

	template <typename T>
	inline T Dot(const Normal3<T>& n1, const Vector3<T>& v2) {
		DCHECK(!n1.HasNaNs() && !v2.HasNaNs());
		return n1.x * v2.x + n1.y * v2.y + n1.z * v2.z;
	}

	template <typename T>
	inline T Dot(const Vector3<T>& v1, const Normal3<T>& n2) {
		DCHECK(!v1.HasNaNs() && !n2.HasNaNs());
		return v1.x * n2.x + v1.y * n2.y + v1.z * n2.z;
	}

	template <typename T>
	inline T Dot(const Normal3<T>& n1, const Normal3<T>& n2) {
		DCHECK(!n1.HasNaNs() && !n2.HasNaNs());
		return n1.x * n2.x + n1.y * n2.y + n1.z * n2.z;
	}

	template <typename T>
	inline T AbsDot(const Normal3<T>& n1, const Vector3<T>& v2) {
		DCHECK(!n1.HasNaNs() && !v2.HasNaNs());
		return std::abs(n1.x * v2.x + n1.y * v2.y + n1.z * v2.z);
	}

	template <typename T>
	inline T AbsDot(const Vector3<T>& v1, const Normal3<T>& n2) {
		DCHECK(!v1.HasNaNs() && !n2.HasNaNs());
		return std::abs(v1.x * n2.x + v1.y * n2.y + v1.z * n2.z);
	}

	template <typename T>
	inline T AbsDot(const Normal3<T>& n1, const Normal3<T>& n2) {
		DCHECK(!n1.HasNaNs() && !n2.HasNaNs());
		return std::abs(n1.x * n2.x + n1.y * n2.y + n1.z * n2.z);
	}

	template <typename T>
	inline Normal3<T> Faceforward(const Normal3<T>& n, const Vector3<T>& v) {
		return (Dot(n, v) < 0.f) ? -n : n;
	}

	template <typename T>
	inline Normal3<T> Faceforward(const Normal3<T>& n, const Normal3<T>& n2) {
		return (Dot(n, n2) < 0.f) ? -n : n;
	}

	template <typename T>
	inline Vector3<T> Faceforward(const Vector3<T>& v, const Vector3<T>& v2) {
		return (Dot(v, v2) < 0.f) ? -v : v;
	}

	template <typename T>
	inline Vector3<T> Faceforward(const Vector3<T>& v, const Normal3<T>& n2) {
		return (Dot(v, n2) < 0.f) ? -v : v;
	}

	template <typename T>
	Normal3<T> Abs(const Normal3<T>& v) {
		return Normal3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
	}

	//Bounds3
	template <typename T>
	Bounds3<T> Union(const Bounds3<T>& b, const Point3<T>& p) {
		Bounds3<T> ret;
		ret.pMin = Min(b.pMin, p);
		ret.pMax = Max(b.pMax, p);
		return ret;
	}

	template <typename T>
	Bounds3<T> Union(const Bounds3<T>& b1, const Bounds3<T>& b2) {
		Bounds3<T> ret;
		ret.pMin = Min(b1.pMin, b2.pMin);
		ret.pMax = Max(b1.pMax, b2.pMax);
		return ret;
	}

}
