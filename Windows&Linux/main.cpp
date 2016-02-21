#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "include/Bird.hpp"
#include "include/Map.hpp"
#include "include/Random.hpp"
#include "include/Pipe.hpp"
#include "include/ResourceManager.hpp"

int main()
{
    initRandomGenerator();

    sf::RenderWindow window(sf::VideoMode(512, 512), "CBird");

    ResourceManager::loadFont("font","assets/dimis.ttf");
    ResourceManager::loadSoundBuffer("fail","assets/fail.wav");
    ResourceManager::loadSoundBuffer("wing","assets/wing.wav");
    ResourceManager::loadSoundBuffer("door","assets/door.wav");
    ResourceManager::loadTexture("bird","assets/bird.png");
    ResourceManager::loadTexture("day","assets/background-day.png");
    ResourceManager::getTexture("day").setRepeated(true);
    ResourceManager::loadTexture("night","assets/background-night.png");
    ResourceManager::getTexture("night").setRepeated(true);
    ResourceManager::loadTexture("ground","assets/ground.png");
    ResourceManager::getTexture("ground").setRepeated(true);
    ResourceManager::loadTexture("green","assets/pipe-green.png");
    ResourceManager::loadTexture("red","assets/pipe-red.png");

    bool retry = false;
    do
    {
        sf::View view(sf::FloatRect(-156,0,256,256));

        Bird bird;

        Map map;

        Pipe pipe;

        sf::Sound fail(ResourceManager::getSoundBuffer("fail"));

        sf::Clock clock;

        bool paused = true;
        bool died = false;
        bool debug = false;

        sf::Text text;
        text.setFont(ResourceManager::getFont("font"));
        text.setString("Press Space to play !");
        text.setPosition(512 - text.getGlobalBounds().width - 30, 256 - text.getGlobalBounds().height / 2 );

        while (!died)
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
                {
                    if(paused)
                    {
                        paused = false;
                    }
                    if(!died)
                    {
                        bird.handleSpace();
                    }
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F3)
                {
                    debug = !debug;
                }
            }

            sf::Time dt = clock.restart();

            if(!paused && !died)
            {
                bird.update(dt);
                view.move(40 * dt.asSeconds(),0);

                if(pipe.collide(bird.getGlobalBounds()) || bird.getGlobalBounds().top + bird.getGlobalBounds().height > 200)
                {
                    fail.play();
                    died = true;
                }

                pipe.update(bird.getGlobalBounds().left);
            }
            else if (paused && !died)
            {
                bird.updatePaused(dt);
            }
            else
            {
                bird.updateDied(dt);
            }

            window.clear();

            map.drawBack(window);

            window.setView(view);

            pipe.draw(window);
            map.drawGround(window);
            bird.draw(window);

            if (debug)
            {
                bird.drawDebug(window);
                pipe.drawDebug(window);
            }

            window.setView(window.getDefaultView());

            pipe.drawText(window);

            if(paused)
            {
                window.draw(text);
            }

            window.display();
        }

        do
        {
            bird.updateDied(clock.restart());
            window.clear();
            map.drawBack(window);
            window.setView(view);
            pipe.draw(window);
            map.drawGround(window);
            bird.draw(window);
            window.setView(window.getDefaultView());
            pipe.drawText(window);
            window.display();
        } while (bird.getGlobalBounds().top + bird.getGlobalBounds().height < 200);

        sf::Text replay;
        replay.setFont(ResourceManager::getFont("font"));
        replay.setString("Press Space to play again\nor Escape to quit");
        replay.setPosition(400 - text.getGlobalBounds().width - 30, 200 - text.getGlobalBounds().height / 2 );

        window.clear();
        map.drawBack(window);
        window.setView(view);
        pipe.draw(window);
        map.drawGround(window);
        bird.draw(window);
        window.setView(window.getDefaultView());
        pipe.drawText(window);
        window.draw(replay);
        window.display();

        bool wait = true;
        sf::Event event;
        while (wait)
        {
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
                {
                    wait = false;
                    retry = true;
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                {
                    wait = false;
                    retry = false;
                }
            }
        }

    } while (retry);

    return 0;
}
