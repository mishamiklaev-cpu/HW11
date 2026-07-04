#pragma once

#include "GameWorld.h"
#include "ResourceSystem.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"
#include "RigidbodyComponent.h"
#include "SpriteColliderComponent.h"

namespace Roguelike
{
	class Wall
	{
	public:
		Wall(const Engine::Vector2Df& position, int textureMapIndex);
		Engine::GameObject* GetGameObject() const { return gameObject; }
	private:
		Engine::GameObject* gameObject;
	};
}