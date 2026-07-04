#include "pch.h"
#include "SpriteColliderComponent.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "RenderSystem.h"
#include "Logger.h"
namespace Engine
{
    SpriteColliderComponent::SpriteColliderComponent(GameObject* gameObject) : ColliderComponent(gameObject)
    {
        auto spriteRenderer = gameObject->GetComponent<SpriteRendererComponent>();
        if (spriteRenderer == nullptr)
        {
            LOG_ERROR("SpriteRenderer required for SpriteCollider.");
            gameObject->RemoveComponent(this);
            return;
        }
        sprite = const_cast<sf::Sprite*>(spriteRenderer->GetSprite());
        transform = gameObject->GetComponent<TransformComponent>();
        spriteRendererComponent = spriteRenderer;
        PhysicsSystem::Instance()->Subscribe(this);
    }
    SpriteColliderComponent::~SpriteColliderComponent()
    {
        PhysicsSystem::Instance()->Unsubscribe(this);
    }
    void SpriteColliderComponent::Update(float deltaTime)
    {
        if (sprite != nullptr && transform != nullptr)
        {
            auto spriteTexture = sprite->getTexture();
            if (spriteTexture != nullptr)
            {
                auto textureSize = spriteTexture->getSize();
                auto scale = sprite->getScale();
                float width = textureSize.x * std::abs(scale.x);
                float height = textureSize.y * std::abs(scale.y);
                auto position = transform->GetWorldPosition();
                bounds = sf::FloatRect(
                    position.x - width / 2.0f,
                    position.y - height / 2.0f,
                    width,
                    height
                );
            }
        }
    }
    void SpriteColliderComponent::Render()
    {
    }
}