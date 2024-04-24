/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#pragma once

#include <stdint.h> // RJP - Including the standard integer types header file.

// RJP - Declaration of the interface class 'IProcess'.
class IProcess
{
public:
	// RJP - Enum defining states of a process.
	enum State
	{
		Invalid = 0, // RJP - Indicates an invalid state.
		NotStarted, // RJP - Indicates that the process has not started yet.
		Running, // RJP - Indicates that the process is currently running.
		Yielded, // RJP - Indicates that the process has yielded control.
		Complete, // RJP - Indicates that the process has completed successfully.
		Zombie, // RJP - Indicates that the process is in a zombie state.

		Max // RJP - Maximum value of the enum.
	};

	// RJP - Structure containing information about a process.
	struct Info
	{
		uint32_t id; // RJP - Unique identifier of the process.
		uint32_t procTypeID; // RJP - Identifier for the type of process.
		uint32_t tickOrder; // RJP - Order in which the process is ticked.
		uint32_t renderOrder; // RJP - Order in which the process is rendered.
		State currentState; // RJP - Current state of the process.
	};

	// RJP - Virtual destructor to ensure proper cleanup.
	virtual ~IProcess() = default;

	// RJP - Abstract functions representing different stages of the process lifecycle.
	virtual void start(const Info& info) = 0; // RJP - Function to start the process.
	virtual void tick(const Info& info) = 0; // RJP - Function to tick the process.
	virtual void render(const Info& info) = 0; // RJP - Function to render the process.
	virtual void endOfFrameUpdate(const Info& info) = 0; // RJP - Function to update the process at the end of a frame.
	virtual void completed(const Info& info) = 0; // RJP - Function called when the process is completed.
};
