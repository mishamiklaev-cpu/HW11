#include "pch.h"
#include "Component.h"
#include "Logger.h"
namespace Engine
{
    Component::Component(GameObject* gameObject) : gameObject(gameObject) {}
    Component::~Component()
    {
        LOG_INFO("Deleted component: " + std::to_string(reinterpret_cast<uintptr_t>(this)));
    }
    GameObject* Component::GetGameObject()
    {
        return gameObject;
    }
}