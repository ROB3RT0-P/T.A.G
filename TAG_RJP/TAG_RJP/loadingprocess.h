/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#pragma once

#include "iprocess.h"
#include "global.h"

#include <SDL.h>
#include <string>
#include <vector>

class LoadingProcess : public IProcess
{
public:
	struct LoadRequest {
		global::Res resource;
		union {
			struct {
				size_t len;
				void* data;
			} rBinary;
			SDL_Texture* rTexture;
		};
	};

	class ILoadingProcessCallback
	{
	public:
		virtual void onLoadComplete(LoadRequest* loadedResources, size_t count) = 0;
	};

	size_t currentData;
	std::vector<LoadRequest> dataToLoad;
	ILoadingProcessCallback* callback = NULL;

	virtual void start(const Info& info);
	virtual void tick(const Info& info);
	virtual void render(const Info& info) {}
	virtual void endOfFrameUpdate(const Info& info) {}
	virtual void completed(const Info& info);
};