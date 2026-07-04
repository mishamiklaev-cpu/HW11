#pragma once

#include "Component.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "SpriteRendererComponent.h"

namespace Engine
{
	class FollowComponent : public Component
	{
	public:
		FollowComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetTarget(GameObject* targetObject);
		void SetSpeed(float newSpeed);
		float GetSpeed() const;
		void SetDetectionRadius(float radius);

	private:
		TransformComponent* transform = nullptr;
		RigidbodyComponent* rigidbody = nullptr;
		TransformComponent* targetTransform = nullptr;
		SpriteRendererComponent* spriteRenderer = nullptr;

		float speed = 100.0f;
		float detectionRadius = 400.0f;
	};
}