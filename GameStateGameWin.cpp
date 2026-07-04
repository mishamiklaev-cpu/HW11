#include "GameStateGameWin.h"
#include "Application.h"
#include "Game.h"
#include <assert.h>
#include "Logger.h"

namespace Roguelike
{
    void GameStateGameWinData::Init()
    {
        assert(font.loadFromFile("assets/Roboto-Regular.ttf"));
        LOG_INFO("GameStateGameWin initialized.");

        timeSinceGameWin = 0.f;
        background.setFillColor(sf::Color(0, 0, 0, 200));
        gameWinText.setFont(font);
        gameWinText.setCharacterSize(48);
        gameWinText.setStyle(sf::Text::Bold);
        gameWinText.setFillColor(sf::Color::Green);
        gameWinText.setString("YOU WIN!");
        hintText.setFont(font);
        hintText.setCharacterSize(24);
        hintText.setFillColor(sf::Color::White);
        hintText.setString("Press Space to restart\nEsc to exit to main menu");
    }

    void GameStateGameWinData::HandleWindowEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Space)
            {
                Application::Instance().GetGame().StartGame();
            }
            else if (event.key.code == sf::Keyboard::Escape)
            {
                Application::Instance().GetGame().ExitGame();
            }
        }
    }

    void GameStateGameWinData::Update(float timeDelta)
    {
        timeSinceGameWin += timeDelta;
        sf::Color gameWinTextColor = (int)timeSinceGameWin % 2 ? sf::Color::Green : sf::Color::Yellow;
        gameWinText.setFillColor(gameWinTextColor);
    }

    void GameStateGameWinData::Draw(sf::RenderWindow& window)
    {
        sf::Vector2f viewSize = window.getView().getSize();
        background.setSize(viewSize);
        window.draw(background);
        gameWinText.setOrigin(gameWinText.getLocalBounds().width / 2.f, gameWinText.getLocalBounds().height);
        gameWinText.setPosition(viewSize.x / 2.f, viewSize.y / 2.f);
        window.draw(gameWinText);
        hintText.setOrigin(hintText.getLocalBounds().width / 2.f, 0.f);
        hintText.setPosition(viewSize.x / 2.f, viewSize.y - 100.f);
        window.draw(hintText);
    }
}