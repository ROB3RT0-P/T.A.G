/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#pragma once

#include <SDL.h>
#include <string>
#include <vector>
#include <stdint.h>

// RJP - Structure to hold binary resource data.
struct ResourceBinBlob
{
    size_t len; // RJP - Length of the binary data.
    void* data; // RJP - Pointer to the binary data.
};

// RJP - Manages resources including loading, unloading, and querying their types.
class ResourceManager
{
public:
    // RJP - Enum defining types of resources.
    enum class ResourceType : uint32_t {
        Unknown,  // RJP - Unknown resource type.
        Binary,   // RJP - Binary resource type.
        Texture,  // RJP - Texture resource type.

        Max,      // RJP - Maximum number of resource types.
    };

    // RJP - Identifier for a resource.
    typedef size_t ResourceID;

    // RJP - Structure defining a resource database entry.
    struct ResourceDBEntry
    {
        ResourceID id;          // RJP - Unique ID of the resource.
        std::string filePath;   // RJP - File path of the resource.
        ResourceType type;      // RJP - Type of the resource.
    };

    // RJP - Default constructor.
    ResourceManager() = default;

    // RJP - Initializes the resource database with provided entries.
    void initializeResourceDatabase(ResourceDBEntry* entries, uint32_t entryCount, uint32_t maxResources, std::string resourcePrefix);

    // RJP - Initializes loader data with the SDL renderer.
    void initializeLoaderData(SDL_Renderer* inRenderer)
    {
        sdlRenderer = inRenderer;
    }

    // RJP - Loads a resource identified by the resource ID.
    bool loadResource(ResourceID resID);

    // RJP - Unloads a resource identified by the resource ID.
    bool unloadResource(ResourceID resID);

    // RJP - Checks if a resource identified by the resource ID is resident.
    bool isResourceResident(ResourceID resID) {
        if (resID >= resources.size()) return false;
        return resources[resID].refCount > 0;
    }

    // RJP - Gets the type of a resource identified by the resource ID.
    ResourceType getResourceType(ResourceID resID) {
        if (resID >= resources.size()) return ResourceType::Unknown;
        return resources[resID].type;
    }

    // RJP - Gets the binary data of a resource identified by the resource ID.
    ResourceBinBlob getResourceAsBlob(ResourceID resID) {
        ResourceBinBlob invalid = { 0 };
        if (resID >= resources.size()) return invalid;
        if (resources[resID].type != ResourceType::Binary) return invalid;
        return resources[resID].rBinary;
    }

    // RJP - Gets the texture of a resource identified by the resource ID.
    SDL_Texture* getResourceAsTexture(ResourceID resID) {
        SDL_Texture* invalid = NULL;
        if (resID >= resources.size()) return invalid;
        if (resources[resID].type != ResourceType::Texture) return invalid;
        return resources[resID].rTexture;
    }

private:
    // RJP - Structure defining a resource.
    struct Resource {
        std::string filePath;       // RJP - File path of the resource.
        uint32_t refCount;          // RJP - Reference count of the resource.
        ResourceType type;          // RJP - Type of the resource.
        union {
            ResourceBinBlob rBinary; // RJP - Binary resource data.
            SDL_Texture* rTexture;   // RJP - Texture resource data.
        };
    };

    // RJP - Typedef for the vector of resources.
    typedef std::vector<Resource> ResourceVector;

    SDL_Renderer* sdlRenderer = NULL; // RJP - Pointer to the SDL renderer.

    std::string commonRoot;     // RJP - Common root path for all resources.
    ResourceVector resources;   // RJP - Vector to hold the resources.
};
