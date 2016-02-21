#include "../include/Pipe.hpp"

Pipe::Pipe()
{
    m_score = 0;
    m_color = getRandom(0,2);

    for(int i = 0; i < 3; ++i)
    {
        int b = getRandom(50,101);
        sf::Sprite top, bot;
        if(m_color == 0)
        {
            top.setTexture(ResourceManager::getTexture("green"));
            bot.setTexture(ResourceManager::getTexture("green"));
        }
        else
        {
            top.setTexture(ResourceManager::getTexture("red"));
            bot.setTexture(ResourceManager::getTexture("red"));
        }

        top.setTextureRect(sf::IntRect(0,0,26,160));
        top.setOrigin(0,160);
        top.setPosition(100*(m_score + i), b);

        bot.setTextureRect(sf::IntRect(26,0,26,160));
        bot.setPosition(100*(m_score + i), b + 56);

        m_tops.push_back(top);
        m_bottoms.push_back(bot);
    }

    m_scoreText.setFont(ResourceManager::getFont("font"));
    m_scoreText.setPosition(10,10);
    m_scoreText.setString(std::to_string(m_score));

    m_sound.setBuffer(ResourceManager::getSoundBuffer("door"));
}

void Pipe::draw(sf::RenderWindow& window)
{
    for(sf::Sprite& sprite : m_tops)
    {
        window.draw(sprite);
    }
    for(sf::Sprite& sprite : m_bottoms)
    {
        window.draw(sprite);
    }
}

void Pipe::drawDebug(sf::RenderWindow& window)
{
    for(std::size_t i = 0; i < m_tops.size(); i++)
    {
        sf::FloatRect r = m_tops[i].getGlobalBounds();
        sf::RectangleShape b;
        b.setPosition(r.left,r.top);
        b.setSize(sf::Vector2f(r.width,r.height));
        b.setFillColor(sf::Color::Transparent);
        b.setOutlineColor(sf::Color::Red);
        b.setOutlineThickness(1.f);
        window.draw(b);
    }
    for(std::size_t i = 0; i < m_bottoms.size(); i++)
    {
        sf::FloatRect r = m_bottoms[i].getGlobalBounds();
        sf::RectangleShape b;
        b.setPosition(r.left,r.top);
        b.setSize(sf::Vector2f(r.width,r.height));
        b.setFillColor(sf::Color::Transparent);
        b.setOutlineColor(sf::Color::Red);
        b.setOutlineThickness(1.f);
        window.draw(b);
    }
}

void Pipe::drawText(sf::RenderWindow& window)
{
    window.draw(m_scoreText);
}

void Pipe::update(float birdX)
{
    int a = static_cast<int>(floor(birdX/100)+1);
    if(a > m_score)
    {
        m_score = a;

        m_sound.play();

        m_scoreText.setString(std::to_string(m_score));

        int b = getRandom(50,101);

        sf::Sprite top, bot;
        if(m_color == 0)
        {
            top.setTexture(ResourceManager::getTexture("green"));
            bot.setTexture(ResourceManager::getTexture("green"));
        }
        else
        {
            top.setTexture(ResourceManager::getTexture("red"));
            bot.setTexture(ResourceManager::getTexture("red"));
        }

        top.setTextureRect(sf::IntRect(0,0,26,160));
        top.setOrigin(0,160);
        top.setPosition(100*(m_score+2), b);

        bot.setTextureRect(sf::IntRect(26,0,26,160));
        bot.setPosition(100*(m_score+2), b + 56);

        m_tops.push_back(top);
        m_bottoms.push_back(bot);
    }
}

bool Pipe::collide(sf::FloatRect rect)
{
    for(int i = 0; i < m_tops.size(); i++)
    {
        if(m_tops[i].getGlobalBounds().intersects(rect))
        {
            return true;
        }
    }
    for(int i = 0; i < m_bottoms.size(); i++)
    {
        if(m_bottoms[i].getGlobalBounds().intersects(rect))
        {
            return true;
        }
    }
    return false;
}



