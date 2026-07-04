#include "Floor.h"

namespace Roguelike
{
	Floor::Floor(const Engine::Vector2Df& position, int textureMapIndex)
	{
		gameObject = Engine::GameWorld::Instance()->CreateGameObject("Floor");
		auto transform = gameObject->GetComponent<Engine::TransformComponent>();
		transform->SetWorldPosition(position);

		auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
		renderer->SetTexture(*Engine::ResourceSystem::Instance()->GetTextureMapElementShared("level_floors", textureMapIndex));
		renderer->SetPixelSize(128, 128);
	}
}