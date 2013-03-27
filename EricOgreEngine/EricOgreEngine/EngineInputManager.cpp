#include "Includes.h"
#include "EngineInputManager.h"
#include <OISEvents.h>
#include <OISInputManager.h>
#include <CEGUISystem.h>
#include "Point.h"

namespace EricOgreEngine
{

	EngineInputManager* EngineInputManager::m_pInstance = nullptr;

	EngineInputManager::EngineInputManager()
		:  mMouse(nullptr),
		mKeyboard(nullptr),
		mMouseDelta(nullptr),
		mPreviousMousePosition(nullptr)
	{
	}

	EngineInputManager::~EngineInputManager(void)
	{
	}

	void EngineInputManager::Init(std::string windowHndStr)
	{
		OIS::ParamList pl;

		mKeysReleasedThisFrameMap = new std::map<OIS::KeyCode, bool>();

		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr));

		mOISInputManager = OIS::InputManager::createInputSystem( pl );
		mKeyboard = static_cast<OIS::Keyboard*>(mOISInputManager->createInputObject( OIS::OISKeyboard, true ));
		mMouse = static_cast<OIS::Mouse*>(mOISInputManager->createInputObject( OIS::OISMouse, true ));

		mMouse->setEventCallback(this);
		mKeyboard->setEventCallback(this);

		mPreviousMousePosition = new Point(mMouse->getMouseState().X.abs, mMouse->getMouseState().Y.abs);
		mMouseDelta = new Point(0,0);
		UpdatePreviousMouseState();
	}

	EngineInputManager* EngineInputManager::GetInstance(void)
	{
		if(!m_pInstance)
			m_pInstance = new EngineInputManager();

		return m_pInstance;
	}

	void EngineInputManager::UpdatePreviousMouseState(void)
	{
		OIS::MouseState currentState = mMouse->getMouseState();
		mPrevMouseState = OIS::MouseState();
		mPrevMouseState.buttons = currentState.buttons;
		mPrevMouseState.X = currentState.X;
		mPrevMouseState.Y = currentState.Y;
		mPrevMouseState.Z = currentState.Z;
	}

	void EngineInputManager::Update(float deltaTime)
	{
		//Need to capture/update each device
		mKeyboard->capture();
		mMouse->capture();

		int currX = mMouse->getMouseState().X.abs;
		int currY = mMouse->getMouseState().Y.abs;
		mMouseDelta->x = mMouse->getMouseState().X.rel;
		mMouseDelta->y = mMouse->getMouseState().Y.rel;
		mPreviousMousePosition->x = currX;
		mPreviousMousePosition->y = currY;
	}

	void EngineInputManager::LateUpdate(float deltaTime)
	{
		std::map<OIS::KeyCode, bool>::iterator it;
		for(it = mKeysReleasedThisFrameMap->begin(); it != mKeysReleasedThisFrameMap->end(); it++)
		{
			it->second = false;
		}

		UpdatePreviousMouseState();
	}

	bool EngineInputManager::GetMouseButtonDown(OIS::MouseButtonID buttonID)
	{
		return mMouse->getMouseState().buttonDown(buttonID);
	}

	bool EngineInputManager::GetMouseButtonPressed(OIS::MouseButtonID buttonID)
	{
		return mMouse->getMouseState().buttonDown(buttonID) 
			&& !mPrevMouseState.buttonDown(buttonID);
	}

	bool EngineInputManager::GetMouseButtonReleased(OIS::MouseButtonID buttonID)
	{
		return !mMouse->getMouseState().buttonDown(buttonID) 
			&& mPrevMouseState.buttonDown(buttonID);
	}

	Point* EngineInputManager::GetMousePosition(void)
	{
		const OIS::MouseState state = mMouse->getMouseState();
		return &Point(state.X.abs, state.Y.abs);
	}

	Point* EngineInputManager::GetMouseDelta(void)
	{
		return mMouseDelta;
	}

	bool EngineInputManager::IsKeyDown(OIS::KeyCode key)
	{
		return mKeyboard->isKeyDown(key);
	}

	bool EngineInputManager::IsKeyReleased(OIS::KeyCode key)
	{
		return false;
	}

	void EngineInputManager::windowResized(int width, int height)
	{
		const OIS::MouseState &ms = mMouse->getMouseState();
		ms.width = width;
		ms.height = height;
	}

	void EngineInputManager::windowClosed(void)
	{
		if(mOISInputManager)
		{
			mOISInputManager->destroyInputObject( mMouse );
			mOISInputManager->destroyInputObject( mKeyboard );

			OIS::InputManager::destroyInputSystem(mOISInputManager);
			mOISInputManager = 0;
		}
	}

	bool EngineInputManager::keyPressed( const OIS::KeyEvent &arg )
	{
		CEGUI::System &sys = CEGUI::System::getSingleton();
		sys.injectKeyDown(arg.key);
		sys.injectChar(arg.text);
		return true;
	}

	bool EngineInputManager::keyReleased( const OIS::KeyEvent &arg )
	{
		(*mKeysReleasedThisFrameMap)[arg.key] = true;
		CEGUI::System::getSingleton().injectKeyUp(arg.key);
		return true;
	}

	bool EngineInputManager::mouseMoved( const OIS::MouseEvent &arg )
	{
		CEGUI::System &sys = CEGUI::System::getSingleton();
		sys.injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
		// Scroll wheel.
		if (arg.state.Z.rel)
			sys.injectMouseWheelChange(arg.state.Z.rel / 120.0f);

		return true;
	}

	bool EngineInputManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		CEGUI::System::getSingleton().injectMouseButtonDown(ConvertButtonPressToCEGUI(id));
		return true;
	}

	bool EngineInputManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		CEGUI::System::getSingleton().injectMouseButtonUp(ConvertButtonPressToCEGUI(id));
		return true;
	}

	CEGUI::MouseButton EngineInputManager::ConvertButtonPressToCEGUI(OIS::MouseButtonID button)
	{
		switch (button)
		{
		case OIS::MB_Left:
			return CEGUI::LeftButton;
 
		case OIS::MB_Right:
			return CEGUI::RightButton;
 
		case OIS::MB_Middle:
			return CEGUI::MiddleButton;
 
		default:
			return CEGUI::LeftButton;
		}
	}
}