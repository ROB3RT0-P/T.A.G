/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#include "processmanager.h"

// RJP - Constructor for the ProcessManager class.
ProcessManager::ProcessManager() {
	// RJP - Reserves memory for vectors to avoid frequent reallocations.
	pendingRegisterProcesses.reserve(64);
	registeredProcesses.reserve(256);
	tickProcessesOrdered.reserve(256);
	renderProcessesOrdered.reserve(256);
	pendingDeletes.reserve(64);
}

// RJP - Registers a process with the specified ID, tick order, and render order.
void ProcessManager::registerProcess(IProcess* proc, uint32_t id, uint32_t tickOrder, uint32_t renderOrder) {
	// RJP - Creates a new process instance and initializes its information.
	Process newProcess;
	newProcess.func = proc;
	newProcess.info.procTypeID = id;
	newProcess.info.tickOrder = tickOrder;
	newProcess.info.renderOrder = renderOrder;
	newProcess.info.currentState = IProcess::NotStarted;
	// RJP - Adds the new process to the list of pending register processes.
	pendingRegisterProcesses.push_back(newProcess);
}

// RJP - Yields a process, removing it from sorted bins and updating its state to 'Yielded'.
void ProcessManager::yieldProcess(uint32_t procID) {
	removeProcessFromSortedBins(procID);
	registeredProcesses[procID].info.currentState = IProcess::Yielded;
}

// RJP - Resumes a yielded process by re-inserting it into sorted bins.
void ProcessManager::resumeProcess(uint32_t procID) {
	// RJP - Checks if the process is in 'Yielded' state before resuming.
	if (registeredProcesses[procID].info.currentState != IProcess::Yielded) return;
	insertProcessIntoSortedBins(procID);
}

// RJP - Marks a process as 'Complete' and removes it from sorted bins.
void ProcessManager::completeProcess(uint32_t procID) {
	removeProcessFromSortedBins(procID);
	registeredProcesses[procID].info.currentState = IProcess::Complete;
}

// RJP - Registers a process deferredly, ensuring proper initialization and insertion into sorted bins.
void ProcessManager::registerProcessDeferred(Process* proc) {
	uint32_t processCount = (uint32_t)registeredProcesses.size();
	uint32_t insertIndex = processCount;
	for (uint32_t i = 0; i < processCount; ++i) {
		// RJP - Finds an empty slot or the end of the registered processes list to insert the new process.
		if (registeredProcesses[i].func == nullptr) {
			registeredProcesses[i] = *proc;
			insertIndex = i;
			break;
		}
	}
	// RJP - If no empty slot is found, adds the new process to the end of the registered processes list.
	if (insertIndex >= processCount) {
		registeredProcesses.push_back(*proc);
		insertIndex = processCount;
	}
	// RJP - Sets the ID of the process and inserts it into sorted bins.
	registeredProcesses[insertIndex].info.id = insertIndex;
	insertProcessIntoSortedBins(insertIndex);
	// RJP - If the process is not started, initializes it and sets its state to 'Running'.
	if (registeredProcesses[insertIndex].info.currentState == IProcess::NotStarted) {
		registeredProcesses[insertIndex].func->start(registeredProcesses[insertIndex].info);
		registeredProcesses[insertIndex].info.currentState = IProcess::Running;
	}
}

// RJP - Executes tick functions for all registered processes in the tick order.
void ProcessManager::tickProcesses() {
	for (size_t i = 0, n = tickProcessesOrdered.size(); i < n; ++i) {
		Process* process = &registeredProcesses[tickProcessesOrdered[i].process];
		process->func->tick(process->info);
	}
}

// RJP - Executes render functions for all registered processes in the render order.
void ProcessManager::renderProcesses() {
	for (size_t i = 0, n = renderProcessesOrdered.size(); i < n; ++i) {
		Process* process = &registeredProcesses[renderProcessesOrdered[i].process];
		process->func->render(process->info);
	}
}

// RJP - Performs end-of-frame cleanup tasks, including executing end-of-frame updates, processing pending register requests, and handling completed processes.
void ProcessManager::endOfFrameCleanup() {
	// RJP - Executes end-of-frame updates for tick processes and processes pending register requests.
	for (size_t i = 0, n = tickProcessesOrdered.size(); i < n; ++i) {
		Process* process = &registeredProcesses[tickProcessesOrdered[i].process];
		process->func->endOfFrameUpdate(process->info);
	}

	// RJP - Processes pending register requests by registering the processes and clearing the pending list.
	for (size_t i = 0, n = pendingRegisterProcesses.size(); i < n; ++i) {
		registerProcessDeferred(&pendingRegisterProcesses[i]);
	}
	pendingRegisterProcesses.clear();

	// RJP - Marks completed processes for deletion and executes their completion functions.
	for (size_t i = 0, n = registeredProcesses.size(); i < n; ++i) {
		if (registeredProcesses[i].info.currentState == IProcess::Complete) {
			OrderedProcess toDelete;
			toDelete.process = (uint32_t)i;
			toDelete.order = 0;
			pendingDeletes.push_back(toDelete);
		}
	}

	// RJP - Removes completed processes and executes their completion functions.
	for (size_t i = 0, n = pendingDeletes.size(); i < n; ++i) {
		Process process = registeredProcesses[pendingDeletes[i].process];
		removeProcess(pendingDeletes[i].process);
		process.func->completed(process.info);
	}
	pendingDeletes.clear();
}

// RJP - Inserts a process into sorted bins based on its tick and render orders.
void ProcessManager::insertProcessIntoSortedBins(uint32_t procID) {
	IProcess::Info info = registeredProcesses[procID].info;
	OrderedProcess orderedProc;
	orderedProc.process = procID;
	orderedProc.order = info.tickOrder;
	size_t i = 0;
	// RJP - Inserts the process into the tick processes list in order.
	for (size_t n = tickProcessesOrdered.size(); i < n; ++i) {
		if (tickProcessesOrdered[i].order > orderedProc.order) {
			tickProcessesOrdered.insert(tickProcessesOrdered.begin() + i, orderedProc);
			break;
		}
	}
	// RJP - If the process has the highest tick order or was not inserted earlier, adds it to the end of the tick processes list.
	if (i == tickProcessesOrdered.size()) tickProcessesOrdered.push_back(orderedProc);
	orderedProc.order = info.renderOrder;
	i = 0;
	// RJP - Updates the order for rendering processes and inserts the process into the render processes list.
	for (size_t n = renderProcessesOrdered.size(); i < n; ++i) {
		if (renderProcessesOrdered[i].order > orderedProc.order) {
			renderProcessesOrdered.insert(renderProcessesOrdered.begin() + i, orderedProc);
			break;
		}
	}
	// RJP - If the process has the highest render order or was not inserted earlier, adds it to the end of the render processes list.
	if (i == renderProcessesOrdered.size()) renderProcessesOrdered.push_back(orderedProc);
}

// RJP - Removes a process from sorted bins, both from the tick and render processes lists.
void ProcessManager::removeProcessFromSortedBins(uint32_t procID) {
	// RJP - Removes the process from the tick processes list.
	for (size_t i = 0, n = tickProcessesOrdered.size(); i < n; ++i) {
		if (tickProcessesOrdered[i].process == procID) {
			tickProcessesOrdered.erase(tickProcessesOrdered.begin() + i);
			break;
		}
	}
	// RJP - Removes the process from the render processes list.
	for (size_t i = 0, n = renderProcessesOrdered.size(); i < n; ++i) {
		if (renderProcessesOrdered[i].process == procID) {
			renderProcessesOrdered.erase(renderProcessesOrdered.begin() + i);
			break;
		}
	}
}

// RJP - Removes a process from the registered processes list by its index.
void ProcessManager::removeProcess(uint32_t procIndex) {
	// RJP - Removes the process from sorted bins before erasing it from the registered processes list.
	removeProcessFromSortedBins(procIndex);
	registeredProcesses.erase(registeredProcesses.begin() + procIndex);
}
