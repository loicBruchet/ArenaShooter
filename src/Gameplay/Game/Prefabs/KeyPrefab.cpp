#include "KeyPrefab.h"

#include <Engine.h>
#include <Shapes.h>

#include "Framework/ResourceManager.h"

#include "Game/Scripts/UI/RecoltKey.h"

KeyPrefab::KeyPrefab(gce::Scene& _scene, json* _properties) : Prefab(_scene, _properties)
{
	gce::Geometry* GEO = RESOURCE_MANAGER.GetGeometry("key");
	gce::Texture* TEX = RESOURCE_MANAGER.GetTexture("tex-key-color");

	gce::GameObject& keyObject = m_root;

	keyObject.SetName("Key");
	keyObject.SetTags(TAG_DEFAULT);
	keyObject.transform.SetLocalPosition({ -15.0f, 0.5f, 12.0f });

	gce::SphereCollider& collectableCollider = *keyObject.AddComponent<gce::SphereCollider>();
	collectableCollider.localScale = 1.0f;
	collectableCollider.isTrigger = true;

	gce::MeshRenderer& chromeMeshRenderer = *keyObject.AddComponent<gce::MeshRenderer>();
	chromeMeshRenderer.SetGeometry(GEO);
	chromeMeshRenderer.SetAlbedoTexture(TEX);

	RecoltKey& keyScript = *keyObject.AddScript<RecoltKey>();
}

KeyPrefab::~KeyPrefab() {}