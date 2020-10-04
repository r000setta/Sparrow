#pragma once
#include <type_traits>
#include <cinttypes>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include "glog/logging.h"

#pragma warning(disable : 4305)  // double constant assigned to float
#pragma warning(disable : 4244)  // int -> float conversion
#pragma warning(disable : 4843)  // double -> float conversion
#pragma warning(disable : 4267)  // size_t -> int
#pragma warning(disable : 4838)  // another double -> int

namespace sparrow {
	//Global Forward Declartions
	template<typename T>
	class Vector2;
	template<typename T>
	class Vector3;
	template<typename T>
	class Point2;
	template<typename T>
	class Point3;
	template<typename T>
	class Normal3;
	class Ray;
	template<typename T>
	class Bounds3;
	struct Matrix4x4;
	class Transform;
	class Shape;
	struct Interaction;
	class SurfaceInteraction;
	class Primitive;
#define FLOAT_AS_DOUBLE
#ifdef FLOAT_AS_DOUBLE
	typedef double Float;
#else
	typedef float Float;
#endif // FLOAT_AS_DOUBLE


#ifdef _MSC_VER
#define MaxFloat std::numeric_limits<Float>::max()
#define Infinity std::numeric_limits<Float>::infinity()
#define MachineEpsilon (std::numeric_limits<Float>::epsilon()*0.5)
#endif // _MSC_VER

	static constexpr Float Pi = 3.14159265358979323846;

	inline Float gamma(int n) {
		return (n * MachineEpsilon) / (1 - n * MachineEpsilon);
	}

	template<typename T,typename U,typename V>
	inline T Clamp(T val, U low, V high) {
		if (val < low)
			return low;
		else if (val > high)
			return high;
		else
			return val;
	}

	inline Float Radians(Float deg) {
		return (Pi / 180) * deg;
	}

	inline Float RandomFloat() {
		static std::uniform_real_distribution<Float> dis(0.0, 1.0);
		static std::mt19937 gen;
		return dis(gen);
	}

	inline Float RandomFloat(Float min,Float max) {
		return min + (max - min) * RandomFloat();
	}

	inline int RandomInt(int min, int max) {
		return static_cast<int>(RandomFloat(min, max + 1));
	}
}
