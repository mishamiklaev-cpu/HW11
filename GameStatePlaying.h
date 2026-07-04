#pragma once
#include "GameStateData.h"
#include "DeveloperLevel.h"
#include "Music.h"
#include <memory>

namespace Roguelike
{
	class GameStatePlayingData : public GameStateData
	{
	public:
		void Init() override;
		void HandleWindowEvent(const sf::Event& event) override;
		void Update(float timeDelta) override;
		void Draw(sf::RenderWindow& window) override;

	private:
		std::shared_ptr<DeveloperLevel> developerLevel;
		std::unique_ptr<Music> music;
	};
}