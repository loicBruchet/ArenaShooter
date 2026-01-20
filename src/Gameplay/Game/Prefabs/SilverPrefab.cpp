#include "SilverPrefab.h"

#include <Engine.h>
#include <Shapes.h>

#include "Framework/Scripts/Agent.h"
#include "Game/Scripts/Enemy/SilverAI.h"

#include "Game/Scripts/Stats/HealthStat.h"
#include "Game/Scripts/Stats/ShieldStat.h"

#include "Framework/ResourceManager.h"

#include "Game/Tags.h"

SilverPrefab::SilverPrefab(gce::Scene& _scene, json* _properties) : Prefab(_scene, _properties) {
	gce::Geometry* GEO = RESOURCE_MANAGER.GetGeometry("Mesh_Ennemy_Silver");

	gce::GameObject& silver_object = m_root;
	silver_object.SetName("Silver");
	silver_object.SetTags(TAG_DEFAULT | TAG_ENTITY | TAG_ENEMY);


	gce::SphereCollider& collider = *silver_object.AddComponent<gce::SphereCollider>();
	collider.localRay = 0.3f;
	collider.localScale = 4.0f;
	collider.isTrigger = false;
	silver_object.transform.SetLocalPosition({ 10.5f,1.5f,10.5f });

	gce::MeshRenderer& mesh = *silver_object.AddComponent<gce::MeshRenderer>();
	mesh.SetGeometry(GEO);
	silver_object.SetName("Silver");

	HealthStat& health = *silver_object.AddScript<HealthStat>();
	health.Init(100);

	Agent* agent = silver_object.AddScript<Agent>();

	SilverAI& controller = *silver_object.AddScript<SilverAI>();
}
SilverPrefab::~SilverPrefab() {}