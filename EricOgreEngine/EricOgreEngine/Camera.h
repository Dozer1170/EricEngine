#include "Includes.h"
#ifndef __CAMERA_H__
#define __CAMERA_H__

namespace EricOgreEngine
{
	class DLL_EXPORT Camera
	{
	public:
		Camera(void);
		~Camera(void);
		void TrySetInfiniteFarClip(Ogre::Root& root);
		Ogre::SceneNode* GetCameraTranslationNode(void);
		Ogre::SceneNode* GetCameraYawNode(void);
		Ogre::SceneNode* GetCameraPitchNode(void);
		Ogre::SceneNode* GetCameraRollNode(void);
		Ogre::Camera* GetOgreCamera(void);

	private:
		Ogre::Camera* mCamera;
		Ogre::SceneNode *cameraNode;
		Ogre::SceneNode *cameraYawNode;
		Ogre::SceneNode *cameraPitchNode;
		Ogre::SceneNode *cameraRollNode;
	};
}
#endif