#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <map>

class ResourceManager
{
    public:
        static ResourceManager& instance();

        static void loadTexture(std::string const& id, std::string const& filename);
        static void loadFont(std::string const& id, std::string const& filename);
        static void loadSoundBuffer(std::string const& id, std::string const& filename);

        static sf::Texture& getTexture(std::string const& id);
        static sf::Font& getFont(std::string const& id);
        static sf::SoundBuffer& getSoundBuffer(std::string const& id);

    private:
        ResourceManager();
        ~ResourceManager();

        static ResourceManager mInstance;

        std::map<std::string,sf::Texture> mTextures;
        std::map<std::string,sf::Font> mFonts;
        std::map<std::string,sf::SoundBuffer> mSoundBuffers;
};

#endif // RESOURCEMANAGER_HPP
