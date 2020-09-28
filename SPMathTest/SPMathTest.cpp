#include "pch.h"
#include "CppUnitTest.h"
#include "../SparrowMath/geometry.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using sparrow::Vector3i;
using sparrow::Float;
namespace SPMathTest
{
	TEST_CLASS(SPMathTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Vector3i vec1(1, 2, 3);
			Vector3i vec2(1, 1, 1);
			Vector3i vec3{ vec1 + vec2 };
			Assert::AreEqual(vec2.LengthSquared(),(Float)3);
		}
	};
}
