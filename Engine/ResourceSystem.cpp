#include "pch.h"
#include "ResourceSystem.h"
#include "Logger.h"

namespace Engine
{
    ResourceSystem* ResourceSystem::Instance()
    {
        static ResourceSystem resourceSystem;
        return &resourceSystem;
    }

    void ResourceSystem::LoadTexture(const std::string& name, std::string sourcePath, bool isSmooth)
    {
        if (textures.find(name) != textures.end())
        {
            LOG_WARN("Texture already loaded: " + name);
            return;
        }
        sf::Texture* newTexture = new sf::Texture();
        if (newTexture->loadFromFile(sourcePath))
        {
            newTexture->setSmooth(isSmooth);
            textures.emplace(name, newTexture);
            LOG_INFO("Texture loaded: " + name);
        }
        else
        {
            LOG_ERROR("Failed to load texture: " + sourcePath);
            delete newTexture;
            throw ResourceLoadException(sourcePath);
        }
    }

    const sf::Texture* ResourceSystem::GetTextureShared(const std::string& name) const
    {
        auto it = textures.find(name);
        if (it != textures.end())
        {
            return it->second;
        }
        return nullptr;
    }

    sf::Texture* ResourceSystem::GetTextureCopy(const std::string& name) const
    {
        auto it = textures.find(name);
        if (it != textures.end())
        {
            return new sf::Texture(*it->second);
        }
        return nullptr;
    }

    void ResourceSystem::DeleteSharedTexture(const std::string& name)
    {
        auto texturePair = textures.find(name);
        if (texturePair != textures.end())
        {
            sf::Texture* deletingTexure = texturePair->second;
            textures.erase(texturePair);
            delete deletingTexure;
        }
    }

    void ResourceSystem::LoadTextureMap(const std::string& name, std::string sourcePath, sf::Vector2u elementPixelSize, int totalElements, bool isSmooth)
    {
        if (textureMaps.find(name) != textureMaps.end())
        {
            LOG_WARN("TextureMap already loaded: " + name);
            return;
        }
        sf::Texture textureMap;
        if (textureMap.loadFromFile(sourcePath))
        {
            auto textureMapElements = new std::vector<sf::Texture*>();
            auto textureSize = textureMap.getSize();
            int loadedElements = 0;
            for (int y = 0; y <= textureSize.y - elementPixelSize.y; y += elementPixelSize.y)
            {
                if (loadedElements == totalElements)
                {
                    break;
                }
                for (int x = 0; x <= textureSize.x - elementPixelSize.x; x += elementPixelSize.x)
                {
                    if (loadedElements == totalElements)
                    {
                        break;
                    }
                    sf::Texture* newTextureMapElement = new sf::Texture();
                    if (newTextureMapElement->loadFromFile(sourcePath, sf::IntRect(x, y, elementPixelSize.x, elementPixelSize.y)))
                    {
                        newTextureMapElement->setSmooth(isSmooth);
                        textureMapElements->push_back(newTextureMapElement);
                    }
                    loadedElements++;
                }
            }
            textureMaps.emplace(name, *textureMapElements);
            LOG_INFO("TextureMap loaded: " + name + " (" + std::to_string(loadedElements) + " elements)");
        }
        else
        {
            LOG_ERROR("Failed to load texture map: " + sourcePath);
            throw ResourceLoadException(sourcePath);
        }
    }

    const sf::Texture* ResourceSystem::GetTextureMapElementShared(const std::string& name, int elementIndex) const
    {
        auto textureMap = textureMaps.find(name);
        if (textureMap != textureMaps.end())
        {
            auto textures = textureMap->second;
            if (elementIndex >= 0 && elementIndex < textures.size())
            {
                return textures[elementIndex];
            }
        }
        return nullptr;
    }

    sf::Texture* ResourceSystem::GetTextureMapElementCopy(const std::string& name, int elementIndex) const
    {
        auto textureMap = textureMaps.find(name);
        if (textureMap != textureMaps.end())
        {
            auto textures = textureMap->second;
            if (elementIndex >= 0 && elementIndex < textures.size())
            {
                return new sf::Texture(*textures[elementIndex]);
            }
        }
        return nullptr;
    }

    int ResourceSystem::GetTextureMapElementsCount(const std::string& name) const
    {
        auto textureMap = textureMaps.find(name);
        if (textureMap != textureMaps.end())
        {
            return textureMap->second.size();
        }
        return 0;
    }

    void ResourceSystem::DeleteSharedTextureMap(const std::string& name)
    {
        auto textureMap = textureMaps.find(name);
        if (textureMap != textureMaps.end())
        {
            auto deletingTextures = textureMap->second;
            for (int i = 0; i < deletingTextures.size(); i++)
            {
                delete deletingTextures[i];
            }
            textureMaps.erase(textureMap);
        }
    }

    void ResourceSystem::Clear()
    {
        DeleteAllTextures();
        DeleteAllTextureMaps();
        DeleteAllSoundBuffers();
    }

    void ResourceSystem::DeleteAllTextures()
    {
        std::vector<std::string> keysToDelete;
        for (const auto& texturePair : textures)
        {
            keysToDelete.push_back(texturePair.first);
        }
        for (const auto& key : keysToDelete)
        {
            DeleteSharedTexture(key);
        }
    }

    void ResourceSystem::DeleteAllTextureMaps()
    {
        std::vector<std::string> keysToDelete;
        for (const auto& textureMapPair : textureMaps)
        {
            keysToDelete.push_back(textureMapPair.first);
        }
        for (const auto& key : keysToDelete)
        {
            DeleteSharedTextureMap(key);
        }
    }

    void ResourceSystem::LoadSoundBuffer(const std::string& name, std::string sourcePath)
    {
        if (soundBuffers.find(name) != soundBuffers.end())
        {
            LOG_WARN("SoundBuffer already loaded: " + name);
            return;
        }
        sf::SoundBuffer* newBuffer = new sf::SoundBuffer();
        if (newBuffer->loadFromFile(sourcePath))
        {
            soundBuffers.emplace(name, newBuffer);
            LOG_INFO("SoundBuffer loaded: " + name);
        }
        else
        {
            LOG_ERROR("Failed to load sound buffer: " + sourcePath);
            delete newBuffer;
            throw ResourceLoadException(sourcePath);
        }
    }

    const sf::SoundBuffer* ResourceSystem::GetSoundBufferShared(const std::string& name) const
    {
        auto it = soundBuffers.find(name);
        if (it != soundBuffers.end())
        {
            return it->second;
        }
        return nullptr;
    }

    void ResourceSystem::DeleteSharedSoundBuffer(const std::string& name)
    {
        auto it = soundBuffers.find(name);
        if (it != soundBuffers.end())
        {
            delete it->second;
            soundBuffers.erase(it);
        }
    }

    void ResourceSystem::DeleteAllSoundBuffers()
    {
        std::vector<std::string> keysToDelete;
        for (const auto& bufferPair : soundBuffers)
        {
            keysToDelete.push_back(bufferPair.first);
        }
        for (const auto& key : keysToDelete)
        {
            DeleteSharedSoundBuffer(key);
        }
    }
}