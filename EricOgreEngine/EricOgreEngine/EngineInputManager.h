#include "Includes.h"
#ifndef __INPUTMANAGER_H__
#define __INPUTMANAGER_H__
#include "OISKeyboard.h"
#include "OISMouse.h"

namespace CEGUI
{
	enum MouseButton;
}

namespace EricOgreEngine
{
	class Point;

	class DLL_EXPORT EngineInputManager :  public OIS::KeyListener, public OIS::MouseListener
	{
	public:
		~EngineInputManager(void);
		void Init(std::string windowHndStr);
		void Update(float deltaTime);
		void LateUpdate(float deltaTime);
		bool GetMouseButtonDown(OIS::MouseButtonID buttonID);
		bool GetMouseButtonPressed(OIS::MouseButtonID buttonID);
		bool GetMouseButtonReleased(OIS::MouseButtonID buttonID);
		Point* GetMousePosition(void);
		Point* GetMouseDelta(void);
		bool IsKeyDown(OIS::KeyCode key);
		bool IsKeyReleased(OIS::KeyCode key);
		void windowResized(int width, int height);
		void windowClosed(void);

		static EngineInputManager* GetInstance(void);
	private:
		EngineInputManager(void);

		void UpdatePreviousMouseState(void);
		CEGUI::MouseButton ConvertButtonPressToCEGUI(OIS::MouseButtonID button);

		// OIS::KeyListener
		bool keyPressed( const OIS::KeyEvent &arg );
		bool keyReleased( const OIS::KeyEvent &arg );
		// OIS::MouseListener
		bool mouseMoved( const OIS::MouseEvent &arg );
		bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

		OIS::InputManager* mOISInputManager;
		//OIS Input devices
		OIS::Mouse    *mMouse;
		OIS::Keyboard *mKeyboard;

		OIS::MouseState mPrevMouseState;
		Point *mPreviousMousePosition;
		Point *mMouseDelta;

		std::map<OIS::KeyCode, bool>* mKeysReleasedThisFrameMap;

		static EngineInputManager* m_pInstance;
	};
}
#endif