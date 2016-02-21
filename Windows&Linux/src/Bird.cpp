#include "../include/Bird.hpp"

Bird::Bird()
{
    m_type = getRandom(0,3);
    m_sprite.setTexture(ResourceManager::getTexture("bird"));
    m_sprite.setTextureRect(sf::IntRect(0,12*m_type,17,12));
    m_sprite.setOrigin(9,6);
    m_sprite.setPosition(sf::Vector2f(-100,100));
    m_fly = 0;
    m_sound.setBuffer(ResourceManager::getSoundBuffer("wing"));
}

void Bird::update(sf::Time dt)
{
    updateWings(dt);
    updateFall(dt);
    updateMove(dt);
}

void Bird::handleSpace()
{
    if(m_sprite.getPosition().y > 0)
    {
        m_sprite.rotate(-30);
        if(m_sprite.getRotation() < 330)
        {
            m_sprite.setRotation(330);
        }

        m_sprite.move(0,-10);

        m_fly = 30;

        m_fallTimer = sf::Time::Zero;

        m_sound.play();
    }
}

void Bird::draw(sf::RenderWindow& window)
{
    window.draw(m_sprite);
}

void Bird::drawDebug(sf::RenderWindow& window)
{
    sf::FloatRect r = getGlobalBounds();
    sf::RectangleShape b;
    b.setPosition(r.left,r.top);
    b.setSize(sf::Vector2f(r.width,r.height));
    b.setFillColor(sf::Color::Transparent);
    b.setOutlineColor(sf::Color::Red);
    b.setOutlineThickness(1.f);
    window.draw(b);
}

void Bird::updateWings(sf::Time dt)
{
    m_wingsTimer += dt;
    if(m_wingsTimer < sf::seconds(0.125))
    {
        m_sprite.setTextureRect(sf::IntRect(0,12*m_type,17,12));
    }
    if(m_wingsTimer >= sf::seconds(0.125)  && m_wingsTimer < sf::seconds(0.25))
    {
        m_sprite.setTextureRect(sf::IntRect(17,12*m_type,17,12));
    }
    if(m_wingsTimer >= sf::seconds(0.25)  && m_wingsTimer < sf::seconds(0.375))
    {
        m_sprite.setTextureRect(sf::IntRect(34,12*m_type,17,12));
    }
    if(m_wingsTimer >= sf::seconds(0.375)  && m_wingsTimer < sf::seconds(0.5))
    {
        m_sprite.setTextureRect(sf::IntRect(17,12*m_type,17,12));
    }

    while(m_wingsTimer >= sf::seconds(0.5))
    {
        m_wingsTimer -= sf::seconds(0.5);
    }
}

void Bird::updateFall(sf::Time dt)
{
    m_fallTimer += dt;
    if(m_sprite.getRotation() <= 90 || m_sprite.getRotation() >= 270)
    {
        m_sprite.rotate(m_fallTimer.asSeconds()/10);
    }
}

void Bird::updateMove(sf::Time dt)
{
    if(m_fly > 0)
    {
        m_fly--;
        m_sprite.move(0, -600*dt.asSeconds());
    }

    m_sprite.move(40 * dt.asSeconds(),m_fallTimer.asSeconds()/16);
}

sf::FloatRect Bird::getGlobalBounds()
{
    return m_sprite.getGlobalBounds();
}

void Bird::updatePaused(sf::Time dt)
{
    updateWings(dt);
    if(m_pausedGoTop)
    {
        m_sprite.move(0, -50 * dt.asSeconds());
    }
    else
    {
        m_sprite.move(0, 50 * dt.asSeconds());
    }
    if(m_sprite.getPosition().y <= 75)
    {
        m_pausedGoTop = false;
    }
    if(m_sprite.getPosition().y >= 125)
    {
        m_pausedGoTop = true;
    }
}

void Bird::updateDied(sf::Time dt)
{
    updateFall(dt);

    if(m_sprite.getGlobalBounds().top + m_sprite.getGlobalBounds().height < 200)
    {
        m_sprite.move(0,m_fallTimer.asSeconds()/8);
    }
}




