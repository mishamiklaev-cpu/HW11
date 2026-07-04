#include "AI.h"
#include "ResourceSystem.h"
#include "Logger.h"
#include "TransformComponent.h"

namespace Roguelike
{
    AI::AI(const Engine::Vector2Df& position, Engine::GameObject* player)
    {
        gameObject = Engine::GameWorld::Instance()->CreateGameObject("AI");
        auto transform = gameObject->GetComponent<Engine::TransformComponent>();
        transform->SetWorldPosition(position);

        auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
        renderer->SetTexture(*Engine::ResourceSystem::Instance()->GetTextureMapElementShared("AI", 0));
        renderer->SetPixelSize(100, 100);

        auto rigidbody = gameObject->AddComponent<Engine::RigidbodyComponent>();
        rigidbody->SetKinematic(false);

        auto follower = gameObject->AddComponent<Engine::FollowComponent>();
        follower->SetTarget(player);
        follower->SetSpeed(120.0f);
        follower->SetDetectionRadius(350.0f);

        auto collider = gameObject->AddComponent<Engine::SpriteColliderComponent>();
        LOG_INFO("AI enemy created successfully. HP: " + std::to_string(healthPoints) +
            ", Armor: " + std::to_string(armor));
    }

    Engine::GameObject* AI::GetGameObject()
    {
        return gameObject;
    }

    void AI::TakeDamage(float damage)
    {
        assert(damage >= 0 && "Damage cannot be negative");

        if (!IsAlive())
        {
            LOG_WARN("AI is already dead, ignoring damage");
            return;
        }

        float actualDamage = damage - armor;
        if (actualDamage < 0)
        {
            actualDamage = 0;
        }

        healthPoints -= actualDamage;

        if (healthPoints < 0)
        {
            healthPoints = 0;
        }

        LOG_WARN("AI took " + std::to_string(actualDamage) + " damage. HP: " +
            std::to_string(healthPoints) + "/" + std::to_string(maxHealthPoints));

        isFlashing = true;
        flashTimer = 0.3f;

        if (!IsAlive())
        {
            LOG_INFO("AI enemy died!");
            Engine::GameWorld::Instance()->DestroyGameObject(gameObject);
        }
    }

    void AI::Heal(float amount)
    {
        assert(amount >= 0 && "Heal amount cannot be negative");

        if (!IsAlive())
        {
            LOG_WARN("Cannot heal dead AI");
            return;
        }

        healthPoints += amount;
        if (healthPoints > maxHealthPoints)
        {
            healthPoints = maxHealthPoints;
        }

        LOG_INFO("AI healed for " + std::to_string(amount) + ". HP: " +
            std::to_string(healthPoints) + "/" + std::to_string(maxHealthPoints));
    }

    void AI::Update(float deltaTime)
    {
        if (isFlashing)
        {
            flashTimer -= deltaTime;
            if (flashTimer <= 0.0f)
            {
                isFlashing = false;
            }
        }
    }

    void AI::UpdateUI(sf::RenderWindow& window)
    {
        if (!IsAlive())
        {
            return;
        }

        if (!uiInitialized)
        {
            if (font.loadFromFile("assets/Fonts/Roboto-Regular.ttf"))
            {
                hpText.setFont(font);
                hpText.setCharacterSize(18);
                hpText.setFillColor(sf::Color::Red);
                uiInitialized = true;
            }
            else
            {
                LOG_ERROR("Failed to load font for AI UI!");
                return;
            }
        }

        auto transform = gameObject->GetComponent<Engine::TransformComponent>();
        auto worldPos = transform->GetWorldPosition();

        std::string hpString = "HP: " + std::to_string((int)healthPoints) + "/" + std::to_string((int)maxHealthPoints);
        hpText.setString(hpString);

        if (isFlashing)
        {
            hpText.setFillColor(sf::Color::Yellow);
        }
        else
        {
            hpText.setFillColor(sf::Color::Red);
        }

        sf::View originalView = window.getView();
        sf::Vector2f worldPosition(worldPos.x, worldPos.y);
        sf::Vector2i screenPosition = window.mapCoordsToPixel(worldPosition);

        window.setView(window.getDefaultView());

        float textWidth = hpText.getLocalBounds().width;
        hpText.setOrigin(textWidth / 2.0f, 0.0f);

        hpText.setPosition((float)screenPosition.x, (float)screenPosition.y - 80.0f);

        window.draw(hpText);

        window.setView(originalView);
    }
}