#pragma once

#include "AudioComponent.h"
#include "ResourceSystem.h"
#include "GameWorld.h"
#include <iostream>

namespace Roguelike
{
	class Music
	{
	public:
		Music(const std::string& resourceName);
		~Music();

		void Play();
		void Stop();

	private:
		Engine::GameObject* gameObject;
		Engine::AudioComponent* audioComponent;
	};
}