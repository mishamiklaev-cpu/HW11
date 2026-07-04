#include "pch.h"
#include "PhysicsSystem.h"
#include "Logger.h"
namespace Engine
{
    PhysicsSystem* PhysicsSystem::Instance()
    {
        static PhysicsSystem physicsSystem;
        return &physicsSystem;
    }
    float PhysicsSystem::GetFixedDeltaTime() const
    {
        return fixedDeltaTime;
    }
    void PhysicsSystem::Update()
    {
        float dt = fixedDeltaTime;
        for (int i = 0; i < colliders.size(); i++)
        {
            auto body = colliders[i]->GetGameObject()->GetComponent<RigidbodyComponent>();
            if (body == nullptr || body->GetKinematic())
            {
                continue;
            }
            auto transform = colliders[i]->GetGameObject()->GetComponent<TransformComponent>();
            transform->MoveBy(body->GetLinearVelocity() * dt);
            transform->RotateBy(body->GetAngleVelocity() * dt);
        }
        for (int i = 0; i < colliders.size(); i++)
        {
            colliders[i]->Update(dt);
        }
        for (int i = 0; i < colliders.size(); i++)
        {
            auto body = colliders[i]->GetGameObject()->GetComponent<RigidbodyComponent>();
            if (body == nullptr || body->GetKinematic())
            {
                continue;
            }
            for (int j = 0; j < colliders.size(); j++)
            {
                if (j == i)
                {
                    continue;
                }
                sf::FloatRect intersection;
                if (colliders[i]->GetBounds().intersects(colliders[j]->GetBounds(), intersection))
                {
                    float intersectionWidth = intersection.width;
                    float intersectionHeight = intersection.height;
                    Vector2Df intersectionPosition = { intersection.left - 0.5f * intersectionWidth, intersection.top - 0.5f * intersectionHeight };
                    Vector2Df aPosition = { colliders[i]->GetBounds().left,  colliders[i]->GetBounds().top };
                    auto aTransform = colliders[i]->GetGameObject()->GetComponent<TransformComponent>();
                    if (intersectionWidth > intersectionHeight)
                    {
                        if (intersectionPosition.y > aPosition.y)
                        {
                            aTransform->MoveBy({ 0, -intersectionHeight });
                            body->SetLinearVelocity({ body->GetLinearVelocity().x, 0.f });
                        }
                        else
                        {
                            aTransform->MoveBy({ 0, intersectionHeight });
                            body->SetLinearVelocity({ body->GetLinearVelocity().x, 0.f });
                        }
                    }
                    else
                    {
                        if (intersectionPosition.x > aPosition.x)
                        {
                            aTransform->MoveBy({ -intersectionWidth, 0.f });
                            body->SetLinearVelocity({ 0.f, body->GetLinearVelocity().y });
                        }
                        else
                        {
                            aTransform->MoveBy({ intersectionWidth, 0.f });
                            body->SetLinearVelocity({ 0.f, body->GetLinearVelocity().y });
                        }
                    }
                    auto collision = new Collision(colliders[i], colliders[j], intersection);
                    colliders[i]->OnCollision(*collision);
                    colliders[j]->OnCollision(*collision);
                }
            }
        }
        for (auto triggeredPair = triggersEnteredPair.cbegin(), nextTriggeredPair = triggeredPair; triggeredPair != triggersEnteredPair.cend(); triggeredPair = nextTriggeredPair)
        {
            ++nextTriggeredPair;
            if (!triggeredPair->first->GetBounds().intersects(triggeredPair->second->GetBounds()))
            {
                auto trigger = new Trigger(triggeredPair->first, triggeredPair->second);
                triggeredPair->first->OnTriggerExit(*trigger);
                triggeredPair->second->OnTriggerExit(*trigger);
                triggersEnteredPair.erase(triggeredPair);
            }
        }
    }
    void PhysicsSystem::Subscribe(ColliderComponent* collider)
    {
        colliders.push_back(collider);
    }
    void PhysicsSystem::Unsubscribe(ColliderComponent* collider)
    {
        colliders.erase(std::remove_if(colliders.begin(), colliders.end(), [collider](ColliderComponent* obj) { return obj == collider; }), colliders.end());
    }
}