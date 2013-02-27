#include "Includes.h"
#include "TerrainManager.h"
#include "GameManager.h"
#include <Critter.h>

namespace EricOgreEngine
{
	TerrainManager* TerrainManager::m_pInstance = nullptr;

	TerrainManager::TerrainManager(void)
		: mTerrainGlobals(nullptr),
		mTerrainGroup(nullptr),
		mTerrainsImported(false),
		mHeightMapFileName(Ogre::StringUtil::BLANK),
		mTerrainSize(0),
		mWorldSize(0),
		mInputScale(0),
		mSceneGeometry(nullptr)
	{
	}

	TerrainManager::~TerrainManager(void)
	{
		OGRE_DELETE mTerrainGroup;
		OGRE_DELETE mTerrainGlobals;
	}

	TerrainManager* TerrainManager::GetInstance(void)
	{
		if(m_pInstance == nullptr)
			m_pInstance = new TerrainManager();

		return m_pInstance;
	}

	void TerrainManager::CreateTerrain(std::string terrainName, std::string fileExtension,
									   std::string heightMapFileName, std::string directionalLightName,
									   Ogre::uint16 terrainSize, Ogre::Real worldSize, Ogre::Real inputScale)
	{
		mHeightMapFileName = heightMapFileName;
		mTerrainSize = terrainSize;
		mWorldSize = worldSize;
		mInputScale = inputScale;

		mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
		mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(GameManager::GetInstance()->GetSceneManager(),
			Ogre::Terrain::ALIGN_X_Z, mTerrainSize, mWorldSize);
		mTerrainGroup->setFilenameConvention(Ogre::String(terrainName), fileExtension);
		mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

		ConfigureTerrainDefaults(GameManager::GetInstance()->GetSceneManager()->getLight(directionalLightName));

		for (long x = 0; x <= 0; ++x)
			for (long y = 0; y <= 0; ++y)
				DefineTerrain(x, y);
 
		// sync load since we want everything in place when we start
		mTerrainGroup->loadAllTerrains(true);

		if (mTerrainsImported)
		{
			Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
			while(ti.hasMoreElements())
			{
				Ogre::Terrain* t = ti.getNext()->instance;
				InitBlendMaps(t);
			}
			//mTerrainGroup->saveAllTerrains(true);
			mTerrainsImported = false;
		}

		mTerrainGroup->freeTemporaryResources();

		CreatePhysicsShape();
	}

	void TerrainManager::CreatePhysicsShape(void)
	{
		Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
		while (ti.hasMoreElements())
		{
			Ogre::Terrain *t = ti.getNext()->instance;
			LoadTerrainGeometry(t->getMaterialName(), t->getHeightData(), 
				t->getSize(), t->getWorldSize(), t->getMinHeight(), t->getMaxHeight(), t->getPosition());
		}
	}

	void TerrainManager::LoadTerrainGeometry(const Ogre::String& name, float* data, Ogre::uint16 size, Ogre::Real worldSize, Ogre::Real minHeight, Ogre::Real maxHeight, const Ogre::Vector3& position)
	{
	   // Create the manual heightfield
	   NxOgre::ManualHeightField mhf;
	   mhf.begin(size, size);
	   Ogre::Real normMin = -32768.0f;
	   Ogre::Real normMax = 32767.0f;
	   // Sample the data to the manual heightfield
	   for(int x = 0; x < size; ++x)
	   {
		  NxOgre::Enums::HeightFieldTesselation tess = NxOgre::Enums::HeightFieldTesselation_NW_SE;
		  for(int z = size-1; z >= 0; --z)
		  {
			 Ogre::Real height = data[(size * z) + x];
			 short sample = (short)(((height - minHeight) / (maxHeight - minHeight)) * (normMax - normMin) + normMin);
			 mhf.sample(sample, 0, 0, tess);
			 if(tess == NxOgre::Enums::HeightFieldTesselation_NE_SW)
				tess = NxOgre::Enums::HeightFieldTesselation_NW_SE;
			 else
				tess = NxOgre::Enums::HeightFieldTesselation_NE_SW;
		  }
	   }
	   // Create the actual heightfield
	   NxOgre::HeightField *hf = mhf.end(name.c_str());
	   Ogre::Real hf_size = worldSize + (worldSize / size);
	   Ogre::Real hf_height = (maxHeight - minHeight) / 2.0f;
	   Ogre::Real hf_pose_x = position.x - (worldSize / 2.0f);
	   Ogre::Real hf_pose_y = position.y + ((maxHeight + minHeight) / 2.0f);
	   Ogre::Real hf_pose_z = position.z - (worldSize / 2.0f);
	#if NxOgreVersionMajor <= 1 && NxOgreVersionMinor <= 5
	   NxOgre::HeightFieldGeometry* hfg = new NxOgre::HeightFieldGeometry(hf, NxOgre::Vec3(hf_size, hf_height, hf_size));
	   hfg->setLocalPose(NxOgre::Matrix44(NxOgre::Vec3(hf_pose_x, hf_pose_y, hf_pose_z)));
	   mSceneGeometry = GameManager::GetInstance()->GetRenderSystem()->getScene()->createSceneGeometry(hfg);
	#else
	   NxOgre::HeightFieldGeometryDescription desc(hf, NxOgre::Vec3(hf_size, hf_height, hf_size));

	   mSceneGeometry = GameManager::GetInstance()->GetRenderSystem()->getScene()->createSceneGeometry(
		   desc, NxOgre::Matrix44(NxOgre::Vec3(hf_pose_x, hf_pose_y, hf_pose_z)));
	   mSceneGeometry->setGroup(1);
	#endif
	}

	void TerrainManager::GetTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
	{
		img.load(mHeightMapFileName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		if (flipX)
			img.flipAroundY();
		if (flipY)
			img.flipAroundX();
	}

	void TerrainManager::DefineTerrain(long x, long y)
	{
		Ogre::String filename = mTerrainGroup->generateFilename(x, y);
		if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
		{
			mTerrainGroup->defineTerrain(x, y);
		}
		else
		{
			Ogre::Image img;
			GetTerrainImage(x % 2 != 0, y % 2 != 0, img);
			mTerrainGroup->defineTerrain(x, y, &img);
			mTerrainsImported = true;
		}
	}

	void TerrainManager::InitBlendMaps(Ogre::Terrain* terrain)
	{
		Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
		Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
		Ogre::Real minHeight0 = 70;
		Ogre::Real fadeDist0 = 40;
		Ogre::Real minHeight1 = 70;
		Ogre::Real fadeDist1 = 15;
		float* pBlend0 = blendMap0->getBlendPointer();
		float* pBlend1 = blendMap1->getBlendPointer();
		for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
		{
			for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
			{
				Ogre::Real tx, ty;
 
				blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
				Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
				Ogre::Real val = (height - minHeight0) / fadeDist0;
				val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
				*pBlend0++ = val;
 
				val = (height - minHeight1) / fadeDist1;
				val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
				*pBlend1++ = val;
			}
		}
		blendMap0->dirty();
		blendMap1->dirty();
		blendMap0->update();
		blendMap1->update();
	}

	void TerrainManager::ConfigureTerrainDefaults(Ogre::Light* light)
	{
		if(light)
		{
			// Configure global
			mTerrainGlobals->setMaxPixelError(8);
			// testing composite map
			mTerrainGlobals->setCompositeMapDistance(5000);

			// Important to set these so that the terrain knows what to use for derived (non-realtime) data
			mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
			mTerrainGlobals->setCompositeMapAmbient(GameManager::GetInstance()->GetSceneManager()->getAmbientLight());
			mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());

			// Configure default import settings for if we use imported image
			Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
			defaultimp.terrainSize = mTerrainSize;
			defaultimp.worldSize = mWorldSize;
			defaultimp.inputScale = mInputScale; // due terrain.png is 8 bpp
			defaultimp.minBatchSize = 33;
			defaultimp.maxBatchSize = 65;

			// textures
			defaultimp.layerList.resize(3);
			defaultimp.layerList[0].worldSize = 100;
			defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
			defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
			defaultimp.layerList[1].worldSize = 30;
			defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
			defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
			defaultimp.layerList[2].worldSize = 200;
			defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
			defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
		}
		else
		{
			throw Ogre::Exception(-1, 
				Ogre::String("Light given to TerrainManager was a nullptr! Please provide a valid light"), "");
		}
	}
}