#include "Application.h"
#include "GameSettings.h"
#include <cstdlib>

namespace Roguelike
{
	Application& Application::Instance()
	{
		static Application instance;
		return instance;
	}

	Application::Application() :
		window(sf::VideoMode(SETTINGS.SCREEN_WIDTH, SETTINGS.SCREEN_HEIGHT), SETTINGS.GAME_NAME)
	{
		unsigned int seed = (unsigned int)time(nullptr);
		srand(seed);
	}

	void Application::Run()
	{
		sf::Clock gameClock;

		while (window.isOpen()) {
			float startTime = gameClock.getElapsedTime().asSeconds();

			if (!window.isOpen()) {
				break;
			}
			game.UpdateGame(SETTINGS.TIME_PER_FRAME, window);

			float endTime = gameClock.getElapsedTime().asSeconds();
			float deltaTime = endTime - startTime;
			if (deltaTime < SETTINGS.TIME_PER_FRAME) {
				sf::sleep(sf::seconds(SETTINGS.TIME_PER_FRAME - deltaTime));
			}
		}
	}
}