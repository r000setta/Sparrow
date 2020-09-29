#pragma once
#include <type_traits>
#include <cinttypes>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>
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
#ifdef FLOAT_AS_DOUBLE
	typedef double Float;
#else
	typedef float Float;
#endif // FLOAT_AS_DOUBLE


#ifdef _MSC_VER
#define MaxFloat std::numeric_limits<Float>::max()
#define Infinity std::numeric_limits<Float>::infinity()
#endif // _MSC_VER

}