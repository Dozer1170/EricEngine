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
		Ogre::SceneNode* GetCameraNode(void);
		Ogre::Camera* GetOgreCamera(void);
		void LookAt(Ogre::Vector3& point);
		void LookAt(float x, float y, float z);

	private:
		Ogre::Camera* mCamera;
		Ogre::SceneNode *mCameraNode;
	};
}
#endif