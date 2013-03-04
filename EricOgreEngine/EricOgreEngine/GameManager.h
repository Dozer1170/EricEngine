#include "Includes.h"
#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__
#include "Caelum.h"

namespace OIS
{
	class KeyEvent;
	class MouseEvent;
	class InputManager;
	class Mouse;
	class Keyboard;
}

namespace NxOgre
{
	class World;
	class Scene;
	class Material;
}

namespace Critter
{
	class RenderSystem;
	class Renderable;
}

namespace EricOgreEngine
{
	class Scene;
	class GameObject;
	class EngineInputManager;
	class TerrainManager;
	class Camera;
	class SoundManager;

	class DLL_EXPORT GameManager : public Ogre::FrameListener, public Ogre::WindowEventListener
	{
	public:
		void AddGameObject(GameObject& gameObject);
		void RemoveGameObject(GameObject& gameObject);
		GameObject* GetGameObject(std::string name);
		void LoadScene(Scene& scene);
		void DestroyScene(void);
		void SetCaelumSystem(Caelum::CaelumSystem::CaelumComponent components);
		Ogre::SceneManager* GetSceneManager(void);
		Critter::RenderSystem* GetRenderSystem(void);
		Camera* GetCamera(void);
		static GameManager* GetInstance(void);

	private:
		GameManager(void);
		~GameManager(void);
		void Update(float deltaTime);
		bool setup(void);
		void run(void);
		bool configure(void);
		void chooseSceneManager(void);
		void createCamera(void);
		void createFrameListener(void);
		void createViewports(void);
		void setupResources(void);
		void createResourceListener(void);
		void loadResources(void);
		// Ogre::FrameListener
		bool frameRenderingQueued(const Ogre::FrameEvent& evt);
		// Ogre::WindowEventListener
		//Adjust mouse clipping area
		void windowResized(Ogre::RenderWindow* rw);
		//Unattach OIS before window shutdown (very important under Linux)
		void windowClosed(Ogre::RenderWindow* rw);

		Ogre::Root *mRoot;
		Ogre::SceneManager* mSceneMgr;
		Ogre::RenderWindow* mWindow;
		Ogre::String mResourcesCfg;
		Ogre::String mPluginsCfg;
		Ogre::Viewport* mViewport;

		Caelum::CaelumSystem* mCaelumSystem;
		NxOgre::World* mWorld;
		NxOgre::Scene* mScene;
		NxOgre::Material* mDefaultMaterial;
		Critter::RenderSystem* mRenderSystem;
		Critter::Renderable* mVisualDebuggerRenderable;
		Ogre::SceneNode* mVisualDebuggerNode;

		bool mShutDown;

		Camera* mActiveCamera;
		EngineInputManager* mInputManager;
		TerrainManager* mTerrainManager;
		SoundManager* mSoundManager;

		Scene* mCurrentScene;
		std::vector<GameObject*> mGameObjects;

		float mLastUpdateTime;
		float mDeltaTime;

		static GameManager* m_pInstance;
	};
}
#endif

