#include "pch.h"
#include "PlayerMovementComponent.h"

namespace Engine
{
	PlayerMovementComponent::PlayerMovementComponent(GameObject* gameObject)
		: Component(gameObject)
	{
		transform = gameObject->GetComponent<TransformComponent>();
		rigidbody = gameObject->GetComponent<RigidbodyComponent>();
		input = gameObject->GetComponent<InputComponent>();
	}

	void PlayerMovementComponent::Update(float deltaTime)
	{
		if (!transform || !rigidbody || !input) return;

		float horizontal = input->GetHorizontalAxis();
		float vertical = input->GetVerticalAxis();

		if (horizontal != 0 && vertical != 0)
		{
			horizontal *= 0.707f;
			vertical *= 0.707f;
		}

		rigidbody->SetLinearVelocity({ horizontal * speed, vertical * speed });
	}

	void PlayerMovementComponent::Render() {}

	void PlayerMovementComponent::SetSpeed(float newSpeed)
	{
		speed = newSpeed;
	}
}
