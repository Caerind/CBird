#include "../include/ResourceManager.hpp"

ResourceManager ResourceManager::mInstance;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

ResourceManager& ResourceManager::instance()
{
    return mInstance;
}

void ResourceManager::loadTexture(std::string const& id, std::string const& filename)
{
    mInstance.mTextures[id].loadFromFile(filename);
}

void ResourceManager::loadFont(std::string const& id, std::string const& filename)
{
    mInstance.mFonts[id].loadFromFile(filename);
}

void ResourceManager::loadSoundBuffer(std::string const& id, std::string const& filename)
{
    mInstance.mSoundBuffers[id].loadFromFile(filename);
}

sf::Texture& ResourceManager::getTexture(std::string const& id)
{
    return mInstance.mTextures[id];
}

sf::Font& ResourceManager::getFont(std::string const& id)
{
    return mInstance.mFonts[id];
}

sf::SoundBuffer& ResourceManager::getSoundBuffer(std::string const& id)
{
    return mInstance.mSoundBuffers[id];
}
