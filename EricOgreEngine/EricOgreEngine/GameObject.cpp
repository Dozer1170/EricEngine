#include "Includes.h"
#include "GameManager.h"
#include "GameObject.h"
#include "PhysicsStrategy.h"
#include "NxOgre.h"

namespace EricOgreEngine
{
	GameObject::GameObject(std::string objectName, std::string meshName)
		: started(false),
		mPhysicsStrategy(nullptr)
	{
		Init(objectName, meshName, Ogre::Vector3(0,0,0), 1);
	}

	GameObject::GameObject(std::string objectName, std::string meshName, const Ogre::Vector3& position)
		: started(false),
		mPhysicsStrategy(nullptr)
	{
		Init(objectName, meshName, position, 1);
	}

	GameObject::GameObject(std::string objectName, std::string meshName, const Ogre::Vector3& position, float mass)
		: started(false),
		mPhysicsStrategy(nullptr)
	{
		Init(objectName, meshName, position, mass);
	}

	void GameObject::Init(std::string objectName, std::string meshName, const Ogre::Vector3& position, float mass)
	{
		name = objectName;

		mPhysicsStrategy = new PhysicsStrategy(this, meshName, position, mass);

		mPhysicsStrategy->SetGlobalPosition(position.x, position.y, position.z);
		GameManager::GetInstance()->AddGameObject(*this);
	}

	GameObject::~GameObject(void)
	{
		delete mPhysicsStrategy;
	}

	void GameObject::Start(void)
	{

	}

	void GameObject::Update(float deltaTime)
	{

	}

	Ogre::Quaternion GameObject::GetOrientation(void)
	{
		return mPhysicsStrategy->GetGlobalOrientationQuat();
	}

	void GameObject::SetOrientation(Ogre::Quaternion *rot)
	{
		mPhysicsStrategy->SetGlobalOrientationQuat(rot->w, rot->x, rot->y, rot->z);
	}

	Ogre::Vector3 GameObject::GetGlobalPosition(void)
	{
		return mPhysicsStrategy->GetGlobalPosition();
	}

	void GameObject::SetGlobalPosition(Ogre::Vector3 *pos)
	{
		mPhysicsStrategy->SetGlobalPosition(pos->x, pos->y, pos->z);
	}

	void GameObject::Translate(Ogre::Vector3& diff)
	{
		NxOgre::Vec3 pos = mPhysicsStrategy->GetGlobalPosition();
		mPhysicsStrategy->SetGlobalPosition(pos.x + diff.x, pos.y + diff.y, pos.z + diff.z);
	}

	void GameObject::ParentTo(GameObject *parent)
	{
		UnParent();

		parent->AddChild(this);	
	}

	void GameObject::UnParent(void)
	{
		Ogre::Node *currParent = mPhysicsStrategy->GetBodySceneNode()->getParent();
		if(currParent != NULL)
		{
			currParent->removeChild(mPhysicsStrategy->GetBodySceneNode()->getName());
		}	
	}

	void GameObject::AddChild(Ogre::MovableObject *child)
	{
		Ogre::Node *currParent = child->getParentNode();
		if(currParent != NULL)
		{
			currParent->removeChild(mPhysicsStrategy->GetBodySceneNode()->getName());
		}

		mPhysicsStrategy->GetBodySceneNode()->attachObject(child);
	}

	void GameObject::AddChild(GameObject *child)
	{
		mPhysicsStrategy->GetBodySceneNode()->addChild(&child->GetSceneNode());
	}

	void GameObject::AddChild(Ogre::SceneNode *child)
	{
		Ogre::Node *currParent = child->getParent();
		if(currParent != NULL)
		{
			if(currParent->getName().compare("Ogre/SceneNode"))
			{
				GameManager::GetInstance()->GetSceneManager()->getRootSceneNode()->removeChild(child);
			}
			else
			{
				currParent->removeChild(mPhysicsStrategy->GetBodySceneNode()->getName());
			}
		}

		mPhysicsStrategy->GetBodySceneNode()->addChild(child);
	}

	void GameObject::RemoveChild(std::string name)
	{
		mPhysicsStrategy->GetBodySceneNode()->removeChild(name);
	}

	Ogre::SceneNode& GameObject::GetSceneNode(void)
	{
		return *mPhysicsStrategy->GetBodySceneNode();
	}

	PhysicsStrategy* GameObject::GetPhysicsStrategy(void)
	{
		return mPhysicsStrategy;
	}

	void GameObject::OnCollision(GameObject *objectHit)
	{

	}

	void GameObject::Yaw(Ogre::Degree degrees, Ogre::Node::TransformSpace relativeTo)
	{
		mPhysicsStrategy->GetBodySceneNode()->yaw(degrees, relativeTo);
		Ogre::Quaternion quat = mPhysicsStrategy->GetBodySceneNode()->getOrientation();
		mPhysicsStrategy->SetGlobalOrientationQuat(quat.w, quat.x, quat.y, quat.z);
	}

	void GameObject::Pitch(Ogre::Degree degrees, Ogre::Node::TransformSpace relativeTo)
	{
		mPhysicsStrategy->GetBodySceneNode()->pitch(degrees, relativeTo);
		Ogre::Quaternion quat = mPhysicsStrategy->GetBodySceneNode()->getOrientation();
		mPhysicsStrategy->SetGlobalOrientationQuat(quat.w, quat.x, quat.y, quat.z);
	}

	void GameObject::Roll(Ogre::Degree degrees, Ogre::Node::TransformSpace relativeTo)
	{
		mPhysicsStrategy->GetBodySceneNode()->roll(degrees, relativeTo);
		Ogre::Quaternion quat = mPhysicsStrategy->GetBodySceneNode()->getOrientation();
		mPhysicsStrategy->SetGlobalOrientationQuat(quat.w, quat.x, quat.y, quat.z);
	}
}