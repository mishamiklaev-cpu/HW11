#include "GameState.h"
#include "GameStatePlaying.h"
#include "GameStateGameOver.h"
#include "GameStatePauseMenu.h"
#include "GameStateMainMenu.h"
#include "GameStateRecords.h"
#include "GameStateGameWin.h"

namespace Roguelike
{
	GameState::GameState(GameStateType type, bool isExclusivelyVisible)
	{
		this->type = type;
		this->isExclusivelyVisible = isExclusivelyVisible;

		switch (type)
		{
		case GameStateType::MainMenu:
			data = std::make_shared<GameStateMainMenuData>();
			break;
		case GameStateType::Playing:
			data = std::make_shared<GameStatePlayingData>();
			break;
		case GameStateType::GameOver:
			data = std::make_shared<GameStateGameOverData>();
			break;
		case GameStateType::GameWin:
			data = std::make_shared<GameStateGameWinData>();
			break;
		case GameStateType::ExitDialog:
			data = std::make_shared<GameStatePauseMenuData>();
			break;
		case GameStateType::Records:
			data = std::make_shared<GameStateRecordsData>();
			break;
		default:
			break;
		}

		if (data)
		{
			data->Init();
		}
	}

	GameState::~GameState()
	{
	}

	void GameState::Update(float timeDelta)
	{
		if (data)
		{
			data->Update(timeDelta);
		}
	}

	void GameState::Draw(sf::RenderWindow& window)
	{
		if (data)
		{
			data->Draw(window);
		}
	}

	void GameState::HandleWindowEvent(sf::Event& event)
	{
		if (data)
		{
			data->HandleWindowEvent(event);
		}
	}
}