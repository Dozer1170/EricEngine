#include "Includes.h"
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

namespace EricOgreEngine
{
	class PhysicsStrategy;

	class DLL_EXPORT GameObject
	{
	public:
		GameObject(std::string objectName, std::string meshName);
		GameObject(std::string objectName, std::string meshName, const Ogre::Vector3& position);
		GameObject(std::string objectName, std::string meshName, const Ogre::Vector3& position, float mass);
		~GameObject(void);

		//Virtual functions
		virtual void Start(void);
		virtual void Update(float deltaTime);
		virtual void OnCollision(GameObject *objectHit);


		Ogre::Quaternion GetOrientation(void);
		void SetOrientation(Ogre::Quaternion *rot);
		Ogre::Vector3 GetGlobalPosition(void);
		void SetGlobalPosition(Ogre::Vector3 *pos);
		void Translate(Ogre::Vector3& diff);
		void ParentTo(GameObject *parent);
		void UnParent(void);
		void AddChild(Ogre::MovableObject *child);
		void AddChild(GameObject *child);
		void AddChild(Ogre::SceneNode *child);
		void RemoveChild(std::string name);
		Ogre::SceneNode& GetSceneNode(void);

		PhysicsStrategy* GetPhysicsStrategy(void);

		void Yaw(Ogre::Degree degrees, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TransformSpace::TS_LOCAL);
		void Pitch(Ogre::Degree degrees, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TransformSpace::TS_LOCAL);
		void Roll(Ogre::Degree degrees, Ogre::Node::TransformSpace relativeTo = Ogre::Node::TransformSpace::TS_LOCAL);

		bool started;
		std::string name;

	private:
		void Init(std::string objectName, std::string meshName, const Ogre::Vector3& position, float mass);
		PhysicsStrategy* mPhysicsStrategy;
	};
}
#endif