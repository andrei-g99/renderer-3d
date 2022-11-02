#ifndef WORLD_H
#define WORLD_H

#include "vec3.h"
#include "camera.h"
#include "object.h"
#include "triangle.h"
#include "utility.h"
#include "light_source.h"
#include <memory>
#include <vector>
#include <iterator>
#include <cmath>
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Image.hpp"


using math::vec3;
using utility::EdgeFunction;

class world
{
private:
	std::vector<std::shared_ptr<triangle>> object_list_;
	std::shared_ptr<camera> camera_;
	std::vector<std::shared_ptr<light>> sources_list_;
	double m_camera_width;
	double m_camera_height;
	unsigned int m_screen_width;
	unsigned int m_screen_height;

public:

	world(const std::shared_ptr <camera>& cam, const std::vector<std::shared_ptr<triangle>>& list,const std::vector<std::shared_ptr<light>>& l,double w,double h,unsigned int width,unsigned int height)
	{
		object_list_ = list;
		camera_ = cam;
		sources_list_ = l;
		m_camera_width = w;
		m_camera_height = h;
		m_screen_width = width;
		m_screen_height = height;
	}

	std::vector<std::shared_ptr<triangle>> objects() const
	{
		return object_list_;
	}

	std::shared_ptr<camera> camera() const
	{
		return camera_;
	}

	std::vector<std::shared_ptr<light>> sources() const
	{
		return sources_list_;
	}

	double cam_h() const
	{
		return m_camera_height;
	}

	double cam_w() const
	{
		return m_camera_width;
	}

	unsigned int screen_h() const
	{
		return m_screen_height;
	}

	unsigned int screen_w() const
	{
		return m_screen_width;
	}
};
namespace render
{

	sf::Color shader(const sf::Color& initial_color,const triangle& t_in, const std::vector<std::shared_ptr<light>>& ls,const camera& cam)
	{
		vec3 normal = math::normalize(math::cross(t_in.third() - t_in.first(), t_in.second() - t_in.first()));
		vec3 first_to_source = math::normalize((*ls.front()).center() - t_in.first());
		vec3 first_to_cam = math::normalize((cam.center() - t_in.first()));
		double product = math::dot(normal, first_to_source);
		double product_with_camera = math::dot(normal, first_to_cam);
		if(!(product_with_camera >= 0)) product = -product_with_camera;
		double intensity = (*ls.front()).intensity();
		
		if(product > 0)
		{
			auto subtract = (sf::Uint8)((1-intensity) * (255 * (1 - product)));
			sf::Uint8 R = initial_color.r - subtract;
			if (subtract >= initial_color.r) R = 0;

			sf::Uint8 G = initial_color.g - subtract;
			if (subtract >= initial_color.g) G = 0;

			sf::Uint8 B = initial_color.b - subtract;
			if (subtract >= initial_color.b) B = 0;

			sf::Color newc = {R,G,B};
			//std::cout << (int)G << std::endl;//
			return newc;
		}else
		{
			return sf::Color::Black;
		}


	}

	std::vector<triangle> project_triangles(const world& scene)
	{
		std::vector<triangle> output;
		auto triangles = scene.objects();
		auto cam = *scene.camera();
		for (const auto& t : triangles)
		{
			auto t_in = *t;

			const double cx = cos(cam.orientation()[0]);	const double cy = cos(cam.orientation()[1]);
			const double cz = cos(cam.orientation()[2]);	const double sx = sin(cam.orientation()[0]);
			const double sy = sin(cam.orientation()[1]);	const double sz = sin(cam.orientation()[2]);
			auto result = triangle(0, 0, 0, t_in.color());
			for (int i = 0; i < 3; i++)
			{
				//three points to project!
				vec3 point;
				if (i == 0)
					point = vec3(t_in.first()[0], t_in.first()[1], t_in.first()[2]);
				if (i == 1)
					point = vec3(t_in.second()[0], t_in.second()[1], t_in.second()[2]);
				if (i == 2)
					point = vec3(t_in.third()[0], t_in.third()[1], t_in.third()[2]);

				const double x = point[0] - cam.center()[0];
				const double y = point[1] - cam.center()[1];
				const double z = point[2] - cam.center()[2];

				const double dx = cy * (sz * y + cz * x) - sy * z;
				const double dy = sx * (cy * z + sy * (sz * y + cz * x)) + cx * (cz * y - sz * x);
				const double dz = cx * (cy * z + sy * (sz * y + cz * x)) - sx * (cz * y - sz * x);


				const double ex = cam.near()[0];
				const double ey = cam.near()[1];
				const double ez = cam.near()[2];

				const double bx = (ez / dz) * (dx)+ex;
				const double by = (ez / dz) * (dy)+ey;
				const double bz = dz;

				if (i == 0)
					result.set_first(vec3(bx, by, bz));

				if (i == 1)
					result.set_second(vec3(bx, by, bz));

				if (i == 2)
					result.set_third(vec3(bx, by, bz));
			}

			output.push_back(result);
		}
		return output;
	}

	std::vector<triangle> projected_to_screen(const std::vector<triangle>& triangles, const double w, const double h, const camera& cam)
	{
		std::vector<triangle> output;
		for (auto& t_in : triangles)
		{
			const double cw = cam.width();
			const double ch = cam.height();
			const double wslope = w / cw;
			const double hslope = h / ch;
			auto point1 = vec3(t_in.first()[0] * wslope + w / 2, t_in.first()[1] * hslope + h / 2, t_in.first()[2]);
			auto point2 = vec3(t_in.second()[0] * wslope + w / 2, t_in.second()[1] * hslope + h / 2, t_in.second()[2]);
			auto point3 = vec3(t_in.third()[0] * wslope + w / 2, t_in.third()[1] * hslope + h / 2, t_in.third()[2]);
			output.push_back(triangle{ point1,point2,point3,t_in.color() });

		}

		return output;

	}

	sf::Image Rasterizer(const std::vector<triangle>& triangles,const world& scene,const std::vector<triangle>& prj,bool shade = true)
	{

		float** depthBuffer = new float* [scene.screen_h()];
		// Initialize depth-buffer with very large number
		for (unsigned int y = 0; y < scene.screen_h(); ++y)
			depthBuffer[y] = new float[scene.screen_w()];

		for (unsigned int y = 0; y < scene.screen_h(); ++y)
			for (unsigned int x = 0; x < scene.screen_w(); ++x)
				depthBuffer[y][x] = -INFINITY;

		sf::Image output;
		output.create(scene.screen_w(), scene.screen_h(), sf::Color::Black);
		for (auto it = triangles.begin();it!=triangles.end();++it)
		{
			auto t_in = *it;
			sf::Color pixel_color = t_in.color();
			//shader
			if(shade)
				pixel_color = shader(t_in.color(), *scene.objects()[(int)(it - triangles.begin())], scene.sources(),*scene.camera());


			//make 2D bounding box
			std::vector<double> horizontal{ t_in.first()[0],t_in.second()[0],t_in.third()[0] };
			std::vector<double> vertical{ t_in.first()[1],t_in.second()[1],t_in.third()[1] };
			std::sort(horizontal.begin(), horizontal.end());
			std::sort(vertical.begin(), vertical.end());
			const double max_x = horizontal.back();		const double max_y = vertical.back();
			const double min_x = horizontal.front();	const double min_y = vertical.front();

			auto top_l_c = vec3(floor(min_x), ceil(max_y), 0);
			auto bot_r_c = vec3(ceil(max_x), floor(min_y), 0);

			//std::cout << "TOP LEFT CORNER : x = " << top_l_c[0] << " | y = " << top_l_c[1] << std::endl;///
			//std::cout << "BOTTOM RIGHT CORNER : x = " << bot_r_c[0] << " | y = " << bot_r_c[1] << std::endl;///

			if ((top_l_c[0] <= scene.screen_w()) && (top_l_c[0] >= 0) && (bot_r_c[0] <= scene.screen_w()) && (bot_r_c[0] >= 0)
				&& (top_l_c[1] <= scene.screen_h()) && (top_l_c[1] >= 0) && (bot_r_c[1] <= scene.screen_h()) && (bot_r_c[1] >= 0))
			{
				for (int i = (int)(top_l_c[0]); i < (int)bot_r_c[0]; i++)
				{
					for (int j = (int)(bot_r_c[1]); j < (int)top_l_c[1]; j++)
					{
						auto x_p = (double)(i + 0.5);
						auto y_p = (double)(j + 0.5);
						//auto temp = utility::ScreenToProjSpace(x_p, y_p, scene.screen_w(), scene.screen_h(), scene.cam_w(), scene.cam_h());
						std::pair<unsigned int, unsigned int> p_coords{ i,j };
						vec3 p = vec3{ x_p,y_p,0 };

						if (utility::InsideTriangleTest(p, t_in))
						{
							//compute depth
							float z = utility::compute_distance_z(p, t_in);
							if ((double)z < (-((*scene.camera()).near()[2] + (*scene.camera()).center()[2])))
							{
								if (z > depthBuffer[j][i])
								{
									p_coords = utility::CoordsToSFML(p_coords, scene.screen_h(),scene.screen_w());
									depthBuffer[j][i] = z;
									output.setPixel(p_coords.first, p_coords.second, pixel_color);
								}
							}


						}



					}
				}
			}
		}
		//Free memory
		for (unsigned int i = 0; i < scene.screen_h(); i++) {
			delete[] depthBuffer[i];
		}
		delete[] depthBuffer;
		return output;
	}

}
#endif

