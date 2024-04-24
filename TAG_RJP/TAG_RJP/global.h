/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#pragma once

#include <stdint.h>
#include <type_traits>

// RJP - Forward declarations of classes.
class Game;
class ProcessManager;
class EntityManager;
class ResourceManager;

// RJP - Namespace 'global' containing various declarations and enums.
namespace global
{
	// RJP - Function declarations for retrieving global instances.
	Game* game();
	ProcessManager* processManager();
	EntityManager* entityManager();
	ResourceManager* resourceManager();

	// RJP - Enum defining task IDs.
	enum class TaskID : uint32_t {
		DontCare = 0, // RJP - TaskID indicating no specific care about task order.
		Loading, // RJP - TaskID indicating loading task.
	};

	// RJP - Enum defining tick order.
	enum class TickOrder : uint32_t {
		DontCare = 0, // RJP - TickOrder indicating no specific care about tick order.
		First = 1, // RJP - TickOrder for tasks to be executed first.
		Second = 2, // RJP - TickOrder for tasks to be executed second.
	};

	// RJP - Enum defining render order.
	enum class RenderOrder : uint32_t {
		DontCare = 0, // RJP - RenderOrder indicating no specific care about render order.
		First = 1, // RJP - RenderOrder for objects to be rendered first.
		Second = 2, // RJP - RenderOrder for objects to be rendered second.
	};

	// RJP - Enum defining resource IDs.
	enum class Res : uint32_t {
		Default, // RJP - Default resource ID.
		PlayerSprite, // RJP - Resource ID for player sprite.
		EnemySprite, // RJP - Resource ID for enemy sprite.
		MainMenuSprite, // RJP - Resource ID for main menu sprite.
		BackgroundSprite, // RJP - Resource ID for background sprite.
		GameOverSprite, // RJP - Resource ID for game over sprite.
		DeadEndSprite, // RJP - Resource ID for dead end sprite.
		ContinueSprite, // RJP - Resource ID for continue sprite.
		EscapeSprite, // RJP - Resource ID for escape sprite.

		Max = 256 // RJP - Maximum number of resource IDs.
	};

	// RJP - Enum defining entity types.
	enum class EntityType : uint32_t {
		Unknown = 0, // RJP - Unknown entity type.
		Player, // RJP - Entity type for player.
		Enemy, // RJP - Entity type for enemy.
	};

}

// RJP - Template function for getting the raw value of an enum.
template <typename E> // RJP - Template declaration for a function taking an enum type.
constexpr typename std::underlying_type<E>::type raw_enum(E e) noexcept {
	// RJP - Casts the enum value to its underlying integral type and returns it.
	return static_cast<typename std::underlying_type<E>::type>(e);
}