/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#pragma once

#include <stdint.h>

class IProcess
{
public:
	enum State
	{
		Invalid = 0,
		NotStarted,
		Running,
		Yielded,
		Complete,
		Zombie,

		Max
	};

	struct Info
	{
		uint32_t id;
		uint32_t procTypeID;
		uint32_t tickOrder;
		uint32_t renderOrder;
		State currentState;
	};

	virtual ~IProcess() = default;

	virtual void start(const Info& info) = 0;
	virtual void tick(const Info& info) = 0;
	virtual void render(const Info& info) = 0;
	virtual void endOfFrameUpdate(const Info& info) = 0;
	virtual void completed(const Info& info) = 0;
};
