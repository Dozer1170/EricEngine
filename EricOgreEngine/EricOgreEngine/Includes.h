#ifndef __INCLUDES_H__
#define __INCLUDES_H__
#include <string>
#include <vector>
#include <OgreException.h>
#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>
#include <Terrain\OgreTerrain.h>
#include <Terrain\OgreTerrainGroup.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef ENGINE_CLIENT
	#define DLL_EXPORT __declspec(dllimport)
#else
	#define DLL_EXPORT __declspec(dllexport)
#endif

#endif //ifndef