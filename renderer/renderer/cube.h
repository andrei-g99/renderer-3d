#ifndef CUBE_H
#define CUBE_H
#include <functional>

#include "object.h"
#include "vec3.h"
#include "triangle.h"
#include <vector>
#include <iterator>
#include "utility.h"
#include "mmath.h"

class cube : public object
{
private:
	std::vector<std::shared_ptr<triangle>> list;
	math::vec3 m_center;
public:

	cube(const vec3& center = {0,0,0}, double l = 1, sf::Color color = sf::Color::Green)
	{
		list.reserve(12);
		//xy triangles
		list.push_back(std::make_shared<triangle>(vec3{ -l / 2,-l / 2,-l / 2 }, vec3{ -l / 2,l / 2,-l / 2 }, vec3{ l / 2,-l / 2,-l / 2 }, color));
		list.push_back(std::make_shared<triangle>(vec3{ -l / 2,l / 2,-l / 2 }, vec3{ l / 2,l / 2,-l / 2 }, vec3{ l / 2,-l / 2,-l / 2 }, color));

		//xz triangles
		list.push_back(std::make_shared<triangle>(vec3{ -l / 2 , -l / 2 , -l / 2 }, vec3{ l / 2 , -l / 2 , -l / 2 }, vec3{ l / 2 , -l / 2 , l / 2 }, color));
		list.push_back(std::make_shared<triangle>(vec3{ -l / 2 , -l / 2 , -l / 2 }, vec3{ l / 2 , -l / 2 , l / 2 }, vec3{ -l / 2 , -l / 2 , l / 2 }, color));

		//yz triangles
		list.push_back(std::make_shared<triangle>(vec3{ -l/2,-l / 2,-l / 2 }, vec3{ -l / 2,l / 2,-l / 2 }, vec3{ -l / 2,-l / 2,l / 2 }, color));
		list.push_back(std::make_shared<triangle>(vec3{ -l / 2,-l / 2,l / 2}, vec3{ -l / 2,l / 2,-l / 2}, vec3{ -l / 2,l / 2,l / 2}, color));

		//xy triangles (z>0)
		list.push_back(std::make_shared<triangle>(vec3{ -l / 2,-l / 2,l/2 }, vec3{ -l / 2,l / 2,l/2 }, vec3{ l / 2,-l / 2,l/2 }, color));
		list.push_back(std::make_shared<triangle>(vec3{ -l / 2,l / 2,l / 2 }, vec3{ l / 2,l / 2,l / 2 }, vec3{ l / 2,-l / 2,l / 2 }, color));

		//xz triangles (z>0)
		list.push_back(std::make_shared<triangle>(vec3{ l / 2,l / 2,-l / 2 }, vec3{ -l / 2,l / 2,-l / 2 }, vec3{ -l / 2,l / 2,l / 2 }, color));
		list.push_back(std::make_shared<triangle>(vec3{ l / 2,l / 2,-l / 2 }, vec3{ -l / 2,l / 2,l / 2 }, vec3{ l / 2,l / 2,l / 2 }, color));

		//yz triangles (z>0)
		list.push_back(std::make_shared<triangle>(vec3{ l / 2,-l / 2,-l / 2 }, vec3{ l / 2,l / 2,-l / 2 }, vec3{ l / 2,-l / 2,l / 2 }, color));
		list.push_back(std::make_shared<triangle>(vec3{ l / 2,-l / 2,l / 2 }, vec3{ l / 2,l / 2,-l / 2 }, vec3{ l / 2,l / 2,l / 2 }, color));

		for(const auto& i : list)
		{
			(*i).set_first((*i).first() + center);
			(*i).set_second((*i).second() + center);
			(*i).set_third((*i).third() + center);
		}

		m_center = center;
		
	}

	void move(const vec3& displacement)
	{
		for (const auto& i : list)
		{
			(*i).set_first((*i).first() + displacement);
			(*i).set_second((*i).second() + displacement);
			(*i).set_third((*i).third() + displacement);
		}
	}

	std::vector<std::shared_ptr<triangle>> primitives() const
	{
		return list;
	}

	void rotate_x(double angle)
	{
		math::mat3 rotation_matrix = {{1,0,0},{0,cos(angle),sin(angle)},{0,sin(angle),cos(angle)}};

		for(auto& i : list)
		{
			
			(*i).set_first(m_center + rotation_matrix * ((*i).first() - m_center));
			(*i).set_second(m_center + rotation_matrix * ((*i).second() - m_center));
			(*i).set_third(m_center + rotation_matrix * ((*i).third() - m_center));
			
		}
		
	}

	void rotate_y(double angle)
	{
		math::mat3 rotation_matrix = { {cos(angle),0,sin(angle)},{0,1,0},{-sin(angle),0,cos(angle)}};

		for (auto& i : list)
		{
			(*i).set_first(m_center + rotation_matrix * ((*i).first() - m_center));
			(*i).set_second(m_center + rotation_matrix * ((*i).second() - m_center));
			(*i).set_third(m_center + rotation_matrix * ((*i).third() - m_center));
		}

	}

	void rotate_z(double angle)
	{
		math::mat3 rotation_matrix = { {cos(angle),-sin(angle),0},{sin(angle),cos(angle),0},{0,0,1}};

		for (auto& i : list)
		{
			(*i).set_first(m_center + rotation_matrix * ((*i).first() - m_center));
			(*i).set_second(m_center + rotation_matrix * ((*i).second() - m_center));
			(*i).set_third(m_center + rotation_matrix * ((*i).third() - m_center));

		}
	}

};

#endif

