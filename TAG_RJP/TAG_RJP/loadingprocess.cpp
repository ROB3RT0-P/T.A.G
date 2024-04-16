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
	currentData = 0;
}

void LoadingProcess::tick(const Info& info)
{
	ResourceManager::ResourceType rtype = global::resourceManager()->getResourceType(raw_enum(dataToLoad[currentData].resource));
	switch (rtype) {
	case ResourceManager::ResourceType::Texture:
		global::resourceManager()->loadResource(raw_enum(dataToLoad[currentData].resource));
		dataToLoad[currentData].rTexture = global::resourceManager()->getResourceAsTexture(raw_enum(dataToLoad[currentData].resource));
		break;
	case ResourceManager::ResourceType::Binary:
		break;
	default:
		break;
	}
	++currentData;
	if (currentData >= dataToLoad.size()) {
		global::processManager()->completeProcess(info.id);
	}
}

void LoadingProcess::completed(const Info& info)
{
	if (callback) callback->onLoadComplete(dataToLoad.data(), dataToLoad.size());
}
