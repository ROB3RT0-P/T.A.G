/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#pragma once

#include "global.h"
#include "resourcemanager.h"
#include "entitymanager.h"
#include "entity.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

struct SDL_Texture;

class Player : public IEntity
{
public:
	static void registerWithEntityManager();
	
	Player(float x, float y);

	void init(float fScreenWidth, float fScreenHeight);
	void init();
	void render();
	void update(float deltaTime);
	void move(float newX, float newY, float deltaTime);
	float lerp(float start, float end, float t);
	void floatPlayer();
	void checkBounds();

	void setPlayerX(float newPosX) { fPlayerX_ = newPosX; }
	void setPlayerY(float newPosY) { fPlayerY_ = newPosY; }

	float getPlayerX() { return fPlayerX_; }
	float getPlayerY() { return fPlayerY_; }

	Player() = default;
	~Player() {};

	int getPlayerHealth() { return iPlayerHealth_; }
	void damagePlayerHealth(int damage) { iPlayerHealth_ -= damage; }
	void setPlayerHealth(int healthAmount)
	{
		iPlayerHealth_ += healthAmount;
		if (iPlayerHealth_ > 100) { iPlayerHealth_ = 100; }
	}

	SDL_Texture& getTexture() { return *tTexture; }

	float getPlayerVelocityX(){ return fPlayerVelocityX_; }
	float getPlayerVelocityY() { return fPlayerVelocityY_; }
	float getPlayerAcceleration() { return fPlayerAcceleration_; }

	void setPlayerVelocityX(float fNewVelocityX) { fPlayerVelocityX_ = fNewVelocityX; }
	void setPlayerVelocityY(float fNewVelocityY) { fPlayerVelocityY_ = fNewVelocityY; }
	void setPlayerAcceleration( float fNewAcceleration) { }

	Uint32 getPlayerTurnsRemaining() { return uTurnsRemaining_; }
	Uint32 getPlayerCorrectTurns() { return uCorrectTurns_;  }

	void decrementPlayerTurnsRemaining() { --uTurnsRemaining_; }
	void incrementPlayerCorrectTurns() { ++uCorrectTurns_; }

private:
	friend IEntity* playerEntityCreateFunc(void* user);
	friend IEntity* playerEntityDestroyFunc(IEntity* entity, void* user);

	virtual void save() const;
	virtual void load();

	SDL_Texture* tTexture = NULL;

	Uint32 uTurnsRemaining_;
	Uint32 uCorrectTurns_;

	float fPlayerVelocityX_;
	float fPlayerVelocityY_;

	float fPlayerY_;
	float fPlayerX_;
	float fPlayerAcceleration_;
	float fPlayerFloatSpeedY_;
	float fPlayerFloatSpeedX_;

	float fPlayerSpeed_;
	int iPlayerHealth_;
	float fBoundOffset_;
};