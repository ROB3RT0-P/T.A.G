/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#include "processmanager.h"

ProcessManager::ProcessManager() {
	pendingRegisterProcesses.reserve(64);
	registeredProcesses.reserve(256);
	tickProcessesOrdered.reserve(256);
	renderProcessesOrdered.reserve(256);
	pendingDeletes.reserve(64);
}

void ProcessManager::registerProcess(IProcess* proc, uint32_t id, uint32_t tickOrder, uint32_t renderOrder) {
	Process newProcess;
	newProcess.func = proc;
	newProcess.info.procTypeID = id;
	newProcess.info.tickOrder = tickOrder;
	newProcess.info.renderOrder = renderOrder;
	newProcess.info.currentState = IProcess::NotStarted;
	pendingRegisterProcesses.push_back(newProcess);
}

void ProcessManager::yieldProcess(uint32_t procID) {
	removeProcessFromSortedBins(procID);
	registeredProcesses[procID].info.currentState = IProcess::Yielded;
}

void ProcessManager::resumeProcess(uint32_t procID) {
	if (registeredProcesses[procID].info.currentState != IProcess::Yielded) return;
	insertProcessIntoSortedBins(procID);
}

void ProcessManager::completeProcess(uint32_t procID) {
	removeProcessFromSortedBins(procID);
	registeredProcesses[procID].info.currentState = IProcess::Complete;
}

void ProcessManager::registerProcessDeferred(Process* proc) {
	uint32_t processCount = (uint32_t)registeredProcesses.size();
	uint32_t insertIndex = processCount;
	for (uint32_t i = 0; i < processCount; ++i) {
		if (registeredProcesses[i].func == NULL) {
			registeredProcesses[i] = *proc;
			insertIndex = i;
			break;
		}
	}
	if (insertIndex >= processCount) {
		registeredProcesses.push_back(*proc);
		insertIndex = processCount;
	}
	registeredProcesses[insertIndex].info.id = insertIndex;
	insertProcessIntoSortedBins(insertIndex);
	if (registeredProcesses[insertIndex].info.currentState == IProcess::NotStarted) {
		registeredProcesses[insertIndex].func->start(registeredProcesses[insertIndex].info);
		registeredProcesses[insertIndex].info.currentState = IProcess::Running;
	}
}

void ProcessManager::tickProcesses() {
	for (size_t i = 0, n = tickProcessesOrdered.size(); i < n; ++i) {
		Process* process = &registeredProcesses[tickProcessesOrdered[i].process];
		process->func->tick(process->info);
	}
}

void ProcessManager::renderProcesses() {
	for (size_t i = 0, n = renderProcessesOrdered.size(); i < n; ++i) {
		Process* process = &registeredProcesses[renderProcessesOrdered[i].process];
		process->func->render(process->info);
	}
}

void ProcessManager::endOfFrameCleanup() {
	for (size_t i = 0, n = tickProcessesOrdered.size(); i < n; ++i) {
		Process* process = &registeredProcesses[tickProcessesOrdered[i].process];
		process->func->endOfFrameUpdate(process->info);
	}

	for (size_t i = 0, n = pendingRegisterProcesses.size(); i < n; ++i) {
		registerProcessDeferred(&pendingRegisterProcesses[i]);
	}
	pendingRegisterProcesses.clear();

	for (size_t i = 0, n = registeredProcesses.size(); i < n; ++i) {
		if (registeredProcesses[i].info.currentState == IProcess::Complete) {
			OrderedProcess toDelete;
			toDelete.process = (uint32_t)i;
			toDelete.order = 0;
			pendingDeletes.push_back(toDelete);
		}
	}

	for (size_t i = 0, n = pendingDeletes.size(); i < n; ++i) {
		Process process = registeredProcesses[pendingDeletes[i].process];
		removeProcess(pendingDeletes[i].process);
		process.func->completed(process.info);
	}
	pendingDeletes.clear();
}

void ProcessManager::insertProcessIntoSortedBins(uint32_t procID) {
	IProcess::Info info = registeredProcesses[procID].info;
	OrderedProcess orderedProc;
	orderedProc.process = procID;
	orderedProc.order = info.tickOrder;
	size_t i = 0;
	for (size_t n = tickProcessesOrdered.size(); i < n; ++i) {
		if (tickProcessesOrdered[i].order > orderedProc.order) {
			tickProcessesOrdered.insert(tickProcessesOrdered.begin() + i, orderedProc);
			break;
		}
	}
	if (i == tickProcessesOrdered.size()) tickProcessesOrdered.push_back(orderedProc);
	orderedProc.order = info.renderOrder;
	i = 0;
	for (size_t n = renderProcessesOrdered.size(); i < n; ++i) {
		if (renderProcessesOrdered[i].order > orderedProc.order) {
			renderProcessesOrdered.insert(renderProcessesOrdered.begin() + i, orderedProc);
			break;
		}
	}
	if (i == renderProcessesOrdered.size()) renderProcessesOrdered.push_back(orderedProc);
}

void ProcessManager::removeProcessFromSortedBins(uint32_t procID) {
	for (size_t i = 0, n = tickProcessesOrdered.size(); i < n; ++i) {
		if (tickProcessesOrdered[i].process == procID) {
			tickProcessesOrdered.erase(tickProcessesOrdered.begin() + i);
			break;
		}
	}
	for (size_t i = 0, n = renderProcessesOrdered.size(); i < n; ++i) {
		if (renderProcessesOrdered[i].process == procID) {
			renderProcessesOrdered.erase(renderProcessesOrdered.begin() + i);
			break;
		}
	}
}

void ProcessManager::removeProcess(uint32_t procIndex) {
	removeProcessFromSortedBins(procIndex);
	registeredProcesses.erase(registeredProcesses.begin() + procIndex);
}
