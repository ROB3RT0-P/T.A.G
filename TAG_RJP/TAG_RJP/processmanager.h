/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#pragma once

#include "iprocess.h"

#include <stdint.h>
#include <vector>

class ProcessManager
{
public:
	ProcessManager();

	void registerProcess(IProcess* proc, uint32_t id, uint32_t tickOrder, uint32_t renderOrder);
	void yieldProcess(uint32_t procID);
	void resumeProcess(uint32_t procID);
	void completeProcess(uint32_t procID);
	void tickProcesses();
	void renderProcesses();
	void endOfFrameCleanup();

private:
	struct Process
	{
		IProcess* func;
		IProcess::Info info;
	};


	void registerProcessDeferred(Process* proc);
	void insertProcessIntoSortedBins(uint32_t procID);
	void removeProcessFromSortedBins(uint32_t procID);
	void removeProcess(uint32_t procIndex);


	struct OrderedProcess
	{
		uint32_t process;
		int32_t order;
	};

	std::vector<Process> pendingRegisterProcesses;
	std::vector<Process> registeredProcesses;
	std::vector<OrderedProcess> tickProcessesOrdered;
	std::vector<OrderedProcess> renderProcessesOrdered;
	std::vector<OrderedProcess> pendingDeletes;
};
