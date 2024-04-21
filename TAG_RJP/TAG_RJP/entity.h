/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#pragma once

#include <stdint.h>

class IEntity
{
public:
	static const uint32_t kInvalidEntityType = 0;
	static const uint32_t kInvalidEntityID = 0;

	IEntity()
		: type(kInvalidEntityType)
		, id(kInvalidEntityID)
	{
		listNext = this;
		listPrev = this;
	}

	IEntity(const IEntity& rhs) = delete;
	IEntity& operator = (const IEntity& rhs) = delete;

	virtual void save() const = 0;
	virtual void load() = 0;

	int getEntityType() const { return type; }
	int getEntityID() const { return id; }
	IEntity* nextEntity() const { return listNext; }
	IEntity* prevEntity() const { return listPrev; }

protected:
	virtual ~IEntity() = default;

private:

	friend class EntityManager;

	void insertAfter(IEntity* other) 
	{
		remove();
		listPrev = other;
		listNext = other->listNext;
		listPrev->listNext = this;
		listNext->listPrev = this;
	}
	void insertBefore(IEntity* other) 
	{
		remove();
		listNext = other;
		listPrev = other->listPrev;
		listPrev->listNext = this;
		listNext->listPrev = this;
	}
	void remove() 
	{
		IEntity* p = listPrev;
		IEntity* n = listNext;
		listNext->listPrev = p;
		listPrev->listNext = n;
		listNext = this;
		listPrev = this;
	}

	uint32_t type;
	uint32_t id;
	IEntity* listNext;
	IEntity* listPrev;
};

