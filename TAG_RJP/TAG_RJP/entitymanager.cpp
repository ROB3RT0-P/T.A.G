/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#include "entitymanager.h"

EntityManager::EntityManager()
	: firstEntity(NULL)
	, nextEntityID(1)
{

}

void EntityManager::registerEntityCreator(uint32_t type, entityCreateFunc* create, entityDestroyFunc* destroy, void* user) 
{
	EntityFactoryData factoryData;
	factoryData.createFunc = create;
	factoryData.destroyFunc = destroy;
	factoryData.user = user;
	factoryMap[type] = factoryData;
}

IEntity* EntityManager::createEntity(uint32_t type) 
{
	EntityFactorMap::iterator itr = factoryMap.find(type);
	if (itr == factoryMap.end()) return NULL;
	IEntity* newEntity = itr->second.createFunc(itr->second.user);
	newEntity->id = nextEntityID++;
	newEntity->type = type;
	if (firstEntity == NULL) 
	{
		firstEntity = newEntity;
	} else {
		newEntity->insertBefore(firstEntity);
	}
	return newEntity;
}

void EntityManager::destroyEntity(IEntity* entity) 
{
	EntityFactorMap::iterator itr = factoryMap.find(entity->type);
	if (entity == firstEntity) 
	{
		firstEntity = firstEntity->nextEntity();
		if (entity == firstEntity) firstEntity = NULL;
	}
	entity->remove();
	itr->second.destroyFunc(entity, itr->second.user);
}
