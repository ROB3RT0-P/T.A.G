/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#include "resourcemanager.h"
#include <SDL_image.h>

void ResourceManager::initializeResourceDatabase(ResourceDBEntry* entries, uint32_t entryCount, uint32_t maxResources, std::string resourcePrefix)
{
	commonRoot = resourcePrefix;

	Resource empty;
	empty.refCount = 0;
	empty.type = ResourceType::Unknown;
	resources.resize(maxResources, empty);

	for (uint32_t i = 0; i < entryCount; ++i) {
		size_t id = entries[i].id;
		resources[id].filePath = commonRoot + entries[i].filePath;
		resources[id].type = entries[i].type;
	}
}

bool ResourceManager::loadResource(ResourceID resID)
{
	if (resID >= resources.size()) return false;
	if (resources[resID].type == ResourceType::Unknown) return false;
	if (resources[resID].refCount > 0) {
		++resources[resID].refCount;
		return true;
	}
	switch (resources[resID].type) {
	case ResourceType::Texture: {
		SDL_Texture* newTexture = NULL;
		SDL_Surface* loadedSurface = IMG_Load(resources[resID].filePath.c_str());
		if (loadedSurface == NULL)
		{
			return false;
		}
		
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(sdlRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			return false;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
		resources[resID].rTexture = newTexture;
		resources[resID].refCount = 1;
	} break;
	case ResourceType::Binary: return false; // TODO
	default: break;
	}
	return true;
}

bool ResourceManager::unloadResource(ResourceID resID)
{
	if (resID >= resources.size()) return false;
	if (resources[resID].type == ResourceType::Unknown) return false;

	if (resources[resID].refCount > 0) {
		--resources[resID].refCount;
		if (resources[resID].refCount == 0) {
			switch (resources[resID].type) {
			case ResourceType::Texture: {
				// RJP - Free the texture.
				if (resources[resID].rTexture != nullptr) {
					SDL_DestroyTexture(resources[resID].rTexture);
					resources[resID].rTexture = nullptr;
				}
			} break;
			case ResourceType::Binary: {
				// RJP - Free binary resources to go here.
			} break;
			default: break;
			}
		}
		return true;
	}
	return false;
}
