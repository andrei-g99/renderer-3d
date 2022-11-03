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

std::vector<triangle> projected;
std::vector<triangle> screened;

world config();

int main() {

    world scene = config();

    auto t0 = utility::TimerStart();

    sf::Image img = render::Rasterizer(screened, scene, projected, true);

    utility::TimerRead(t0);


    std::string path = "..\\out\\output2.png";
    img.saveToFile(path);

}




world config() {


    const int width = 1920;
    const int height = 1080;
    double w = 1;
    double h = (double)height / (double)width;
    auto cam = std::make_shared<camera>(camera({ 0,0,0 }, { 0,0,0 }, { 0,0,1 }, w, h));
    auto ls = std::make_shared<light>(light({ -6,5,5 }, 0.5));

    cube cub = cube({ 0,0,-5 }, 1, sf::Color::White);
    std::vector<std::shared_ptr<triangle>> list = cub.primitives();
    std::vector<std::shared_ptr<light>> light_list;
    light_list.push_back(ls);

    auto scene = world(cam, list, light_list, w, h, width, height);

    projected = render::project_triangles(scene);
    screened = render::projected_to_screen(projected, (double)width, (double)height, *cam);

    return scene;

}