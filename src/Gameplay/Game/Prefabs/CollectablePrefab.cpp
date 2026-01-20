#include "CollectablePrefab.h"

#include <Engine.h>
#include <Shapes.h>

#include "Framework/ResourceManager.h"

#include "Game/Scripts/UI/Healler.h"

CollectablePrefab::CollectablePrefab(gce::Scene& _scene, json* _properties) : Prefab(_scene, _properties) {
	gce::Geometry* GEO = RESOURCE_MANAGER.GetGeometry("healbox");
	//gce::Texture* TEX = RESOURCE_MANAGER.GetTexture("tex-key-color");

	gce::GameObject& collectableObject = m_root;
	collectableObject.SetName("Collectable");
	collectableObject.SetTags(TAG_DEFAULT);
	collectableObject.transform.SetLocalPosition({ -10.0f, 1.0f, 10.0f });


	gce::SphereCollider& collectableCollider = *collectableObject.AddComponent<gce::SphereCollider>();
	collectableCollider.localScale = 1.0f;
	collectableCollider.isTrigger = true;

	gce::MeshRenderer& chromeMeshRenderer = *collectableObject.AddComponent<gce::MeshRenderer>();
	chromeMeshRenderer.SetGeometry(GEO);

	Healler& heallerScript = *collectableObject.AddScript<Healler>();

}
CollectablePrefab::~CollectablePrefab() {}