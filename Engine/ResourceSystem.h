#pragma once

#include <map>
#include <string>
#include <vector>
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace Engine
{
	class ResourceLoadException : public std::runtime_error
	{
	public:
		explicit ResourceLoadException(const std::string& resourceName)
			: std::runtime_error("Failed to load resource: " + resourceName) {}
	};

	class ResourceSystem
	{
	public:
		static ResourceSystem* Instance();

		void LoadTexture(const std::string& name, std::string sourcePath, bool isSmooth = true);
		const sf::Texture* GetTextureShared(const std::string& name) const;
		sf::Texture* GetTextureCopy(const std::string& name) const;
		void DeleteSharedTexture(const std::string& name);

		void LoadTextureMap(const std::string& name, std::string sourcePath, sf::Vector2u elementPixelSize, int totalElements, bool isSmooth = true);
		const sf::Texture* GetTextureMapElementShared(const std::string& name, int elementIndex) const;
		sf::Texture* GetTextureMapElementCopy(const std::string& name, int elementIndex) const;
		int GetTextureMapElementsCount(const std::string& name) const;
		void DeleteSharedTextureMap(const std::string& name);

		void LoadSoundBuffer(const std::string& name, std::string sourcePath);
		const sf::SoundBuffer* GetSoundBufferShared(const std::string& name) const;
		void DeleteSharedSoundBuffer(const std::string& name);

		void Clear();

	private:
		std::map<std::string, sf::Texture*> textures;
		std::map<std::string, std::vector<sf::Texture*>> textureMaps;
		std::map<std::string, sf::SoundBuffer*> soundBuffers;

		ResourceSystem() {}
		~ResourceSystem() {}

		ResourceSystem(ResourceSystem const&) = delete;
		ResourceSystem& operator= (ResourceSystem const&) = delete;

		void DeleteAllTextures();
		void DeleteAllTextureMaps();
		void DeleteAllSoundBuffers();
	};
}