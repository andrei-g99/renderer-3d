#include "pch.h"
#include "CppUnitTest.h"
#include <string>
#include <vector>
#include <iterator>
#include <fstream>
#include <iostream>
#include "cmath"
#include "vec3.h"
#include "world.h"
#include "mmath.h"
#include "triangle.h"
#include "camera.h"
#include "object.h"
#include "light_source.h"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

using math::vec3;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestRenderer
{
	TEST_CLASS(UnitTestVectors)
	{
	public:

		TEST_METHOD(TestCrossProduct)
		{
			const wchar_t m1[] = L"First cross product failed";
			const wchar_t m2[] = L"Second cross product failed";
			const wchar_t m3[] = L"Third cross product failed";

			vec3 v1{ 1,2,3 };		vec3 v2{ 4,2,1 };
			Assert::IsTrue(vec3{ -4,11,-6 } == math::cross(v1, v2), m1);

			v1 = { 1,2,0 };   v2 = { 4,2,0 };
			Assert::IsTrue(vec3{ 0,0,-6 } == math::cross(v1, v2), m2);

			v1 = { 1.2,-5,0 };   v2 = { 0,2,0 };
			Assert::IsTrue(vec3{ 0,0,2.4 } == math::cross(v1, v2), m3);
		}

		TEST_METHOD(TestVectorAdd)
		{
			const wchar_t m1[] = L"First failed";
			const wchar_t m2[] = L"Second failed";
			const wchar_t m3[] = L"Third failed";
			vec3 v1{ 1,2,3 };		vec3 v2{ 4,2,1 };
			Assert::IsTrue(vec3{ 5,4,4 } == (v1 + v2), m1);

			v1 = { 1,2,0 };   v2 = { 4,2,0 };
			Assert::IsTrue(vec3{ 5,4,0 } == (v1 + v2), m2);

			v1 = { -1.2,-5,0 };   v2 = { 0,2,0 };
			Assert::IsTrue(vec3{ -1.2,-3,0 } == (v1 + v2), m1);
		}

		TEST_METHOD(TestVectorNormalization)
		{
			const wchar_t m1[] = L"First failed";
			const wchar_t m2[] = L"Second failed";
			const wchar_t m3[] = L"Third failed";
			vec3 v1{ 0,1,0 };
			Assert::IsTrue(vec3{ 0,1,0 } == math::normalize(v1), m1);

			v1 = { 0,1,1 };
			Assert::IsTrue(vec3{ 0,1 / (sqrt(2)),1 / (sqrt(2)) } == math::normalize(v1), m2);

			v1 = { 1,-1,1 };
			Assert::IsTrue(vec3{ 1 / (sqrt(3)),1 / (-sqrt(3)),1 / (sqrt(3)) } == math::normalize(v1), m3);

		}

		TEST_METHOD(TestVectorNorm)
		{
			const wchar_t m1[] = L"First failed";
			const wchar_t m2[] = L"Second failed";
			const wchar_t m3[] = L"Third failed";
			vec3 v1{ 3, 2, -1 };
			Assert::IsTrue((double)sqrt(14) == math::norm(v1), m1);

			v1 = { 4, 3, 0 };
			Assert::IsTrue((double)(5) == math::norm(v1), m2);

			v1 = { -10, 3, 4 };
			Assert::IsTrue((double)(5 * sqrt(5)) == math::norm(v1), m3);

		}

	};

	TEST_CLASS(PrimitiveOperators)
	{
		TEST_METHOD(TestPointInsideTriangle)
		{
			const wchar_t m1[] = L"First failed";
			const wchar_t m2[] = L"Second failed";
			const wchar_t m3[] = L"Third failed";

			triangle tr = triangle{ {2,4,0},{7,6,0},{7,2,0} };

			Assert::IsTrue(utility::InsideTriangleTest({ 5,4,0 }, tr), m1);
			Assert::IsTrue(utility::InsideTriangleTest({ 6,3,0 }, tr), m2);
			Assert::IsFalse(utility::InsideTriangleTest({ 2,6,0 }, tr), m3);

			tr = triangle{ {7,6,0},{7, 2, 0},{2, 4, 0} };

			Assert::IsTrue(utility::InsideTriangleTest({ 5,4,0 }, tr), m1);
			Assert::IsTrue(utility::InsideTriangleTest({ 6,3,0 }, tr), m2);
			Assert::IsFalse(utility::InsideTriangleTest({ 2,6,0 }, tr), m3);

			tr = triangle{ {150,300,0},{300, 100, 0},{400, 700, 0} };
			Assert::IsTrue(utility::InsideTriangleTest({ 250,300,0 }, tr), m1);
			Assert::IsTrue(utility::InsideTriangleTest({ 300,400,0 }, tr), m2);
			Assert::IsFalse(utility::InsideTriangleTest({ 100,700,0 }, tr), m3);
		}

		TEST_METHOD(TestBarycentricCoordinates)
		{
			const wchar_t m1[] = L"First failed";
			const wchar_t m2[] = L"Second failed";
			const wchar_t m3[] = L"Third failed";

			triangle tr = triangle{ {1,0,0},{-0.5,0.8660254,0},{-0.5,-0.8660254,0} };
			vec3 lambda = utility::barycentric(tr, { 0,0,0 });
			Assert::IsTrue(math::are_doubles_equal(lambda[0], lambda[1]) && math::are_doubles_equal(lambda[1], lambda[2]));

		}
	};

	TEST_CLASS(MathTests)
	{
		TEST_METHOD(TestMatrixAddition)
		{
			const wchar_t m1[] = L"First matrix addition failed";
			const wchar_t m2[] = L"Second matrix addition failed";
			const wchar_t m3[] = L"Third matrix addition failed";

			const math::mat3 mat1 = math::mat3{{1,4,7},{2,5,8},{3,6,9}};
			const math::mat3 mat2 = math::mat3{ {1,0,0},{0,1,0},{0,0,1} };
			Assert::IsTrue((mat1 + mat2) == math::mat3{ {2,4,7}, {2,6,8}, {3,6,10}}, m1);


			const math::mat3 mat3 = math::mat3{ {1,4,7},{2,5,8},{3,6,9} };
			const math::mat3 mat4 = math::mat3{ {1,0,0},{0,1,0},{0,0,1} };
			Assert::IsTrue((mat3 + mat4) == math::mat3{ {2,4,7}, {2,6,8}, {3,6,10} }, m2);


			const math::mat3 mat5 = math::mat3{ {1,0,1},{1,1,1},{0,0,0} };
			const math::mat3 mat6 = math::mat3{ {0,0,2},{0,10,-2},{0,0,10} };
			Assert::IsTrue((mat5 + mat6) == math::mat3{ {1,0,3}, {1,11,-1}, {0,0,10} }, m3);

		}

		TEST_METHOD(TestMatrixMultiply)
		{
			const wchar_t m1[] = L"First matrix product failed";
			const wchar_t m2[] = L"Second matrix product failed";
			const wchar_t m3[] = L"Third matrix product failed";

			const math::mat3 mat1 = math::mat3{ {1,3,1},{2,2,2},{3,1,3} };
			const math::mat3 mat2 = math::mat3{ {4,6,4},{5,5,6},{6,4,5} };
			Assert::IsTrue((mat1 * mat2) == math::mat3{ {28,28,28}, {33,31,33}, {29,31,29} }, m1);


			const math::mat3 mat3 = math::mat3{ {1,0,0},{0,1,0},{0,0,1} };
			const math::mat3 mat4 = math::mat3{ {4,6,4},{5,5,6},{6,4,5} };
			Assert::IsTrue((mat3 * mat4) == math::mat3{ {4,6,4},{5,5,6},{6,4,5} }, m2);


			const math::mat3 mat5 = math::mat3{ {1,9,3},{5,1,-4},{-14,2,1} };
			const math::mat3 mat6 = math::mat3{ {4,6,-4},{-5,5,6},{6,4,5} };
			Assert::IsTrue((mat5 * mat6) == math::mat3{ {90,34,-16}, {-64,-28,-29}, {-44,68,7} }, m3);

		}

		TEST_METHOD(TestMatrixDeterminant)
		{
			const wchar_t m1[] = L"First matrix determinant failed";
			const wchar_t m2[] = L"Second matrix determinant failed";
			const wchar_t m3[] = L"Third matrix determinant failed";

			const math::mat3 mat1 = math::mat3{ {1,3,2},{2,2,1},{3,1,3} };
			Assert::IsTrue(math::are_doubles_equal(math::determinant(mat1),-12), m1);


			const math::mat3 mat2 = math::mat3{ {2,2,1},{-3,0,4},{1,-1,5} };
			Assert::IsTrue(math::are_doubles_equal(math::determinant(mat2), 49), m2);

			const math::mat3 mat3 = math::mat3{ {1,-3,2},{3,-1,3},{2,-3,1} };
			Assert::IsTrue(math::are_doubles_equal(math::determinant(mat3), -15), m3);
		}

		TEST_METHOD(TestMatrixTranspose)
		{
			const math::mat3 mat1 = math::mat3{ {1,3,2},{2,2,1},{3,1,3} };
			Assert::IsTrue(math::transpose(mat1) == math::mat3{{1,2,3},{3,2,1},{2,1,3}});

		}
		TEST_METHOD(TestMatrixInverse)
		{
			const wchar_t m1[] = L"First matrix inverse failed";
			const wchar_t m2[] = L"Second matrix inverse failed";
			const wchar_t m3[] = L"Third matrix inverse failed";

			const math::mat3 mat1 = { {1,3,2},{2,2,1},{3,1,3} };
			Assert::IsTrue(math::inverse(mat1) == math::mat3{ {(double) - 5 / 12,(double)7 / 12,(double)1 / 12}, {(double)1 / 4,(double)1 / 4,(double)-1 / 4}, {(double)1/3,(double)-2/3,(double)1/3}}, m1);

			
			const math::mat3 mat2 = math::mat3{ {1,4,1},{2,5,8},{3,6,9} };
			Assert::IsTrue(math::inverse(mat2) == math::mat3{ {(double)-1 / 6,(double)-5 / 3,(double)3 / 2}, {(double)1 / 3,(double)1 / 3,(double)-1 / 3}, {(double)-1 / 6,(double)1 / 3,(double)-1 / 6} }, m2);


			const math::mat3 mat3 = math::mat3{ {1,0,1},{1,1,1},{0,0,-2} };
			Assert::IsTrue(math::inverse(mat3) == math::mat3{ {(double)1,(double)0,(double)1 / 2}, {(double)-1,(double)1,(double)0}, {(double)0,(double)0,(double)-1 / 2} }, m3);
			
		}

	};

}
