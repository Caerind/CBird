#include <map>
#include <vector>
#include <string>
#include <time.h>
#include <cstdlib>
#include <cmath>
#include <fstream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

////////////////////////////////////////////////////////////////////////////////
// RANDOM                                                                     //
////////////////////////////////////////////////////////////////////////////////
void initRandomGenerator(){srand(time(0));}
int getRandom(int a, int b){return a + rand()%(b-a);}

////////////////////////////////////////////////////////////////////////////////
// RESOURCE MANAGER                                                           //
////////////////////////////////////////////////////////////////////////////////
class ResourceManager
{
    public:
        static void loadTexture(std::string const& id, std::string const& filename){mInstance.mTextures[id].loadFromFile(filename);}
        static void loadFont(std::string const& id, std::string const& filename){mInstance.mFonts[id].loadFromFile(filename);}
        static void loadSoundBuffer(std::string const& id, std::string const& filename){mInstance.mSoundBuffers[id].loadFromFile(filename);}
        static sf::Texture& getTexture(std::string const& id){return mInstance.mTextures[id];}
        static sf::Font& getFont(std::string const& id){return mInstance.mFonts[id];}
        static sf::SoundBuffer& getSoundBuffer(std::string const& id){return mInstance.mSoundBuffers[id];}
    private:
        ResourceManager(){}
        ~ResourceManager(){}
        static ResourceManager mInstance;
        std::map<std::string,sf::Texture> mTextures;
        std::map<std::string,sf::Font> mFonts;
        std::map<std::string,sf::SoundBuffer> mSoundBuffers;
};
ResourceManager ResourceManager::mInstance;

////////////////////////////////////////////////////////////////////////////////
// MAP                                                                        //
////////////////////////////////////////////////////////////////////////////////
class Map
{
    public:
        Map()
		{
			m_background.setTexture(&ResourceManager::getTexture("back"));
			m_background.setTextureRect(sf::IntRect(getRandom(0,2) * 138,0,138,256));
			m_background.setSize(sf::Vector2f(138,256));
			m_background.setPosition(0,0);

			for (int i = -256; i < 10000; i+=7)
			{
				m_ground.push_back(sf::Sprite(ResourceManager::getTexture("ground")));
				m_ground.back().setPosition(i,200);
			}
		}
        void drawBack(sf::RenderWindow& window){window.setView(sf::View(sf::FloatRect(0,0,138,256))); window.draw(m_background);}
        void drawGround(sf::RenderWindow& window){for(auto s : m_ground)window.draw(s);}
    private:
        sf::RectangleShape m_background;
        std::vector<sf::Sprite> m_ground;
};

////////////////////////////////////////////////////////////////////////////////
// PIPE                                                                       //
////////////////////////////////////////////////////////////////////////////////
class Pipe
{
    public:
        Pipe() : m_score(0),m_color(getRandom(0,2))
		{
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
			m_scoreText.setPosition(15,30);
			m_scoreText.setCharacterSize(80);
			m_scoreText.setString(std::to_string(m_score));
			m_sound.setBuffer(ResourceManager::getSoundBuffer("door"));
		}

        void draw(sf::RenderWindow& window)
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
		
        void drawText(sf::RenderWindow& window)
		{
			window.draw(m_scoreText);
		}

        void update(float birdX)
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
				m_bottoms.push_back(bot);
			}
		}

        bool collide(sf::FloatRect rect)
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

    private:
        std::vector<sf::Sprite> m_tops;
        std::vector<sf::Sprite> m_bottoms;
        int m_score;
        int m_color;
        sf::Text m_scoreText;
        sf::Sound m_sound;
};

////////////////////////////////////////////////////////////////////////////////
// BIRD                                                                      //
////////////////////////////////////////////////////////////////////////////////
class Bird
{
    public:
        Bird() : m_type(getRandom(0,3)), m_fly(0)
		{
			m_sprite.setTexture(ResourceManager::getTexture("bird"));
			m_sprite.setTextureRect(sf::IntRect(0,12*m_type,17,12));
			m_sprite.setOrigin(9,6);
			m_sprite.setPosition(sf::Vector2f(-100,100));
			m_sound.setBuffer(ResourceManager::getSoundBuffer("wing"));
		}

        void update(sf::Time dt)
		{
			updateWings(dt);
			updateFall(dt);
			updateMove(dt);
		}
		
        void handleSpace()
		{
			if(m_sprite.getPosition().y > 0)
			{
				m_sprite.rotate(-30);
				if(m_sprite.getRotation() < 330)
				{
					m_sprite.setRotation(330);
				}
				m_sprite.move(0,-10);
				m_fly = 10;
				m_fallTimer = sf::Time::Zero;
				m_sound.play();
			}
		}

        void draw(sf::RenderWindow& window){window.draw(m_sprite);}

        sf::FloatRect getGlobalBounds()
		{
			// TODO : Improve bounds
			return m_sprite.getGlobalBounds();
		}

        void updatePaused(sf::Time dt)
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
		
        void updateDied(sf::Time dt)
		{
			updateFall(dt);
			if(m_sprite.getGlobalBounds().top + m_sprite.getGlobalBounds().height < 200)
			{
				m_sprite.move(0,m_fallTimer.asSeconds()*4);
			}
		}

    private:
        void updateWings(sf::Time dt)
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
		
        void updateFall(sf::Time dt)
		{
			m_fallTimer += dt;
			if(m_sprite.getRotation() <= 90 || m_sprite.getRotation() >= 270)
			{
				m_sprite.rotate(m_fallTimer.asSeconds()*2);
			}
		}
		
        void updateMove(sf::Time dt)
		{
			if(m_fly > 0)
			{
				m_fly--;
				m_sprite.move(0, -100*dt.asSeconds());
			}
			m_sprite.move(40 * dt.asSeconds(),m_fallTimer.asSeconds()*2);
		}

    private:
        sf::Sprite m_sprite;
        sf::Time m_wingsTimer;
        sf::Time m_fallTimer;
        int m_type;
        int m_fly;
        sf::Sound m_sound;
        bool m_pausedGoTop;
};

int main(int argc,char *argv[])
{
    initRandomGenerator();
	
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "");
	
    ResourceManager::loadFont("font","sansation.ttf");
	
    ResourceManager::loadSoundBuffer("fail","fail.wav");
    ResourceManager::loadSoundBuffer("wing","wing.wav");
    ResourceManager::loadSoundBuffer("door","door.wav");
	
    ResourceManager::loadTexture("bird","bird.png");
    ResourceManager::loadTexture("back","back.png");
    ResourceManager::loadTexture("ground","ground.png");
    ResourceManager::loadTexture("green","pipe-green.png");
    ResourceManager::loadTexture("red","pipe-red.png");
	
	bool retry = false;
	do
	{
		sf::View view(sf::FloatRect(-156,0,138,256));
		
		Bird bird;
        Map map;
        Pipe pipe;

        sf::Sound fail(ResourceManager::getSoundBuffer("fail"));

        sf::Clock clock;

        bool paused = true;
        bool died = false;

		sf::Text text;
        text.setFont(ResourceManager::getFont("font"));
        text.setString("Touch to play !");
		text.setCharacterSize(80);
        text.setPosition(530 - text.getGlobalBounds().width, 256 - text.getGlobalBounds().height / 2 );
		
        while (!died)
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
				{
					window.close();
				}
				if (event.type == sf::Event::LostFocus)
				{
                    fail.play();
					died = true;
				}
                if (event.type == sf::Event::TouchBegan)
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
        replay.setString("Touch to replay\nor ragequit");
		replay.setCharacterSize(80);
        replay.setPosition(530 - text.getGlobalBounds().width, 200 - text.getGlobalBounds().height / 2 );

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
                if (event.type == sf::Event::TouchBegan)
                {
                    wait = false;
                    retry = true;
                }
            }
        }
	} while (retry);
	return 0;
}