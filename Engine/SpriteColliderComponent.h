#pragma once

#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include <SFML/Graphics.hpp>

namespace Engine
{
	class SpriteColliderComponent : public ColliderComponent
	{
	public:
		SpriteColliderComponent(GameObject* gameObject);
		~SpriteColliderComponent();

		void Update(float deltaTime) override;
		void Render() override;

		sf::FloatRect& GetBounds() { return bounds; }
		const sf::FloatRect& GetBounds() const { return bounds; }

	private:
		sf::Sprite* sprite = nullptr;
		TransformComponent* transform = nullptr;
		SpriteRendererComponent* spriteRendererComponent = nullptr;
	};
}