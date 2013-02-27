#include "Includes.h"
#ifndef __PHYSICS_STRATEGY__
#define __PHYSICS_STRATEGY__

namespace Critter
{
	class Body;
}

namespace NxOgre
{
	class Quat;
	class Vec3;
	class ContactPair;
}

namespace EricOgreEngine
{
	class GameObject;
	class PhysicsCallback;

	class DLL_EXPORT PhysicsStrategy
	{
	public:
		PhysicsStrategy::PhysicsStrategy(GameObject *owner, std::string meshName, Ogre::Vector3 position, float mass);
		~PhysicsStrategy(void);
		Ogre::SceneNode* GetBodySceneNode(void);

		//Orientation
		Ogre::Quaternion GetGlobalOrientationQuat(void);
		void SetGlobalOrientationQuat(float w, float x, float y, float z);

		//Position
		Ogre::Vector3 GetGlobalPosition(void);
		void SetGlobalPosition(Ogre::Vector3 *pos);
		void SetGlobalPosition(float x, float y, float z);

		void SetGravityEnabled(bool enabled);
		Ogre::Vector3 GetLinearVelocity(void);
		void SetLinearVelocity(Ogre::Vector3 *velocity);
		void SetLinearVelocity(float x, float y, float z);

		//Damping
		void SetLinearDamping(float dampingCoefficient);
		void SetAngularDamping(float dampingCoefficient);
		
		void OnCollision(const NxOgre::ContactPair& pair);
	private:
		Critter::Body *mBody;
		GameObject *mOwner;
		PhysicsCallback *mCallback;
	};
}
#endif