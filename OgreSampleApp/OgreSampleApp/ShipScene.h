#include "stdafx.h"
#ifndef __ShipScene_h_
#define __ShipScene_h_
#include "Scene.h"

class ShipScene : public EricOgreEngine::Scene
{
public:
	ShipScene(void);
	~ShipScene(void);
	void load();
	void exitScene();
};
#endif
