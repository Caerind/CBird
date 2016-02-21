#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Random.hpp"
#include "ResourceManager.hpp"

class Map
{
    public:
        Map();

        void drawBack(sf::RenderWindow& window);
        void drawGround(sf::RenderWindow& window);

    private:
        sf::Sprite s_background;
        sf::Sprite s_ground;
};

#endif // MAP_HPP
