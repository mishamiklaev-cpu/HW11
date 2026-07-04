#pragma once

#include "GameWorld.h"
#include "GameObject.h"
#include "Vector.h"
#include "FollowComponent.h"
#include "RigidbodyComponent.h"
#include "SpriteColliderComponent.h"
#include <cassert>
#include <SFML/Graphics.hpp>

namespace Roguelike
{
	class AI
	{
	public:
		AI(const Engine::Vector2Df& position, Engine::GameObject* player);
		Engine::GameObject* GetGameObject();

		void TakeDamage(float damage);
		void Heal(float amount);
		float GetHealthPoints() const { return healthPoints; }
		float GetMaxHealthPoints() const { return maxHealthPoints; }
		float GetArmor() const { return armor; }
		float GetHealthPercent() const { return healthPoints / maxHealthPoints; }
		bool IsAlive() const { return healthPoints > 0; }

		void UpdateUI(sf::RenderWindow& window);
		void Update(float deltaTime);

	private:
		Engine::GameObject* gameObject;
		float healthPoints = 50.0f;
		float maxHealthPoints = 50.0f;
		float armor = 5.0f;
		float attackDamage = 15.0f;

		sf::Font font;
		sf::Text hpText;
		bool uiInitialized = false;

		float flashTimer = 0.0f;
		bool isFlashing = false;
	};
}