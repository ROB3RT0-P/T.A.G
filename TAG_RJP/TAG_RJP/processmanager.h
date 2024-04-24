/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#pragma once

#include "iprocess.h"

#include <stdint.h>
#include <vector>

// RJP - Class responsible for managing processes within the game.
class ProcessManager
{
public:
	// RJP - Constructs a ProcessManager object.
	ProcessManager();

	// RJP - Registers a process with specified ID, tick order, and render order.
	void registerProcess(IProcess* proc, uint32_t id, uint32_t tickOrder, uint32_t renderOrder);

	// RJP - Yields the process with the given ID.
	void yieldProcess(uint32_t procID);

	// RJP - Resumes the process with the given ID.
	void resumeProcess(uint32_t procID);

	// RJP - Completes the process with the given ID.
	void completeProcess(uint32_t procID);

	// RJP - Ticks all registered processes.
	void tickProcesses();

	// RJP - Renders all registered processes.
	void renderProcesses();

	// RJP - Performs end-of-frame cleanup, including processing pending register processes and removing completed processes.
	void endOfFrameCleanup();

private:
	// RJP - Struct representing a process, including its function and information.
	struct Process
	{
		IProcess* func;         // RJP - Pointer to the process function.
		IProcess::Info info;    // RJP - Information about the process.
	};

	// RJP - Registers a process deferredly.
	void registerProcessDeferred(Process* proc);

	// RJP - Inserts a process into sorted bins based on its tick and render orders.
	void insertProcessIntoSortedBins(uint32_t procID);

	// RJP - Removes a process from sorted bins.
	void removeProcessFromSortedBins(uint32_t procID);

	// RJP - Removes a process from the registered processes list by its index.
	void removeProcess(uint32_t procIndex);

	// RJP - Struct representing an ordered process.
	struct OrderedProcess
	{
		uint32_t process;   // RJP - ID of the process.
		int32_t order;      // RJP - Order of the process.
	};

	std::vector<Process> pendingRegisterProcesses;  // RJP - List of processes pending registration.
	std::vector<Process> registeredProcesses;      // RJP - List of registered processes.
	std::vector<OrderedProcess> tickProcessesOrdered;    // RJP - Ordered list of processes for ticking.
	std::vector<OrderedProcess> renderProcessesOrdered;  // RJP - Ordered list of processes for rendering.
	std::vector<OrderedProcess> pendingDeletes;   // RJP - List of processes pending deletion.
};
