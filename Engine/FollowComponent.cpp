#include "pch.h"
#include "FollowComponent.h"
#include <cmath>
#include "Logger.h"
namespace Engine
{
    FollowComponent::FollowComponent(GameObject* gameObject)
        : Component(gameObject)
    {
        transform = gameObject->GetComponent<TransformComponent>();
        rigidbody = gameObject->GetComponent<RigidbodyComponent>();
        spriteRenderer = gameObject->GetComponent<SpriteRendererComponent>();
        if (transform == nullptr)
        {
            LOG_ERROR("FollowComponent requires a TransformComponent.");
            gameObject->RemoveComponent(this);
        }
        if (rigidbody == nullptr)
        {
            LOG_ERROR("FollowComponent requires a RigidbodyComponent.");
        }
    }
    void FollowComponent::SetTarget(GameObject* targetObject)
    {
        if (targetObject)
        {
            targetTransform = targetObject->GetComponent<TransformComponent>();
            if (targetTransform == nullptr)
            {
                LOG_ERROR("Target object has no TransformComponent.");
            }
        }
    }
    void FollowComponent::SetSpeed(float newSpeed)
    {
        speed = newSpeed;
    }
    float FollowComponent::GetSpeed() const
    {
        return speed;
    }
    void FollowComponent::SetDetectionRadius(float radius)
    {
        detectionRadius = radius;
    }
    void FollowComponent::Update(float deltaTime)
    {
        if (transform == nullptr || targetTransform == nullptr || rigidbody == nullptr) return;
        Vector2Df myPos = transform->GetWorldPosition();
        Vector2Df targetPos = targetTransform->GetWorldPosition();
        float dx = targetPos.x - myPos.x;
        float dy = targetPos.y - myPos.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance < detectionRadius && distance > 0.1f)
        {
            Vector2Df direction = { dx / distance, dy / distance };
            rigidbody->SetLinearVelocity({ direction.x * speed, direction.y * speed });
        }
        else
        {
            rigidbody->SetLinearVelocity({ 0.0f, 0.0f });
        }
    }
    void FollowComponent::Render() {}
}