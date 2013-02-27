#include "Includes.h"
#ifndef __PHYSICS_CALLBACK_H__
#define __PHYSICS_CALLBACK_H__
#include "NxOgreCallback.h"

namespace NxOgre
{
	class ContactPair;
}

namespace EricOgreEngine
{
	class PhysicsStrategy;

	class PhysicsCallback : public NxOgre::Callback
	{
	public:
		PhysicsCallback(PhysicsStrategy *mOwner);
		~PhysicsCallback(void);
		void onContact(const NxOgre::ContactPair& pair);
	private:
		PhysicsStrategy *mOwner;
	};
}
#endif