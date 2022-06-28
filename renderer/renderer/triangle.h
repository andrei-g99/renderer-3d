#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vec3.h"
#include "SFML/Graphics/Color.hpp"
#include "object.h"
using math::vec3;
class triangle : public object
{
private:
	vec3 m_point1;
	vec3 m_point2;
	vec3 m_point3;
	sf::Color m_color;

	friend class world;
public:

	triangle(const vec3& p1, const vec3& p2, const vec3& p3, const sf::Color color = sf::Color::Green)
	{

		m_point1 = p1;
		m_point2 = p2;
		m_point3 = p3;
		m_color = color;
	}

	vec3 first() const
	{
		return m_point1;
	}

	vec3 second() const
	{
		return m_point2;
	}

	vec3 third() const
	{
		return m_point3;
	}

	void set_first(const vec3& in)
	{
		m_point1 = in;
	}

	void set_second(const vec3& in)
	{
		m_point2 = in;
	}

	void set_third(const vec3& in)
	{
		m_point3 = in;
	}

	sf::Color color() const
	{
		return m_color;
	}

	void set_color(const sf::Color& in)
	{
		m_color = in;
	}

};







#endif