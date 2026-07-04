#include "Player.h"
#include "AI.h"
#include <ResourceSystem.h>
#include <SpriteColliderComponent.h>
#include "Application.h"
#include "PlayerMovementComponent.h"
#include "Logger.h"

namespace Roguelike
{
    Player::Player()
    {
        gameObject = Engine::GameWorld::Instance()->CreateGameObject("Player");
        auto transform = gameObject->GetComponent<Engine::TransformComponent>();
        transform->SetWorldPosition(Engine::Vector2Df{ 960.0f, 540.0f });

        auto playerRenderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
        const auto* texture = Engine::ResourceSystem::Instance()->GetTextureShared("ball");
        if (texture)
        {
            playerRenderer->SetTexture(*texture);
        }
        else
        {
            LOG_ERROR("Failed to load player texture 'ball'!");
        }

        playerRenderer->SetPixelSize(64, 64);

        auto playerCamera = gameObject->AddComponent<Engine::CameraComponent>();
        playerCamera->SetWindow(&Application::Instance().GetWindow());
        playerCamera->SetBaseResolution(1280, 720);

        auto playerInput = gameObject->AddComponent<Engine::InputComponent>();
        auto body = gameObject->AddComponent<Engine::RigidbodyComponent>();
        body->SetLinearDamping(15.0f);

        auto movement = gameObject->AddComponent<Engine::PlayerMovementComponent>();
        movement->SetSpeed(200.0f);

        auto collider = gameObject->AddComponent<Engine::SpriteColliderComponent>();
        collider->SubscribeCollision([this](Engine::Collision collision)
            {
                auto myCollider = this->gameObject->GetComponent<Engine::SpriteColliderComponent>();
                if (collision.GetFirst() != myCollider && collision.GetSecond() != myCollider)
                {
                    return;
                }
                Engine::ColliderComponent* otherCollider = nullptr;
                if (collision.GetFirst() == myCollider)
                {
                    otherCollider = collision.GetSecond();
                }
                else
                {
                    otherCollider = collision.GetFirst();
                }
                if (otherCollider != nullptr)
                {
                    auto otherObject = otherCollider->GetGameObject();
                    if (otherObject != nullptr && otherObject->GetName() == "AI")
                    {
                        LOG_WARN("Player collided with AI! AI attacks player.");
                        this->TakeDamage(15.0f);
                    }
                }
            });
        LOG_INFO("Player created successfully. HP: " + std::to_string(healthPoints) + ", Armor: " + std::to_string(armor));
    }

    Engine::GameObject* Player::GetGameObject()
    {
        return gameObject;
    }

    void Player::UpdateUI(sf::RenderWindow& window)
    {
        if (!uiInitialized)
        {
            if (font.loadFromFile("assets/Fonts/Roboto-Regular.ttf"))
            {
                hpText.setFont(font);
                hpText.setCharacterSize(24);
                hpText.setFillColor(sf::Color::White);
                uiInitialized = true;
                LOG_INFO("Player UI initialized");
            }
            else
            {
                LOG_ERROR("Failed to load font for Player UI!");
                return;
            }
        }

        std::string hpString = "HP: " + std::to_string((int)healthPoints) + "/" + std::to_string((int)maxHealthPoints) +
            " | Armor: " + std::to_string((int)armor);
        hpText.setString(hpString);

        sf::View originalView = window.getView();
        sf::View screenView(sf::FloatRect(0.f, 0.f, 1280.f, 720.f));
        window.setView(screenView);

        hpText.setPosition(20.f, 20.f);
        window.draw(hpText);

        window.setView(originalView);
    }

    void Player::TakeDamage(float damage)
    {
        assert(damage >= 0 && "Damage cannot be negative");

        if (!IsAlive())
        {
            LOG_WARN("Player is already dead, ignoring damage");
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

        LOG_WARN("Player took " + std::to_string(actualDamage) + " damage. HP: " +
            std::to_string(healthPoints) + "/" + std::to_string(maxHealthPoints));

        if (!IsAlive())
        {
            LOG_ERROR("Player died!");
            Application::Instance().GetGame().LooseGame();
        }
    }

    void Player::Heal(float amount)
    {
        assert(amount >= 0 && "Heal amount cannot be negative");

        if (!IsAlive())
        {
            LOG_WARN("Cannot heal dead player");
            return;
        }

        healthPoints += amount;
        if (healthPoints > maxHealthPoints)
        {
            healthPoints = maxHealthPoints;
        }

        LOG_INFO("Player healed for " + std::to_string(amount) + ". HP: " +
            std::to_string(healthPoints) + "/" + std::to_string(maxHealthPoints));
    }
}