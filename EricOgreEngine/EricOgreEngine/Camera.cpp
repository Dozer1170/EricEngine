#include "Includes.h"
#include "Camera.h"
#include "GameManager.h"

namespace EricOgreEngine
{
	Camera::Camera(void)
		: mCamera(nullptr),
		cameraNode(nullptr),
		cameraRollNode(nullptr),
		cameraPitchNode(nullptr),
		cameraYawNode(nullptr)
	{
		// Create the camera
		mCamera = GameManager::GetInstance()->GetSceneManager()->createCamera("PlayerCam");
		mCamera->setNearClipDistance(0.1f);

		cameraNode = GameManager::GetInstance()->GetSceneManager()->getRootSceneNode()->createChildSceneNode();
		cameraNode->attachObject(mCamera);
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

	Ogre::SceneNode* Camera::GetCameraTranslationNode(void)
	{
		return cameraNode;
	}

	Ogre::Camera* Camera::GetOgreCamera(void)
	{
		return mCamera;
	}
}