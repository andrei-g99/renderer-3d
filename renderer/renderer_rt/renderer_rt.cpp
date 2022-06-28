#include <string>
#include <vector>
#include <iterator>
#include <fstream>
#include <iostream>
#include "vec3.h"
#include "world.h"
#include "triangle.h"
#include "camera.h"
#include "object.h"
#include "light_source.h"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

using math::vec3;

int main() {
    const int width = 800;
    const int height = 600;
    double move_step = 0.01;
    auto cam = std::make_shared<camera>(camera(vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, 0, 1), 1, 0.75));
    auto tr = std::make_shared<triangle>(triangle(vec3(1, 1, -5.5), vec3(-1, 1, -5.5), vec3(0, -1, -5.5)));
    auto tr2 = std::make_shared<triangle>(triangle({ 1, 1 + 0.5, -6.5 }, { -1, 1 + 0.5, -6.5 }, { 0, -1 + 0.5, -6.5 }, sf::Color::Red));
    std::vector<std::shared_ptr<triangle>> list;
    list.push_back(tr); list.push_back(tr2);

    auto scene = world(cam, list);

    sf::RenderWindow window(sf::VideoMode(width, height), "RENDERER");

    bool start_flag = true;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Enter))
                start_flag = true;

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Down))
                (*cam).move_y((*cam).center()[1] - move_step);

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Up))
                (*cam).move_y((*cam).center()[1] + move_step);

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Left))
                (*cam).move_x((*cam).center()[0] + move_step);

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Right))
                (*cam).move_x((*cam).center()[0] - move_step);

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::W))
                (*cam).move_z((*cam).center()[2] - move_step);

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::S))
                (*cam).move_z((*cam).center()[2] + move_step);//this is backward!
        }
        if (start_flag)
        {
            window.clear();

            //DRAW

            std::vector<triangle> projected = render::project_triangles(scene);
            std::vector<triangle> screened = render::projected_to_screen(projected, width, height, *cam);
            sf::Image img = render::Rasterizer(width, height, screened, *cam);


            sf::Texture texture;
            texture.loadFromImage(img);
            sf::Sprite sprite;
            sprite.setTexture(texture, true);

            window.draw(sprite);
            window.display();


        }
        else
        {

        }

    }

}
