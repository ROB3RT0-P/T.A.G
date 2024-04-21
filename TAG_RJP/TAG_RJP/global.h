/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#pragma once

#include <stdint.h>
#include <type_traits>

// Forwards.
class Game;
class ProcessManager;
class EntityManager;
class ResourceManager;

namespace global
{
	Game* game();
	ProcessManager* processManager();
	EntityManager* entityManager();
	ResourceManager* resourceManager();

	enum class TaskID : uint32_t {
		DontCare = 0,
		Loading,
	};

	enum class TickOrder : uint32_t {
		DontCare = 0,
		First = 1,
		Second = 2,
	};

	enum class RenderOrder : uint32_t {
		DontCare = 0,
		First = 1,
		Second = 2,
	};

	enum class Res : uint32_t {
		Default,
		PlayerSprite,
		EnemySprite,
		MainMenuSprite,
		BackgroundSprite,
		GameOverSprite,
		DeadEndSprite,
		ContinueSprite,
		EscapeSprite,

		Max = 256
	};

	enum class EntityType : uint32_t {
		Unknown = 0,
		Player,
		Enemy,
	};

}

template <typename E>
constexpr typename std::underlying_type<E>::type raw_enum(E e) noexcept {
	return static_cast<typename std::underlying_type<E>::type>(e);
}