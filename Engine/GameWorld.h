#pragma once

#include <vector>
#include <iostream>
#include "GameObject.h"
#include "PhysicsSystem.h"

namespace Engine
{
	class GameWorld
	{
	public:
		static GameWorld* Instance();

		void Update(float deltaTime);
		void FixedUpdate(float deltaTime);
		void Render();
		void LateUpdate();

		GameObject* CreateGameObject();
		GameObject* CreateGameObject(std::string name);
		void DestroyGameObject(GameObject* gameObject);
		void Clear();

		void Print() const;

		// Добавь этот метод
		const std::vector<GameObject*>& GetGameObjects() const { return gameObjects; }

	private:
		GameWorld() {}
		~GameWorld() {}

		GameWorld(GameWorld const&) = delete;
		GameWorld& operator= (GameWorld const&) = delete;

		std::vector<GameObject*> gameObjects;
		std::vector<GameObject*> markedToDestroyGameObjects;

		float fixedCounter = 0.f;

		void DestroyGameObjectImmediate(GameObject* gameObject);
	};
}