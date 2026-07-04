#include "GameStateMainMenu.h"
#include "Application.h"
#include "Game.h"
#include "GameSettings.h"
#include <assert.h>
#include "Logger.h"

namespace Roguelike
{
    void GameStateMainMenuData::Init()
    {
        assert(font.loadFromFile(SETTINGS.FONTS_PATH + "Roboto-Regular.ttf"));
        LOG_INFO("GameStateMainMenu initialized.");

        background.setFillColor(sf::Color(0, 0, 0, 200));
        titleText.setString("ROGUELIKE");
        titleText.setFont(font);
        titleText.setCharacterSize(64);
        titleText.setFillColor(sf::Color::Yellow);
        titleText.setStyle(sf::Text::Bold);
        sf::Text startItem;
        startItem.setString("Start Game");
        startItem.setFont(font);
        startItem.setCharacterSize(32);
        startItem.setFillColor(sf::Color::White);
        menuItems.push_back(startItem);
        sf::Text recordsItem;
        recordsItem.setString("Records");
        recordsItem.setFont(font);
        recordsItem.setCharacterSize(32);
        recordsItem.setFillColor(sf::Color::White);
        menuItems.push_back(recordsItem);
        sf::Text exitItem;
        exitItem.setString("Exit");
        exitItem.setFont(font);
        exitItem.setCharacterSize(32);
        exitItem.setFillColor(sf::Color::White);
        menuItems.push_back(exitItem);
        hintText.setString("Use UP/DOWN arrows to navigate, ENTER to select");
        hintText.setFont(font);
        hintText.setCharacterSize(20);
        hintText.setFillColor(sf::Color::White);
    }

    void GameStateMainMenuData::HandleWindowEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Up)
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
                    Application::Instance().GetGame().StartGame();
                }
                else if (selectedMenuItem == 1)
                {
                    Application::Instance().GetGame().ShowRecords();
                }
                else if (selectedMenuItem == 2)
                {
                    Application::Instance().GetGame().QuitGame();
                }
            }
        }
        for (size_t i = 0; i < menuItems.size(); i++)
        {
            menuItems[i].setFillColor(i == selectedMenuItem ? sf::Color::Yellow : sf::Color::White);
        }
    }

    void GameStateMainMenuData::Update(float timeDelta)
    {
    }

    void GameStateMainMenuData::Draw(sf::RenderWindow& window)
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
        titleText.setPosition(viewSize.x / 2.f, 100);
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