#pragma once

#include "GameWorld.h"
#include "ResourceSystem.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SpriteRendererComponent.h"

namespace Roguelike
{
	class Floor
	{
	public:
		Floor(const Engine::Vector2Df& position, int textureMapIndex);
		Engine::GameObject* GetGameObject() const { return gameObject; }
	private:
		Engine::GameObject* gameObject;
	};
}