/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#pragma once

#include <SDL.h>
#include <string>
#include <vector>
#include <stdint.h>

struct ResourceBinBlob
{
	size_t len;
	void* data;
};

class ResourceManager
{
public:
	enum class ResourceType : uint32_t {
		Unknown,
		Binary,
		Texture,

		Max,
	};

	typedef size_t ResourceID;

	struct ResourceDBEntry
	{
		ResourceID id;
		std::string filePath;
		ResourceType type;
	};

	ResourceManager() = default;

	void initializeResourceDatabase(ResourceDBEntry* entries, uint32_t entryCount, uint32_t maxResources, std::string resourcePrefix);
	void initializeLoaderData(SDL_Renderer* inRenderer)
	{
		sdlRenderer = inRenderer;
	}

	bool loadResource(ResourceID resID);
	bool unloadResource(ResourceID resID);

	bool isResourceResident(ResourceID resID) {
		if (resID >= resources.size()) return false;
		return resources[resID].refCount > 0;
	}
	ResourceType getResourceType(ResourceID resID) {
		if (resID >= resources.size()) return ResourceType::Unknown;
		return resources[resID].type;
	}
	ResourceBinBlob getResourceAsBlob(ResourceID resID) {
		ResourceBinBlob invalid = { 0 };
		if (resID >= resources.size()) return invalid;
		if (resources[resID].type != ResourceType::Binary) return invalid;
		return resources[resID].rBinary;
	}
	SDL_Texture* getResourceAsTexture(ResourceID resID) {
		SDL_Texture* invalid = NULL;
		if (resID >= resources.size()) return invalid;
		if (resources[resID].type != ResourceType::Texture) return invalid;
		return resources[resID].rTexture;
	}

private:

	struct Resource {
		std::string filePath;
		uint32_t refCount;
		ResourceType type;
		union {
			ResourceBinBlob rBinary;
			SDL_Texture* rTexture;
		};
	};

	typedef std::vector<Resource> ResourceVector;

	SDL_Renderer* sdlRenderer = NULL;

	std::string commonRoot;
	ResourceVector resources;
};
