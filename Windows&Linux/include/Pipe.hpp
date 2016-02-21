#ifndef PIPE_HPP
#define PIPE_HPP

#include <SFML/Graphics.hpp>
#include <math.h>
#include "Random.hpp"
#include "ResourceManager.hpp"
#include <SFML/Audio.hpp>

class Pipe
{
    public:
        Pipe();

        void draw(sf::RenderWindow& window);
        void drawDebug(sf::RenderWindow& window);
        void drawText(sf::RenderWindow& window);

        void update(float birdX);

        bool collide(sf::FloatRect rect);

    private:
        std::vector<sf::Sprite> m_tops;
        std::vector<sf::Sprite> m_bottoms;
        int m_score;
        int m_color;
        sf::Text m_scoreText;
        sf::Sound m_sound;
};

#endif // PIPE_HPP
