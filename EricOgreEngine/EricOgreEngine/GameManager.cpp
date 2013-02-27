#include "Includes.h"
#include "GameManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "EngineInputManager.h"
#include "TerrainManager.h"
#include "Camera.h"
#include <Caelum.h>
#include <NxOgre.h>
#include <Critter.h>

namespace EricOgreEngine
{
	GameManager* GameManager::m_pInstance = nullptr;

	GameManager::GameManager(void)
		: mRoot(nullptr),
		mActiveCamera(nullptr),
		mSceneMgr(nullptr),
		mWindow(nullptr),
		mResourcesCfg(Ogre::StringUtil::BLANK),
		mPluginsCfg(Ogre::StringUtil::BLANK),
		mShutDown(false),
		mInputManager(nullptr),
		mTerrainManager(nullptr),
		mCurrentScene(nullptr),
		mCaelumSystem(nullptr),
		mWorld(nullptr),
		mLastUpdateTime(0),
		mDeltaTime(0),
		mGameObjects()
	{
	}

	GameManager::~GameManager(void)
	{
		//Remove ourself as a Window listener
		Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
		windowClosed(mWindow);
		delete mRoot;
		mWorld->destroyWorld();
	}

	GameManager* GameManager::GetInstance(void)
	{
		if(!m_pInstance)
			m_pInstance = new GameManager();

		return m_pInstance;
	}

	Ogre::SceneManager* GameManager::GetSceneManager(void)
	{
		return mSceneMgr;
	}

	Critter::RenderSystem* GameManager::GetRenderSystem(void)
	{
		return mRenderSystem;
	}

	Camera* GameManager::GetCamera(void)
	{
		return mActiveCamera;
	}

	void GameManager::LoadScene(Scene& scene)
	{
		if(mCurrentScene != nullptr)
			mCurrentScene->exitScene();

		mCurrentScene = &scene;
		run();
	}

	void GameManager::DestroyScene(void)
	{
		mTerrainManager->~TerrainManager();
		mInputManager->~EngineInputManager();
	}

	void GameManager::AddGameObject(GameObject& gameObject)
	{
		mGameObjects.push_back(&gameObject);
	}

	void GameManager::RemoveGameObject(GameObject& gameObject)
	{
		std::vector<GameObject*>::iterator it;
		for(it = mGameObjects.begin(); it != mGameObjects.end(); it++)
		{
			if(strcmp((*it)->name.c_str(), gameObject.name.c_str()))
			{
				mGameObjects.erase(it);
				break;
			}
		}
	}

	/// Returns GameObject with given name if it exists, else returns nullptr
	GameObject* GameManager::GetGameObject(std::string name)
	{
		std::vector<GameObject*>::iterator it;
		for(it = mGameObjects.begin(); it != mGameObjects.end(); it++)
		{
			if(strcmp((*it)->name.c_str(), name.c_str()))
			{
				return (*it);
			}
		}
		return nullptr;
	}

	void GameManager::SetCaelumSystem(Caelum::CaelumSystem::CaelumComponent components)
	{
		mCaelumSystem = new Caelum::CaelumSystem(mRoot, mSceneMgr, components);
		mCaelumSystem->attachViewport(mViewport);
		mRoot->addFrameListener(mCaelumSystem);
		mCaelumSystem->setManageSceneFog(Ogre::FogMode::FOG_NONE);
		mCaelumSystem->setGlobalFogDensityMultiplier(0.001f);
		mCaelumSystem->setTimeScale(25);
	}

	void GameManager::Update(float deltaTime)
	{
		mInputManager->Update(deltaTime);

		std::vector<GameObject*>::iterator it;
		for(it = mGameObjects.begin(); it != mGameObjects.end(); it++)
		{
			if(!(*it)->started)
			{
				(*it)->Start();
				(*it)->started = true;
			}

			(*it)->Update(deltaTime);
		}

		mWorld->advance(deltaTime);
		mInputManager->LateUpdate(deltaTime);

		if(mCaelumSystem)
		{
			mCaelumSystem->updateSubcomponents(deltaTime);
			mCaelumSystem->notifyCameraChanged(mActiveCamera->GetOgreCamera());
		}

		if(EngineInputManager::GetInstance()->IsKeyDown(OIS::KC_ESCAPE))
			mShutDown = true;
	}

	void GameManager::run(void)
	{
	#ifdef _DEBUG
		mResourcesCfg = "resources_d.cfg";
		mPluginsCfg = "plugins_d.cfg";
	#else
		mResourcesCfg = "resources.cfg";
		mPluginsCfg = "plugins.cfg";
	#endif

		if (!setup())
			return;

		mLastUpdateTime = (float) mRoot->getTimer()->getMilliseconds();
		mCurrentScene->load();

		mRoot->startRendering();

		// clean up
		DestroyScene();
	}

	bool GameManager::configure(void)
	{
		// Show the configuration dialog and initialise the system
		// You can skip this and use root.restoreConfig() to load configuration
		// settings if you were sure there are valid ones saved in ogre.cfg
		if(mRoot->showConfigDialog())
		{
			// If returned true, user clicked OK so initialise
			// Here we choose to let the system create a default rendering window by passing 'true'
			mWindow = mRoot->initialise(true, "TutorialApplication Render Window");

			return true;
		}
		else
		{
			return false;
		}
	}
	//-------------------------------------------------------------------------------------
	void GameManager::chooseSceneManager(void)
	{
		// Get the SceneManager, in this case a generic one
		mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
	}
	//-------------------------------------------------------------------------------------
	void GameManager::createCamera(void)
	{
		mActiveCamera = new Camera();
		mActiveCamera->TrySetInfiniteFarClip(*mRoot);
	}
	//-------------------------------------------------------------------------------------
	void GameManager::createFrameListener(void)
	{
		Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;

		mWindow->getCustomAttribute("WINDOW", &windowHnd);
		windowHndStr << windowHnd;
    
		mInputManager = EngineInputManager::GetInstance();
		mInputManager->Init(windowHndStr.str());

		//Set initial mouse clipping size
		windowResized(mWindow);

		//Register as a Window listener
		Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

		// create a params panel for displaying sample details
		Ogre::StringVector items;
		items.push_back("cam.pX");
		items.push_back("cam.pY");
		items.push_back("cam.pZ");
		items.push_back("");
		items.push_back("cam.oW");
		items.push_back("cam.oX");
		items.push_back("cam.oY");
		items.push_back("cam.oZ");
		items.push_back("");
		items.push_back("Filtering");
		items.push_back("Poly Mode");

		mRoot->addFrameListener(this);
	}
	//-------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------
	void GameManager::createViewports(void)
	{
		// Create one viewport, entire window
		mViewport = mWindow->addViewport(mActiveCamera->GetOgreCamera());
		mViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));

		// Alter the camera aspect ratio to match the viewport
		mActiveCamera->GetOgreCamera()->setAspectRatio(
			Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));
	}
	//-------------------------------------------------------------------------------------
	void GameManager::setupResources(void)
	{
		// Load resource paths from config file
		Ogre::ConfigFile cf;
		cf.load(mResourcesCfg);

		// Go through all sections & settings in the file
		Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

		Ogre::String secName, typeName, archName;
		while (seci.hasMoreElements())
		{
			secName = seci.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
			Ogre::ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				typeName = i->first;
				archName = i->second;
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
					archName, typeName, secName);
			}
		}
	}
	//-------------------------------------------------------------------------------------
	void GameManager::createResourceListener(void)
	{

	}
	//-------------------------------------------------------------------------------------
	void GameManager::loadResources(void)
	{
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	}
	//-------------------------------------------------------------------------------------
	bool GameManager::setup(void)
	{
		mRoot = new Ogre::Root(mPluginsCfg);
		chooseSceneManager();
		mWorld = NxOgre::World::createWorld();

		// Create the scene
		NxOgre::SceneDescription scene_description;
		scene_description.mGravity = NxOgre::Constants::MEAN_EARTH_GRAVITY;
		scene_description.mUseHardware = false;

		mScene = mWorld->createScene(scene_description);
  
		// Set default material properties
		mDefaultMaterial = mScene->getMaterial(0);
		mDefaultMaterial->setRestitution(0.1f);
		mDefaultMaterial->setDynamicFriction(0.9f);
		mDefaultMaterial->setStaticFriction(0.5f);
  
		// Plane creation
		mScene->createSceneGeometry(NxOgre::PlaneGeometryDescription());

		// Create the rendersystem.
		mRenderSystem = new Critter::RenderSystem(mScene, mSceneMgr);

#ifdef _DEBUG
		/*
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingletonPtr()->create("RenderSystem.VisualDebugger", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		material->createTechnique();
		material->getTechnique(0)->createPass();
		material->getTechnique(0)->getPass(0)->setDepthBias(1);
		material->getTechnique(0)->getPass(0)->setAmbient(1,1,1);
		material->getTechnique(0)->getPass(0)->setSelfIllumination(1,1,1);
		material->getTechnique(0)->getPass(0)->setDiffuse(1,1,1,1);
		material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

		mVisualDebuggerRenderable = new Critter::Renderable(NxOgre::Enums::RenderableType_VisualDebugger);
		mVisualDebuggerRenderable->setMaterial("RenderSystem.VisualDebugger");
		mWorld->getVisualDebugger()->setRenderable(mVisualDebuggerRenderable);
		mVisualDebuggerRenderable->setCastShadows(false);
		mVisualDebuggerNode = GameManager::GetInstance()->GetSceneManager()->getRootSceneNode()->createChildSceneNode();
		mVisualDebuggerNode->attachObject(mVisualDebuggerRenderable);

		mWorld->getVisualDebugger()->setVisualisationMode(NxOgre::Enums::VisualDebugger_ShowAll);
		mVisualDebuggerNode->setVisible(true);
		*/
#endif

		setupResources();

		bool carryOn = configure();
		if (!carryOn) return false;

		createCamera();
		createViewports();

		// Set default mipmap level (NB some APIs ignore this)
		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

		// Create any resource listeners (for loading screens)
		createResourceListener();
		// Load resources
		loadResources();

		createFrameListener();

		mTerrainManager = TerrainManager::GetInstance();

		return true;
	};
	//-------------------------------------------------------------------------------------
	bool GameManager::frameRenderingQueued(const Ogre::FrameEvent& evt)
	{
		if(mWindow->isClosed())
			return false;

		if(mShutDown)
		{
			mCaelumSystem->~CaelumSystem();
			DestroyScene();
			return false;
		}

		float currentTime = (float) mRoot->getTimer()->getMilliseconds();
		mDeltaTime = (currentTime - mLastUpdateTime) / 1000;

		Update(mDeltaTime);

		mLastUpdateTime = currentTime;

		return true;
	}
	//-------------------------------------------------------------------------------------
	//Adjust mouse clipping area
	void GameManager::windowResized(Ogre::RenderWindow* rw)
	{
		unsigned int width, height, depth;
		int left, top;
		rw->getMetrics(width, height, depth, left, top);

		mInputManager->windowResized(width, height);
	}

	//Unattach OIS before window shutdown (very important under Linux)
	void GameManager::windowClosed(Ogre::RenderWindow* rw)
	{
		//Only close for window that created OIS (the main window in these demos)
		if( rw == mWindow )
		{
			mInputManager->windowClosed();
		}
	}
}