#include "GameStateRecords.h"
#include "Application.h"
#include "Game.h"
#include "GameSettings.h"
#include <assert.h>
#include <sstream>
#include "Logger.h"
namespace Roguelike
{
    void GameStateRecordsData::Init()
    {
        if (!font.loadFromFile(SETTINGS.FONTS_PATH + "Roboto-Regular.ttf"))
        {
            LOG_WARN("Could not load font for Records screen!");
            return;
        }
        LOG_INFO("GameStateRecords initialized.");
        titleText.setString("RECORDS");
        titleText.setFont(font);
        titleText.setFillColor(sf::Color::Red);
        titleText.setCharacterSize(48);
        tableTexts.reserve(SETTINGS.MAX_RECORDS_TABLE_SIZE);
        const Game& game = Application::Instance().GetGame();
        std::map<int, std::string> sortedRecordsTable;
        for (const auto& item : game.GetRecordsTable())
        {
            sortedRecordsTable[item.second] = item.first;
        }
        auto it = sortedRecordsTable.rbegin();
        for (int i = 0; i < SETTINGS.MAX_RECORDS_TABLE_SIZE && it != sortedRecordsTable.rend(); ++i, ++it)
        {
            tableTexts.emplace_back();
            sf::Text& text = tableTexts.back();
            std::stringstream sstream;
            sstream << i + 1 << ". " << it->second << ": " << it->first;
            text.setString(sstream.str());
            text.setFont(font);
            text.setFillColor(sf::Color::White);
            text.setCharacterSize(24);
        }
        hintText.setString("Press ESC to return back to main menu");
        hintText.setFont(font);
        hintText.setFillColor(sf::Color::White);
        hintText.setCharacterSize(24);
    }
    void GameStateRecordsData::HandleWindowEvent(const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Escape)
            {
                Application::Instance().GetGame().PopState();
            }
        }
    }
    void GameStateRecordsData::Update(float timeDelta)
    {
    }
    void GameStateRecordsData::Draw(sf::RenderWindow& window)
    {
        sf::Vector2f viewSize = window.getView().getSize();
        titleText.setOrigin(titleText.getLocalBounds().width / 2.f, 0.f);
        titleText.setPosition(viewSize.x / 2.f, 50.f);
        window.draw(titleText);
        float startY = 150.f;
        for (size_t i = 0; i < tableTexts.size(); i++)
        {
            tableTexts[i].setOrigin(tableTexts[i].getLocalBounds().width / 2.f, 0.f);
            tableTexts[i].setPosition(viewSize.x / 2.f, startY + i * 40);
            window.draw(tableTexts[i]);
        }
        hintText.setOrigin(hintText.getLocalBounds().width / 2.f, 0.f);
        hintText.setPosition(viewSize.x / 2.f, viewSize.y - 100.f);
        window.draw(hintText);
    }
}