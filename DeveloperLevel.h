#pragma once
#include <iostream>
#include <array>
#include <memory>
#include <vector>
#include <Scene.h>
#include "Player.h"
#include "AI.h"
#include "Music.h"
#include "Floor.h"
#include "Wall.h"
namespace Roguelike
{
	class DeveloperLevel : public Engine::Scene
	{
	public:
		void Start() override;
		void Restart() override;
		void Stop() override;
		void UpdateUI(sf::RenderWindow& window);
		void Update(float deltaTime);
		void HandlePlayerAttack();
	private:
		std::shared_ptr<Player> player;
		std::shared_ptr<AI> ai;
		std::unique_ptr<Music> music;
		std::vector<std::unique_ptr<Wall>> walls;
		std::vector<std::unique_ptr<Floor>> floors;
	};
}