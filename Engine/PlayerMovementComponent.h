#pragma once

#include "Component.h"
#include "TransformComponent.h"
#include "RigidbodyComponent.h"
#include "InputComponent.h"

namespace Engine
{
	class PlayerMovementComponent : public Component
	{
	public:
		PlayerMovementComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		void SetSpeed(float newSpeed);

	private:
		TransformComponent* transform = nullptr;
		RigidbodyComponent* rigidbody = nullptr;
		InputComponent* input = nullptr;
		float speed = 300.0f;
	};
} 