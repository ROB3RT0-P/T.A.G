/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#include "loadingprocess.h"
#include "global.h"
#include "processmanager.h"
#include "resourcemanager.h"

void LoadingProcess::start(const Info& info)
{
	currentData = 0; // RJP - Resets the current data index to start the loading process.
}

void LoadingProcess::tick(const Info& info)
{
	// RJP - Retrieves the resource type of the current data to be loaded.
	ResourceManager::ResourceType rtype = global::resourceManager()->getResourceType(raw_enum(dataToLoad[currentData].resource));

	// RJP - Switch statement to handle different resource types.
	switch (rtype) {
	case ResourceManager::ResourceType::Texture:
		// RJP - Loads the resource as a texture.
		global::resourceManager()->loadResource(raw_enum(dataToLoad[currentData].resource));
		// RJP - Retrieves the loaded texture and stores it in the loading data structure.
		dataToLoad[currentData].rTexture = global::resourceManager()->getResourceAsTexture(raw_enum(dataToLoad[currentData].resource));
		break;
	case ResourceManager::ResourceType::Binary:
		// RJP - Handling for binary resources (not implemented).
		break;
	default:
		// RJP - Default case, no action needed.
		break;
	}

	// RJP - Moves to the next data item to load.
	++currentData;

	// RJP - Checks if all data items have been loaded.
	if (currentData >= dataToLoad.size()) {
		// RJP - Marks the loading process as complete.
		global::processManager()->completeProcess(info.id);
	}
}

void LoadingProcess::completed(const Info& info)
{
	// RJP - Checks if a callback function is set and invokes it to signal completion of loading.
	if (callback) callback->onLoadComplete(dataToLoad.data(), dataToLoad.size());
}
