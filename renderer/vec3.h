#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

namespace math
{
	class vec3
	{
	private:
		double x;
		double y;
		double z;
	public:

		vec3(double xx = 0, double yy = 0, double zz = 0)
		{
			x = xx;
			y = yy;
			z = zz;
		}

		double operator [](int index) const;

		vec3 operator*(double scalar);

		bool operator==(vec3 in) const;

		void set_x(double in)
		{
			x = in;
		}
		void set_y(double in)
		{
			y = in;
		}
		void set_z(double in)
		{
			z = in;
		}

	};



	inline double vec3::operator[](const int index) const
	{

		if ((index > 2) || (index < 0))
		{
			std::cout << "\nERROR: Vector index out of bound\n";
			exit(0);
		}
		if (index == 0)
		{
			return x;
		}
		if (index == 1)
		{
			return y;
		}
		if (index == 2)
		{
			return z;
		}
		return 0;
	}

	inline vec3 vec3::operator*(double scalar)
	{
		vec3 result = vec3(x * scalar, y * scalar, z * scalar);
		return result;
	}

	inline bool are_doubles_equal(double first, double second, const double eps = 0.000001)
	{
		if (abs(first - second) < eps)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool vec3::operator==(vec3 in) const
	{
		if (are_doubles_equal(x, in[0]) && are_doubles_equal(y, in[1]) && are_doubles_equal(z, in[2]))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	inline vec3 operator*(double scalar, vec3 in)
	{
		return (in * scalar);
	}

	inline vec3 operator+(vec3 first, vec3 second)
	{
		vec3 result = vec3(first[0] + second[0], first[1] + second[1], first[2] + second[2]);
		return result;
	}

	inline vec3 operator-(vec3 first, vec3 second)
	{
		vec3 result = vec3(first[0] - second[0], first[1] - second[1], first[2] - second[2]);
		return result;
	}

	double dot(vec3 first, vec3 second)
	{

		return (first[0] * second[0] + first[1] * second[1] + first[2] * second[2]);
	}

	inline vec3 cross(vec3 first, vec3 second)
	{
		vec3 result = vec3((first[1] * second[2]) - (first[2] * second[1]), first[2] * second[0] - first[0] * second[2],
			first[0] * second[1] - first[1] * second[0]);
		return result;
	}

	inline double norm(vec3 in)
	{
		return sqrt(pow(in[0], 2) + pow(in[1], 2) + pow(in[2], 2));
	}

	inline bool isEqual(vec3 first, vec3 second)
	{
		if (are_doubles_equal(first[0], second[0]) && are_doubles_equal(first[1], second[1])
			&& are_doubles_equal(first[2], second[2]))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	inline vec3 normalize(vec3 in)
	{
		const double n = norm(in);
		return (in * (1 / n));
	}

}

#endif
