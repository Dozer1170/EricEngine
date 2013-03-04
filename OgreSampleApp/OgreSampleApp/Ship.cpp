#include "stdafx.h"
#include "Ship.h"
#include "EngineInputManager.h"
#include "SoundManager.h"
#include "Point.h"
#include "PhysicsStrategy.h"
#include "PlayerController.h"

using namespace EricOgreEngine;

Ship::Ship(std::string objectName, std::string meshName, const Ogre::Vector3& position, float mass) 
	: EricOgreEngine::GameObject(objectName, meshName, position, mass),
	mVelocity(0),
	mAcceleration(0),
	mPlayerController(nullptr)
{
}


Ship::~Ship(void)
{
}

void Ship::Start(void)
{
	mLaserSound = SoundManager::getSingletonPtr()->CreateSound(Ogre::String("blaster.wav"));

	mVelocity = 0;
	mAcceleration = 100.0f;

	GetPhysicsStrategy()->SetGravityEnabled(false);

	GetPhysicsStrategy()->SetAngularDamping(1.5f);
}

void Ship::Update(float deltaTime)
{
	if(mPlayerController != nullptr)
	{
		mPlayerController->Update(deltaTime);
	}
}

void Ship::SetPlayerController(PlayerController *controller)
{
	mPlayerController = controller;
}

void Ship::OnCollision(GameObject *objectHit)
{
	mVelocity = 0;
}

void Ship::FireLaser(void)
{
	int channelFireGun = 0;
	SoundManager::getSingletonPtr()->PlaySound(mLaserSound, &GetSceneNode(), &channelFireGun);
}