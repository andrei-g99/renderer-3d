#include <string>
#include <vector>
#include <iterator>
#include <fstream>
#include <iostream>
#include "vec3.h"
#include "world.h"
#include "triangle.h"
#include "mmath.h"
#include "camera.h"
#include "object.h"
#include "cube.h"
#include "light_source.h"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

int main() {


	const int width = 1920;
    const int height = 1080;
    double w = 1;
    double h = (double)height / (double)width;
    auto cam = std::make_shared<camera>(camera({0,0,0}, {0,0,0}, {0,0,1}, w,h));
    auto ls = std::make_shared<light>(light({-6,5,5},0.5));

    cube cub = cube({0,0,-5},1,sf::Color::White);
    std::vector<std::shared_ptr<triangle>> list = cub.primitives();
    std::vector<std::shared_ptr<light>> light_list;
    light_list.push_back(ls);

    auto scene = world(cam, list,light_list,w,h,width,height);

    cub.rotate_y(0.4);
    cub.rotate_x(0);

    std::vector<triangle> projected = render::project_triangles(scene);
    std::vector<triangle> screened = render::projected_to_screen(projected, (double)width, (double)height, *cam);
    sf::Image img = render::Rasterizer(screened, scene,projected,true);

    //std::string path = "C:\\Users\\bitma\\Desktop\\renders\\output_3.png";
    std::string path = "..\\out\\output.png";
    img.saveToFile(path);


}
