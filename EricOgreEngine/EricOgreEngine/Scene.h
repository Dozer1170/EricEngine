#include "Includes.h"
#ifndef __Scene_h__
#define __Scene_h__
namespace EricOgreEngine
{
	class DLL_EXPORT Scene
	{
	public:
		Scene(void);
		~Scene(void);
		virtual void load() = 0;
		virtual void exitScene() = 0;
	};
}
#endif