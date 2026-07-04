#include "GameSettings.h"
#include "GameStatePauseMenu.h"
#include "Application.h"
#include "Game.h"
#include <assert.h>
#include "Logger.h"

namespace Roguelike
{
    void GameStatePauseMenuData::Init()
    {
        assert(font.loadFromFile("assets/Fonts/Roboto-Regular.ttf"));
        LOG_INFO("GameStatePauseMenu initialized.");

        background.setFillColor(sf::Color(0, 0, 0, 180));
        titleText.setString("PAUSE");
        titleText.setFont(font);
        titleText.setCharacterSize(48);
        titleText.setFillColor(sf::Color::Red);
        titleText.setStyle(sf::Text::Bold);
        sf::Text resumeItem;
        resumeItem.setString("Resume");
        resumeItem.setFont(font);
        resumeItem.setCharacterSize(32);
        resumeItem.setFillColor(sf::Color::White);
        menuItems.push_back(resumeItem);
        sf::Text exitItem;
        exitItem.setString("Exit to Main Menu");
        exitItem.setFont(font);
        exitItem.setCharacterSize(32);
        exitItem.setFillColor(sf::Color::White);
        menuItems.push_back(exitItem);
        hintText.setString("Use UP/DOWN arrows, ENTER to select, ESC to resume");
        hintText.setFont(font);
        hintText.setCharacterSize(20);
        hintText.setFillColor(sf::Color::White);
    }

    void GameStatePauseMenuData::HandleWindowEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                Application::Instance().GetGame().PopState();
            }
            else if (event.key.code == sf::Keyboard::Up)
            {
                selectedMenuItem--;
                if (selectedMenuItem < 0) selectedMenuItem = menuItems.size() - 1;
            }
            else if (event.key.code == sf::Keyboard::Down)
            {
                selectedMenuItem++;
                if (selectedMenuItem >= menuItems.size()) selectedMenuItem = 0;
            }
            else if (event.key.code == sf::Keyboard::Enter)
            {
                if (selectedMenuItem == 0)
                {
                    Application::Instance().GetGame().PopState();
                }
                else if (selectedMenuItem == 1)
                {
                    Application::Instance().GetGame().ExitGame();
                }
            }
        }
        for (size_t i = 0; i < menuItems.size(); i++)
        {
            menuItems[i].setFillColor(i == selectedMenuItem ? sf::Color::Yellow : sf::Color::White);
        }
    }

    void GameStatePauseMenuData::Update(float timeDelta)
    {
    }

    void GameStatePauseMenuData::Draw(sf::RenderWindow& window)
    {
        sf::View originalView = window.getView();
        sf::View screenView(sf::FloatRect(0.f, 0.f,
            static_cast<float>(SETTINGS.SCREEN_WIDTH),
            static_cast<float>(SETTINGS.SCREEN_HEIGHT)));
        window.setView(screenView);
        sf::Vector2f viewSize = window.getView().getSize();
        background.setSize(viewSize);
        window.draw(background);
        titleText.setOrigin(titleText.getLocalBounds().width / 2.f, 0.f);
        titleText.setPosition(viewSize.x / 2.f, 150);
        window.draw(titleText);
        float startY = viewSize.y / 2.f - (menuItems.size() * 40) / 2.f;
        for (size_t i = 0; i < menuItems.size(); i++)
        {
            menuItems[i].setOrigin(menuItems[i].getLocalBounds().width / 2.f, 0.f);
            menuItems[i].setPosition(viewSize.x / 2.f, startY + i * 50);
            window.draw(menuItems[i]);
        }
        hintText.setOrigin(hintText.getLocalBounds().width / 2.f, 0.f);
        hintText.setPosition(viewSize.x / 2.f, viewSize.y - 100);
        window.draw(hintText);
        window.setView(originalView);
    }
}