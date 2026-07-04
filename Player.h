#pragma once

#include "CameraComponent.h"
#include "GameWorld.h"
#include "SpriteRendererComponent.h"
#include "RenderSystem.h"
#include "InputComponent.h"
#include "GameObject.h"
#include <cassert>
#include <SFML/Graphics.hpp>

namespace Roguelike
{
	class Player
	{
	public:
		Player();
		Engine::GameObject* GetGameObject();

		void TakeDamage(float damage);
		void Heal(float amount);
		float GetHealthPoints() const { return healthPoints; }
		float GetMaxHealthPoints() const { return maxHealthPoints; }
		float GetArmor() const { return armor; }
		float GetHealthPercent() const { return healthPoints / maxHealthPoints; }
		bool IsAlive() const { return healthPoints > 0; }

		void UpdateUI(sf::RenderWindow& window);

	private:
		Engine::GameObject* gameObject;
		float healthPoints = 100.0f;
		float maxHealthPoints = 100.0f;
		float armor = 10.0f;
		float maxArmor = 50.0f;

		sf::Font font;
		sf::Text hpText;
		bool uiInitialized = false;
	};
}