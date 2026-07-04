#include "Wall.h"

namespace Roguelike
{
	Wall::Wall(const Engine::Vector2Df& position, int textureMapIndex)
	{
		gameObject = Engine::GameWorld::Instance()->CreateGameObject("Wall");
		auto transform = gameObject->GetComponent<Engine::TransformComponent>();
		transform->SetWorldPosition(position);

		auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
		renderer->SetTexture(*Engine::ResourceSystem::Instance()->GetTextureMapElementShared("level_walls", textureMapIndex));
		renderer->SetPixelSize(128, 128);

		auto rigidbody = gameObject->AddComponent<Engine::RigidbodyComponent>();
		rigidbody->SetKinematic(true);

		auto collider = gameObject->AddComponent<Engine::SpriteColliderComponent>();
	}
}