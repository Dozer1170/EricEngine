#include "Includes.h"
#include "Camera.h"
#include "GameManager.h"

namespace EricOgreEngine
{
	Camera::Camera(void)
		: mCamera(nullptr),
		mCameraNode(nullptr)
	{
		// Create the camera
		mCamera = GameManager::GetInstance()->GetSceneManager()->createCamera("PlayerCam");
		mCamera->setNearClipDistance(0.1f);

		mCameraNode = GameManager::GetInstance()->GetSceneManager()->getRootSceneNode()->createChildSceneNode();
		mCameraNode->attachObject(mCamera);
	}


	Camera::~Camera(void)
	{
	}

	void Camera::TrySetInfiniteFarClip(Ogre::Root& root)
	{
		if (root.getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
		{
			mCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can
		}
	}

	void Camera::LookAt(Ogre::Vector3& point)
	{
		mCameraNode->lookAt(point, Ogre::Node::TransformSpace::TS_WORLD);
	}

	void Camera::LookAt(float x, float y, float z)
	{
		mCameraNode->lookAt(Ogre::Vector3(x,y,z), Ogre::Node::TransformSpace::TS_WORLD);
	}

	Ogre::SceneNode* Camera::GetCameraNode(void)
	{
		return mCameraNode;
	}

	Ogre::Camera* Camera::GetOgreCamera(void)
	{
		return mCamera;
	}
}