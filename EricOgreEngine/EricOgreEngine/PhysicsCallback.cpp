#include "Includes.h"
#include "PhysicsCallback.h"
#include "PhysicsStrategy.h"

namespace EricOgreEngine
{
	PhysicsCallback::PhysicsCallback(PhysicsStrategy *owner)
		: mOwner(nullptr)
	{
		mOwner = owner;
	}


	PhysicsCallback::~PhysicsCallback(void)
	{
	}

	void PhysicsCallback::onContact(const NxOgre::ContactPair& pair)
	{
		mOwner->OnCollision(pair);
	}
}