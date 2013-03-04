#pragma once
#include "GameObject.h"

class PlayerController;

class Ship : public EricOgreEngine::GameObject
{
public:
	Ship(std::string objectName, std::string meshName, const Ogre::Vector3& position, float mass);
	~Ship(void);
	void Start(void);
	void Update(float deltaTime);
	void OnCollision(GameObject *objectHit);

	void SetPlayerController(PlayerController *controller);

	void FireLaser(void);

	float mVelocity;
	float mAcceleration;
private:
	PlayerController *mPlayerController;
	int mLaserSound;
};

