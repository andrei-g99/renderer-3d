#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <chrono>
#include <vector>
#include <iterator>
#include "vec3.h"
#include "mmath.h"

using math::vec3;
namespace utility
{
	bool EdgeFunction(vec3& first, vec3& second)
	{
		vec3 crs = math::cross(first, second);
		if (crs[2] >= 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool InsideTriangleTest(vec3 test_point, triangle tr)
	{
		vec3 t_1 = tr.first();
		vec3 t_2 = tr.second();
		vec3 t_3 = tr.third();
		t_1.set_z(0); t_2.set_z(0); t_3.set_z(0); // sets z to 0 to have 2D vectors (ignoring depth)
		vec3 cross1 = math::cross(t_3 - t_1, test_point - t_1);
		vec3 cross2 = math::cross(t_1 - t_2, test_point - t_2);
		vec3 cross3 = math::cross(t_2 - t_3, test_point - t_3);

		if (((cross1[2] > 0) && (cross2[2] > 0) && (cross3[2] > 0)) || ((cross1[2] < 0) && (cross2[2] < 0) && (cross3[2] < 0)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	std::pair<double,double> ScreenToProjSpace(double x,double y,unsigned int width,unsigned int height, double w,double h)
	{
		return {(x/(double)width)-(w/2),(y / (double)height) - (h / 2) };
	}

	std::pair<vec3, vec3> BoxClipper(const int width, const int height, vec3 tlc, vec3 brc)
	{
		//outputs the intersection box between the screen (e.g. double format 800x600) and the triangle 2D bounding box
		std::pair<vec3, vec3> output;
		if ((tlc[0] <= width) && (tlc[0] >= 0) && (brc[0] <= width) && (brc[0] >= 0)
			&& (tlc[1] <= height) && (tlc[1] >= 0) && (brc[1] <= height) && (brc[1] >= 0))
		{
			//inside the box completely
			output.first = tlc;
			output.second = brc;
			return output;
		}
		else
		{
			//very slow for now, not optimal
			output.first = vec3{ 0,(double)height,0 };
			output.second = vec3{ (double)width,0,0 };
			return output;
		}
	}

	vec3 barycentric(triangle in, vec3 p)
	{
		double tot_area = 0.5 * math::norm(math::cross(in.first() - in.second(), in.first() - in.third()));
		double area0 = 0.5 * math::norm(math::cross(p - in.second(), in.second() - in.third()));
		double area1 = 0.5 * math::norm(math::cross(p - in.first(), in.third() - in.first()));
		double area2 = 0.5 * math::norm(math::cross(p - in.first(), in.second() - in.first()));

		return vec3{ area0 / tot_area,area1 / tot_area,area2 / tot_area };
	}

	float compute_distance_z(const vec3& p_2d,const triangle& tr)
	{
		//computes z distance from screen to projected triangle in 3D space
		vec3 lambda = barycentric(tr, p_2d);
		double a = (lambda[0]) / (tr.first()[2]);
		double b = (lambda[1]) / (tr.second()[2]);
		double c = (lambda[2]) / (tr.third()[2]);
		double z = 1 / (a + b + c);
		return (float)z;
	}

	//DEBUG

	inline void print_triangle(const triangle& in)
	{
		std::cout << "\n____________________________\n";
		std::cout << "FIRST POINT:  x = " << in.first()[0] << "; y = " << in.first()[1] << "; z = " << in.first()[2] << std::endl;
		std::cout << "SECOND POINT:  x = " << in.second()[0] << "; y = " << in.second()[1] << "; z = " << in.second()[2] << std::endl;
		std::cout << "THIRD POINT:  x = " << in.third()[0] << "; y = " << in.third()[1] << "; z = " << in.third()[2] << std::endl;
		std::cout << "____________________________\n";
	}

	std::pair<unsigned int, unsigned int> CoordsToSFML(const std::pair<unsigned int, unsigned int>& in, unsigned int height,unsigned int width)
	{
		//2D natural coordinates to SFML coordinates
		return {width -  in.first,in.second };
	}

	vec3 vecCoordsToSFML(vec3 in, unsigned int height)
	{//2D natural coordinates to SFML coordinates
		return { in[0],(double)(height)-in[1],in[2] };
	}

	inline void print_vector(vec3& in)
	{
		std::cout << "________VECTOR________\n";
		std::cout << "x = " << in[0] << std::endl;
		std::cout << "y = " << in[1] << std::endl;
		std::cout << "z = " << in[2] << std::endl;
		std::cout << "______________________\n";
	}

	inline void print_matrix(const math::mat3& in)
	{
		std::cout << "________MATRIX________\n";
		std::cout << in[0][0] <<"|" << in[1][0] <<"|"<< in[2][0] <<"|"<< std::endl;
		std::cout << in[0][1] <<"|"<< in[1][1]<<"|"<<in[2][1]<<"|"<< std::endl;
		std::cout << in[0][2] <<"|"<< in[1][2]<<"|"<<in[2][2]<<"|"<< std::endl;
		std::cout << "______________________\n";
	}

	//timing

	std::chrono::time_point<std::chrono::steady_clock> TimerStart() {
		auto const t0 = std::chrono::high_resolution_clock::now();
		return t0;
	}

	void TimerRead(const std::chrono::time_point<std::chrono::steady_clock>& t0) {
		auto const t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = t1 - t0;
		std::cout << "Timer expired after " << duration.count() << " seconds\n";
	}

}

#endif
