/*******************************************
	Written by Robert Parry [RJP] - 2024
	Refer to main.cpp for the license
*******************************************/

#include "player.h"
#include "game.h"

Player::Player(float x, float y) : fPlayerX_(x), fPlayerY_(y) {};

static IEntity* playerEntityCreateFunc(void* user) 
{
	Player* playerEntity = new Player();
	// RJP - Should be already loaded but increases the ref count
	global::resourceManager()->loadResource(raw_enum(global::Res::PlayerSprite));

	return playerEntity;
}

static IEntity* playerEntityDestroyFunc(IEntity* entity, void* user) 
{
	delete static_cast<Player*>(entity);

	global::resourceManager()->unloadResource(raw_enum(global::Res::PlayerSprite));

	return NULL;
}

void Player::registerWithEntityManager() 
{
	global::entityManager()->registerEntityCreator(raw_enum(global::EntityType::Player), playerEntityCreateFunc, playerEntityDestroyFunc, NULL);
}

void Player::init()
{
    tTexture = nullptr;
	fPlayerAcceleration_ = 500.0f;
	fPlayerVelocityX_ = 0;
	fPlayerVelocityY_ = 0;
	fPlayerSpeed_ = 500.0f;
	iPlayerHealth_ = 0;
	uTurnsRemaining_ = 10;
	uCorrectTurns_ = 0;
}

void Player::update(float deltaTime)
{
	fPlayerVelocityX_ = SDL_clamp(fPlayerVelocityX_, -fPlayerSpeed_, fPlayerSpeed_);
	fPlayerVelocityY_ = SDL_clamp(fPlayerVelocityY_, -fPlayerSpeed_, fPlayerSpeed_);

	fPlayerX_ += fPlayerVelocityX_ * deltaTime;
	fPlayerY_ += fPlayerVelocityY_ * deltaTime;

	checkBounds();
}

void Player::move(float newX, float newY, float deltaTime)
{
	// Calculate the distance to move based on playerSpeed and deltaTime
	float moveDistance = fPlayerSpeed_ * deltaTime;

	// Calculate the target position after movement
	float targetX = fPlayerX_ + (newX * moveDistance);
	float targetY = fPlayerY_ + (newY * moveDistance);

	// Interpolate between current position and target position
	fPlayerX_ = lerp(fPlayerX_, targetX, deltaTime);
	fPlayerY_ = lerp(fPlayerY_, targetY, deltaTime);

	//checkBounds();
}

void Player::floatPlayer()
{
	fPlayerX_ += fPlayerFloatSpeedX_ * 0.5f;
	fPlayerY_ += fPlayerFloatSpeedY_;

	if (fPlayerX_ < 10.0f || fPlayerX_ >(540.0f - fBoundOffset_)) fPlayerFloatSpeedX_ = -fPlayerFloatSpeedX_;
	if (fPlayerY_ < 10.0f || fPlayerY_ >(960.0f - fBoundOffset_)) fPlayerFloatSpeedY_ = -fPlayerFloatSpeedY_;
}

float Player::lerp(float start, float end, float t)
{
	return start + t * (end - start);
}

void Player::checkBounds()
{
	if (fPlayerX_ < 10.0f) fPlayerX_ = 10.0f;
	if (fPlayerY_ < 10.0f) fPlayerY_ = 10.0f;
	if (fPlayerX_ > (540.0f - fBoundOffset_)) fPlayerX_ = (540.0f - fBoundOffset_);
	if (fPlayerY_ > (960.0f - fBoundOffset_)) fPlayerY_ = (960.0f - fBoundOffset_);
}

void Player::render() 
{
	tTexture = global::resourceManager()->getResourceAsTexture(raw_enum(global::Res::PlayerSprite));
	//global::game()->blit(tTexture, fPlayerX_, fPlayerY_);
}

void Player::save() const
{

}

void Player::load()
{

}