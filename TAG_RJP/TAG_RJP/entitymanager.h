/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#pragma once

#include "entity.h"
#include <unordered_map>

class EntityManager
{
public:
	typedef IEntity* (entityCreateFunc)(void* user); 
	typedef IEntity* (entityDestroyFunc)(IEntity* entity, void* user);

	EntityManager();

	void registerEntityCreator(uint32_t type, entityCreateFunc* create, entityDestroyFunc* destroy, void* user);

	IEntity* createEntity(uint32_t type);
	void destroyEntity(IEntity* entity);

private:

	struct EntityFactoryData
	{
		entityCreateFunc* createFunc;
		entityDestroyFunc* destroyFunc;
		void* user;
	};

	typedef std::unordered_map<uint32_t, EntityFactoryData> EntityFactorMap;

	IEntity* firstEntity;
	EntityFactorMap factoryMap;
	uint32_t nextEntityID;
};