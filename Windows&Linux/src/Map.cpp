#include "../include/Map.hpp"

Map::Map()
{
    int a = getRandom(0,2);
    if(a == 0)
    {
        s_background.setTexture(ResourceManager::getTexture("day"));
    }
    else
    {
        s_background.setTexture(ResourceManager::getTexture("night"));
    }
    s_background.setTextureRect(sf::IntRect(0,0,256,256));
    s_background.setPosition(0,0);

    s_ground.setTexture(ResourceManager::getTexture("ground"));
    s_ground.setTextureRect(sf::IntRect(0,0,10256,56));
    s_ground.setPosition(-256,200);
}

void Map::drawBack(sf::RenderWindow& window)
{
    window.setView(sf::View(sf::FloatRect(0,0,256,256)));
    window.draw(s_background);
}

void Map::drawGround(sf::RenderWindow& window)
{
    window.draw(s_ground);
}
