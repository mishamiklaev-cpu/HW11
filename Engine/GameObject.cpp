#include "pch.h"
#include "GameObject.h"
#include "Logger.h"
namespace Engine
{
	GameObject::GameObject()
	{
		name = "GameObject";
		AddComponent<TransformComponent>();
	}
	GameObject::GameObject(std::string newName)
	{
		name = newName;
		AddComponent<TransformComponent>();
	}
	GameObject::~GameObject()
	{
		for (auto component : components)
		{
			delete component;
		}
		components.clear();
		children.clear();
	}
	std::string GameObject::GetName() const
	{
		return name;
	}
	void GameObject::Print(int depth) const
	{
		LOG_INFO(std::string(depth * 2, ' ') + GetName());
		for (auto& component : components)
		{
			LOG_INFO(std::string(depth * 2, ' ') + "::" + std::to_string(reinterpret_cast<uintptr_t>(component)));
		}
		for (GameObject* child : children)
		{
			child->Print(depth + 1);
		}
	}
	void GameObject::Update(float deltaTime)
	{
		for (auto& component : components)
		{
			component->Update(deltaTime);
		}
	}
	void GameObject::Render()
	{
		for (auto& component : components)
		{
			component->Render();
		}
	}
	void GameObject::AddChild(GameObject* child)
	{
		children.push_back(child);
	}
	void GameObject::RemoveChild(GameObject* child)
	{
		children.erase(std::remove_if(children.begin(), children.end(), [child](GameObject* obj) { return obj == child; }), children.end());
	}
}