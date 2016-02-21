#ifndef BIRD_HPP
#define BIRD_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include "Random.hpp"
#include "ResourceManager.hpp"

class Bird
{
    public:
        Bird();

        void update(sf::Time dt);
        void handleSpace();
        void draw(sf::RenderWindow& window);
        void drawDebug(sf::RenderWindow& window);

        sf::FloatRect getGlobalBounds();

        void updatePaused(sf::Time dt);
        void updateDied(sf::Time dt);

    private:
        void updateWings(sf::Time dt);
        void updateFall(sf::Time dt);
        void updateMove(sf::Time dt);

    private:
        sf::Sprite m_sprite;
        sf::Time m_wingsTimer;
        sf::Time m_fallTimer;
        int m_type;
        int m_fly;
        sf::Sound m_sound;
        bool m_pausedGoTop;
};

#endif // BIRD_HPP
