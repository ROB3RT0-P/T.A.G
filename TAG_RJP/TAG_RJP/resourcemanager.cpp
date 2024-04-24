/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#include "resourcemanager.h"
#include <SDL_image.h>

// RJP - Initializes the resource database with the provided entries.
void ResourceManager::initializeResourceDatabase(ResourceDBEntry* entries, uint32_t entryCount, uint32_t maxResources, std::string resourcePrefix)
{
    commonRoot = resourcePrefix; // RJP - Sets the common root path for all resources.

    Resource empty;
    empty.refCount = 0;
    empty.type = ResourceType::Unknown;
    resources.resize(maxResources, empty); // RJP - Resizes the resources vector and fills it with empty resources.

    // RJP - Populates the resources vector with file paths and types based on the provided entries.
    for (uint32_t i = 0; i < entryCount; ++i) {
        size_t id = entries[i].id;
        resources[id].filePath = commonRoot + entries[i].filePath;
        resources[id].type = entries[i].type;
    }
}

// RJP - Loads a resource with the given resource ID.
bool ResourceManager::loadResource(ResourceID resID)
{
    if (resID >= resources.size()) return false; // RJP - Checks if the resource ID is valid.
    if (resources[resID].type == ResourceType::Unknown) return false; // RJP - Checks if the resource type is known.

    if (resources[resID].refCount > 0) { // RJP - Checks if the resource is already loaded.
        ++resources[resID].refCount; // RJP - Increments the reference count.
        return true;
    }

    switch (resources[resID].type) {
    case ResourceType::Texture: {
        SDL_Texture* newTexture = nullptr;
        SDL_Surface* loadedSurface = IMG_Load(resources[resID].filePath.c_str()); // RJP - Loads the image surface.
        if (loadedSurface == nullptr) {
            return false; // RJP - Failed to load the image.
        }

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(sdlRenderer, loadedSurface); // RJP - Creates texture from surface.
        if (newTexture == nullptr) {
            return false; // RJP - Failed to create texture.
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface); // RJP - Frees the loaded surface.
        resources[resID].rTexture = newTexture; // RJP - Sets the texture.
        resources[resID].refCount = 1; // RJP - Sets the reference count.
    } break;
    case ResourceType::Binary: return false; // RJP - Binary resource loading to be implemented.
    default: break;
    }
    return true;
}

// RJP - Unloads the resource with the given resource ID.
bool ResourceManager::unloadResource(ResourceID resID)
{
    if (resID >= resources.size()) return false; // RJP - Checks if the resource ID is valid.
    if (resources[resID].type == ResourceType::Unknown) return false; // RJP - Checks if the resource type is known.

    if (resources[resID].refCount > 0) { // RJP - Checks if the resource is loaded.
        --resources[resID].refCount; // RJP - Decrements the reference count.
        if (resources[resID].refCount == 0) { // RJP - Checks if the reference count is zero after decrementing.
            switch (resources[resID].type) {
            case ResourceType::Texture: {
                // RJP - Frees the texture.
                if (resources[resID].rTexture != nullptr) {
                    SDL_DestroyTexture(resources[resID].rTexture);
                    resources[resID].rTexture = nullptr;
                }
            } break;
            case ResourceType::Binary: {
                // RJP - Unloading binary resources to be implemented.
            } break;
            default: break;
            }
        }
        return true;
    }
    return false;
}
