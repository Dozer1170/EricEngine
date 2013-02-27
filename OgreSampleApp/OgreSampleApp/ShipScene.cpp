#include "stdafx.h"
#include "ShipScene.h"
#include "GameManager.h"
#include "GameObject.h"
#include "Ship.h"
#include "Caelum.h"
#include "TerrainManager.h"
#include "Camera.h"
#include "PlayerController.h"

using namespace EricOgreEngine;

ShipScene::ShipScene(void)
{
}


ShipScene::~ShipScene(void)
{
}

void ShipScene::load(void)
{
	GameManager* manager = GameManager::GetInstance();

	// Set the scene's ambient light
	Ogre::Vector3 lightdir(0.55f, -0.3f, 0.75f);
    lightdir.normalise();
 
    Ogre::Light* light = manager->GetSceneManager()->createLight("tstLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(lightdir);
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue(0.4f, 0.4f, 0.4f));
 
	manager->GetSceneManager()->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f));

	manager->SetCaelumSystem(Caelum::CaelumSystem::CAELUM_COMPONENTS_ALL);

	TerrainManager::GetInstance()->CreateTerrain("ShipSceneTerrain", "dat", "terrain.png", "tstLight", 513, 12000.0f, 600);
 
	manager->GetCamera()->GetCameraTranslationNode()->setPosition(Ogre::Vector3(0, 300, -100));
	manager->GetCamera()->GetOgreCamera()->lookAt(0,300,0);
	Ship* ship = new Ship("Ship1", "transportship.mesh", Ogre::Vector3(0,300,0), 40.0f);
	PlayerController* controller = new PlayerController(ship);
	ship->SetPlayerController(controller);

	Ship* shipTwo = new Ship("Ship2", "transportship.mesh", Ogre::Vector3(-40, 300, 0), 40.0f);
}

void ShipScene::exitScene(void)
{
	GameManager::GetInstance()->DestroyScene();
}