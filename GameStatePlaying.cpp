#include "GameStatePlaying.h"
#include "Application.h"
#include "GameWorld.h"
#include "RenderSystem.h"
#include "Logger.h"
#include "CrashHandler.h"
namespace Roguelike
{
	void GameStatePlayingData::Init()
	{
		LOG_INFO("GameStatePlaying initialized.");
		Engine::GameWorld::Instance()->Clear();
		Engine::RenderSystem::Instance()->SetMainWindow(&Application::Instance().GetWindow());
		developerLevel = std::make_shared<DeveloperLevel>();
		developerLevel->Start();
		music = std::make_unique<Music>("music");
	}
	void GameStatePlayingData::HandleWindowEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::Closed)
		{
			Application::Instance().GetGame().QuitGame();
		}
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				Application::Instance().GetGame().PauseGame();
			}
			else if (event.key.code == sf::Keyboard::Space)
			{
				developerLevel->HandlePlayerAttack();
			}
			else if (event.key.code == sf::Keyboard::F1)
			{
				LOG_WARN("Test crash triggered: division by zero (F1)");
				Engine::triggerTestCrash(1);
			}
			else if (event.key.code == sf::Keyboard::F2)
			{
				LOG_WARN("Test crash triggered: nullptr dereference (F2)");
				Engine::triggerTestCrash(2);
			}
			else if (event.key.code == sf::Keyboard::F3)
			{
				LOG_WARN("Test crash triggered: abort (F3)");
				Engine::triggerTestCrash(3);
			}
		}
	}
	void GameStatePlayingData::Update(float timeDelta)
	{
		Engine::GameWorld::Instance()->Update(timeDelta);
		Engine::GameWorld::Instance()->FixedUpdate(timeDelta);
		Engine::GameWorld::Instance()->LateUpdate();
		developerLevel->Update(timeDelta);
	}
	void GameStatePlayingData::Draw(sf::RenderWindow& window)
	{
		Engine::RenderSystem::Instance()->SetMainWindow(&window);
		Engine::GameWorld::Instance()->Render();
		developerLevel->UpdateUI(window);
	}
}