#include "stdafx.h"
#include "PlayerController.h"
#include "Ship.h"
#include "EngineInputManager.h"
#include "Point.h"
#include "PhysicsStrategy.h"
#include "GameManager.h"
#include "Camera.h"

using namespace EricOgreEngine;

PlayerController::PlayerController(Ship *controlledShip)
{
	mShip = controlledShip;

	Ogre::SceneNode* cameraTranslationNode = GameManager::GetInstance()->GetCamera()->GetCameraTranslationNode();
	mShip->AddChild(cameraTranslationNode);
	Ogre::Vector3 cameraPosition = mShip->GetOrientation().xAxis() * 10;
	cameraPosition.y += 1;
	cameraTranslationNode->setPosition(cameraPosition);
	GameManager::GetInstance()->GetCamera()->GetOgreCamera()->lookAt
		(mShip->GetGlobalPosition() + mShip->GetOrientation().xAxis() * -200);
}

PlayerController::~PlayerController(void)
{
}

void PlayerController::Update(float deltaTime)
{
	PhysicsStrategy *strat = mShip->GetPhysicsStrategy();
	Ogre::Vector3 finalVel = strat->GetLinearVelocity();
	if(EngineInputManager::GetInstance()->IsKeyDown(OIS::KeyCode::KC_W))
	{
		finalVel += mShip->GetOrientation().xAxis() * -mShip->mAcceleration * deltaTime;
	}
	if(EngineInputManager::GetInstance()->IsKeyDown(OIS::KeyCode::KC_S))
	{
		if(mShip->mVelocity > 0)
		{
			finalVel -= mShip->GetOrientation().xAxis() * -mShip->mAcceleration * deltaTime;
		}
		else
		{
			mShip->mVelocity = 0;
		}
	}

	Point* mouseDelta = EngineInputManager::GetInstance()->GetMouseDelta();
	if(mouseDelta->x != 0)
	{
		mShip->Yaw(Ogre::Degree((float) -mouseDelta->x * 0.13f));
	}
	if(mouseDelta->y != 0)
	{
		mShip->Roll(Ogre::Degree((float) mouseDelta->y * 0.13f));
	}

	if(EngineInputManager::GetInstance()->IsKeyDown(OIS::KeyCode::KC_E))
	{
		mShip->Pitch(Ogre::Degree(-1));
	}

	if(EngineInputManager::GetInstance()->IsKeyDown(OIS::KeyCode::KC_Q))
	{
		mShip->Pitch(Ogre::Degree(1));
	}

	if(EngineInputManager::GetInstance()->GetMouseButtonPressed(OIS::MouseButtonID::MB_Left))
	{
		mShip->FireLaser();
	}

	strat->SetLinearVelocity(finalVel.x, finalVel.y, finalVel.z);
}