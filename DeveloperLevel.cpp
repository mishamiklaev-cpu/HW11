#include "DeveloperLevel.h"
#include <ResourceSystem.h>
#include "Logger.h"
#include "TransformComponent.h"
#include <cmath>

namespace Roguelike
{
	void DeveloperLevel::Start()
	{
		LOG_INFO("DeveloperLevel started");
		Engine::ResourceSystem::Instance()->LoadTextureMap("level_walls", "assets/walls.png", sf::Vector2u(128, 128), 4);
		Engine::ResourceSystem::Instance()->LoadTextureMap("level_floors", "assets/floors.png", sf::Vector2u(128, 128), 4);
		Engine::ResourceSystem::Instance()->LoadTextureMap("AI", "assets/enemy.png", sf::Vector2u(100, 100), 1);
		Engine::ResourceSystem::Instance()->LoadTexture("ball", "assets/ball.png");
		Engine::ResourceSystem::Instance()->LoadSoundBuffer("music", "assets/music.ogg");
		int width = 15;
		int height = 15;
		float tileSize = 128.0f;
		LOG_INFO("Creating floors...");
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				Engine::Vector2Df pos = { x * tileSize, y * tileSize };
				floors.push_back(std::make_unique<Floor>(pos, 0));
			}
		}
		LOG_INFO("Creating walls...");
		for (int y = 0; y < height + 1; y++)
		{
			walls.push_back(std::make_unique<Wall>(Engine::Vector2Df{ 0.0f, y * tileSize }, 0));
			walls.push_back(std::make_unique<Wall>(Engine::Vector2Df{ width * tileSize, y * tileSize }, 0));
		}
		for (int x = 0; x < width + 1; x++)
		{
			walls.push_back(std::make_unique<Wall>(Engine::Vector2Df{ x * tileSize, 0.0f }, 0));
			walls.push_back(std::make_unique<Wall>(Engine::Vector2Df{ x * tileSize, height * tileSize }, 0));
		}
		LOG_INFO("Creating player...");
		player = std::make_shared<Player>();
		auto playerTransform = player->GetGameObject()->GetComponent<Engine::TransformComponent>();
		playerTransform->SetWorldPosition(Engine::Vector2Df{ width / 2.0f * tileSize, height / 2.0f * tileSize });
		LOG_INFO("Creating AI enemy...");
		ai = std::make_shared<AI>(Engine::Vector2Df{ width / 3.0f * tileSize, height / 3.0f * tileSize }, player->GetGameObject());
		LOG_INFO("DeveloperLevel initialization complete");
	}
	void DeveloperLevel::Restart()
	{
		LOG_INFO("DeveloperLevel restarting...");
		Stop();
		Start();
	}
	void DeveloperLevel::Stop()
	{
		LOG_INFO("DeveloperLevel stopped");
		Engine::GameWorld::Instance()->Clear();
		player.reset();
		ai.reset();
		music.reset();
		walls.clear();
		floors.clear();
	}
	void DeveloperLevel::UpdateUI(sf::RenderWindow& window)
	{
		if (player)
		{
			player->UpdateUI(window);
		}
		if (ai && ai->IsAlive())
		{
			ai->UpdateUI(window);
		}
	}
	void DeveloperLevel::Update(float deltaTime)
	{
		if (ai && ai->IsAlive())
		{
			ai->Update(deltaTime);
		}
	}
	void DeveloperLevel::HandlePlayerAttack()
	{
		if (!player || !ai || !ai->IsAlive())
		{
			return;
		}

		auto playerTransform = player->GetGameObject()->GetComponent<Engine::TransformComponent>();
		auto aiTransform = ai->GetGameObject()->GetComponent<Engine::TransformComponent>();

		auto playerPos = playerTransform->GetWorldPosition();
		auto aiPos = aiTransform->GetWorldPosition();

		float dx = aiPos.x - playerPos.x;
		float dy = aiPos.y - playerPos.y;
		float distance = std::sqrt(dx * dx + dy * dy);

		float attackRange = 150.0f;

		if (distance <= attackRange)
		{
			LOG_INFO("Player attacks AI! Distance: " + std::to_string(distance));
			ai->TakeDamage(25.0f);
		}
		else
		{
			LOG_INFO("Player attack missed! Distance: " + std::to_string(distance) + " (too far)");
		}
	}
}