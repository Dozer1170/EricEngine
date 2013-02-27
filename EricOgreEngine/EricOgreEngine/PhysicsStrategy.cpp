#include "Includes.h"
#include "PhysicsStrategy.h"
#include "GameManager.h"
#include "GameObject.h"
#include "PhysicsCallback.h"
#include <Critter.h>

namespace EricOgreEngine
{
	PhysicsStrategy::PhysicsStrategy(GameObject *owner, std::string meshName, Ogre::Vector3 position, float mass)
		: mBody(nullptr),
		mOwner(nullptr)
	{
		Critter::BodyDescription desc;
		desc.mMass = mass;

		mCallback = new PhysicsCallback(this);

		mBody = GameManager::GetInstance()->GetRenderSystem()->createBody(
			NxOgre::BoxDescription(1,1,1), NxOgre::Vec3(position.x, position.y, position.z), meshName, desc);
		mBody->setGroup(1);
		mBody->setContactCallback(mCallback);

		mOwner = owner;
	}

	PhysicsStrategy::~PhysicsStrategy(void)
	{
	}

	Ogre::Quaternion PhysicsStrategy::GetGlobalOrientationQuat(void)
	{
		return mBody->getGlobalOrientationQuat().as<Ogre::Quaternion>();
	}

	void PhysicsStrategy::SetGlobalOrientationQuat(float w, float x, float y, float z)
	{
		mBody->setGlobalOrientationQuat(w, x, y ,z);
	}

	Ogre::Vector3 PhysicsStrategy::GetGlobalPosition(void)
	{
		return mBody->getGlobalPosition().as<Ogre::Vector3>();
	}

	void PhysicsStrategy::SetGlobalPosition(Ogre::Vector3 *pos)
	{
		mBody->setGlobalPosition(pos->x, pos->y, pos->z);
	}

	void PhysicsStrategy::SetGlobalPosition(float x, float y, float z)
	{
		mBody->setGlobalPosition(x, y ,z);
	}

	Ogre::SceneNode* PhysicsStrategy::GetBodySceneNode(void)
	{
		return mBody->getSceneNode();
	}

	void PhysicsStrategy::SetGravityEnabled(bool enable)
	{
		if(enable)
		{
			mBody->clearDynamicFlag(NxOgre::DynamicRigidbodyFlags::Flags::DisableGravity);
		}
		else
		{
			mBody->raiseDynamicFlag(NxOgre::DynamicRigidbodyFlags::Flags::DisableGravity);
		}
	}

	Ogre::Vector3 PhysicsStrategy::GetLinearVelocity(void)
	{
		return mBody->getLinearVelocity().as<Ogre::Vector3>();
	}

	void PhysicsStrategy::SetLinearVelocity(Ogre::Vector3 *velocity)
	{
		mBody->setLinearVelocity(velocity->x, velocity->y, velocity->z);
	}

	void PhysicsStrategy::SetLinearVelocity(float x, float y, float z)
	{
		mBody->setLinearVelocity(x, y, z);
	}

	void PhysicsStrategy::SetLinearDamping(float dampingCoefficient)
	{
		mBody->setLinearDamping(dampingCoefficient);
	}

	void PhysicsStrategy::SetAngularDamping(float dampingCoefficient)
	{
		mBody->setAngularDamping(dampingCoefficient);
	}

	void PhysicsStrategy::OnCollision(const NxOgre::ContactPair& pair)
	{
		GameObject *other;
		if(pair.mFirst->getName().compare(mOwner->name))
		{
			other = GameManager::GetInstance()->GetGameObject(pair.mFirst->getName());
		}
		else
		{
			other = GameManager::GetInstance()->GetGameObject(pair.mFirst->getName());
		}
		mOwner->OnCollision(other);
	}
}