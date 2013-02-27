#include "Includes.h"
#ifndef __TERRAINMANAGER_H__
#define __TERRAINMANAGER_H__

namespace NxOgre
{
	class SceneGeometry;
}

namespace EricOgreEngine
{
	class DLL_EXPORT TerrainManager
	{
	public:
		~TerrainManager(void);
		void CreateTerrain(std::string terrainName, std::string fileExtension, 
			std::string heightmapFileName, std::string directionalLightName, 
			Ogre::uint16 terrainSize, Ogre::Real worldSize, Ogre::Real inputScale);
		static TerrainManager* GetInstance(void);
	private:
		TerrainManager(void);

		//Ogre::Terrain stuff
		void GetTerrainImage(bool flipX, bool flipY, Ogre::Image& img);
		void DefineTerrain(long x, long y);
		void InitBlendMaps(Ogre::Terrain* terrain);
		void ConfigureTerrainDefaults(Ogre::Light* light);
		void CreatePhysicsShape(void);
		void TerrainManager::LoadTerrainGeometry(const Ogre::String& name, float* data, 
			Ogre::uint16 size, Ogre::Real worldSize, Ogre::Real minHeight, Ogre::Real maxHeight, 
			const Ogre::Vector3& position);

		Ogre::TerrainGlobalOptions* mTerrainGlobals;
		Ogre::TerrainGroup* mTerrainGroup;
		bool mTerrainsImported;

		std::string mHeightMapFileName;

		Ogre::uint16 mTerrainSize;
		Ogre::Real mWorldSize;
		Ogre::Real mInputScale;

		NxOgre::SceneGeometry *mSceneGeometry;

		static TerrainManager* m_pInstance;
	};
}
#endif