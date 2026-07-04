#pragma once
#include <string>

namespace Roguelike
{
	class GameWorld {
	private:
		GameWorld() = default;
	public:
		static GameWorld& Instance() {
			static GameWorld gameWorld;
			return gameWorld;
		}

		const std::string RESOURCES_PATH = "assets/";
		const std::string TEXTURES_PATH = RESOURCES_PATH;
		const std::string FONTS_PATH = RESOURCES_PATH + "Fonts/";
		const std::string SOUNDS_PATH = RESOURCES_PATH;

		const unsigned int SCREEN_WIDTH = 1280;
		const unsigned int SCREEN_HEIGHT = 720;
		const float TIME_PER_FRAME = 1.f / 60.f;

		const int MAX_RECORDS_TABLE_SIZE = 5;
		const char* PLAYER_NAME = "Player";

		const std::string GAME_NAME = "Roguelike";
	};
}

#define SETTINGS Roguelike::GameWorld::Instance()