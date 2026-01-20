#include "BreakableObjectPrefab.h"

#include <Engine.h>
#include <Shapes.h>
#include "Game/Scripts/Stats/HealthStat.h"
#include "Game/Scripts/Player/Breakable.h"

BreakableObjectPrefab::BreakableObjectPrefab(gce::Scene& _scene, json* _properties) :Prefab(_scene, _properties)
{
	gce::GameObject& breackabaleObject = m_root;

	breackabaleObject.SetName("BreakableObject");
	breackabaleObject.SetTags(TAG_ENEMY);

	gce::BoxCollider& collectableCollider = *breackabaleObject.AddComponent<gce::BoxCollider>();
	collectableCollider.localScale = { 1.0f, 1.0f, 1.0f };
	collectableCollider.isTrigger = false;

	gce::MeshRenderer& chromeMeshRenderer = *breackabaleObject.AddComponent<gce::MeshRenderer>();
	chromeMeshRenderer.SetGeometry(gce::SHAPES.CUBE);

	breackabaleObject.transform.SetLocalPosition({ -5.0f, 0.5f, 10.0f });
	breackabaleObject.transform.SetLocalScale({ 2.0f, 5.0f, 1.0f });
	HealthStat& keyScript = *breackabaleObject.AddScript<HealthStat>();
	keyScript.Init(1);

	Breakable& breakableScript = *breackabaleObject.AddScript<Breakable>();
}

BreakableObjectPrefab::~BreakableObjectPrefab() {}